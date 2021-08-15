/***************************************************************************
    MICRO C CUBE / COMPACT, DEVICE DRIVER
    Ethernet driver for RX62N/RX63N

    Copyright (c) 2011-2012, eForce Co.,Ltd.  All rights reserved.

    Version Information
            2011.09.10: Created.
            2011.11.22: Updating for RX63N series.
 ***************************************************************************/

#include <string.h>
#include "kernel.h"
#include "DDR_RX_ETH_cfg.h"
#include "DDR_RX_ETH.h"
#include "DDR_PHY.h"
#include "net_hdr.h"
#ifdef NET_C_OS
#include "kernel_id.h"
#endif

/*
 *  Configuration Macros
 */

#ifndef ETH_PHY_MODE
#define ETH_PHY_MODE    0
    /*
     * Select PHY mode  (Default = 0)
     *  ETH_PHY_MODE = 0 : Auto select mode
     *               = 1 : 10M Half Duplex manual mode
     *               = 2 : 10M Full/Half (Duplex auto select mode)
     *               = 3 : 100M Half Duplex manual mode
     *               = 4 : 100M Full/Half (Duplex auto select mode)
     */
#endif

#ifndef ETH_FILTER_MODE
#define ETH_FILTER_MODE 0
    /*
     * Select Address Filter mode  (Default = 0)
     *  ETH_FILTER_MODE  = 0 : Filter disable (Perfect filtering)
     *                   = 1 : promiscuous mode (receive all packets)
     *                   = 2 : multicast filter mode
     */
#endif

#ifndef ETH_RXDESC_CNT
#define ETH_RXDESC_CNT  8
#endif

#ifndef ETH_TXDESC_CNT
#define ETH_TXDESC_CNT  8
#endif

#define PHY_POLL_TMOUT  500

#ifndef ETH_TX_TMOUT
#define ETH_TX_TMOUT    1000        /* 1 second */
#endif

#ifndef ETH_MAXLEN
#define ETH_MAXLEN      1514U       /* Maximum ethernet frame size */
#endif

#if (RX_SRS==630)       /* RX63N */
#define CFG_PRCR_ENB    /* Enable PRCR */
#endif

#ifndef NET_C_OS

static void eth_snd_tsk(VP_INT exinf);
static void eth_rcv_tsk(VP_INT exinf);
static void eth_ctl_tsk(VP_INT exinf);
static const T_CFLG c_eth_flg = {TA_TPRI | TA_WMUL, 0};
static const T_CMBX c_eth_snd_mbx  = { TA_TFIFO|TA_MFIFO, 0, NULL};
static const T_CMBX c_eth_rcv_mbx  = { TA_TFIFO|TA_MFIFO, 0, NULL};
static const T_CTSK c_eth_snd_tsk  = { TA_HLNG, NULL, (FP)eth_snd_tsk,  4,  1024, 0};
static const T_CTSK c_eth_rcv_tsk  = { TA_HLNG, NULL, (FP)eth_rcv_tsk,  4,  1024, 0};
static const T_CTSK c_eth_ctl_tsk  = { TA_HLNG, NULL, (FP)eth_ctl_tsk,  4,  1024, 0};
static ID ID_ETH_SND_TSK;
static ID ID_ETH_RCV_TSK;
static ID ID_ETH_CTL_TSK;
static ID ID_ETH_SND_MBX;
static ID ID_ETH_RCV_MBX;
static ID ID_ETH_FLG;
static ID ID_ETH_ISR;

void _ddr_rx62n_eth_intr(void);
const T_CISR _ddr_rx62n_eth_cisr =
        {TA_HLNG, NULL, INT_EINT, (FP)_ddr_rx62n_eth_intr, IPL_ETHERNET};
#else
#define ID_ETH_FLG  ID_ETH_RCV_FLG
#endif

/*************************************************************************
 * Descriptor and Data buffer setup definitions.
 *
 *  ETH_BUFSZ       : Data buffer block size (same for Rx and Tx)
 *  TXBUF_CNT       : TX Descriptor count
 *  RXBUF_CNT       : RX Descriptor count
 ************************************************************************/
#ifndef ETH_BUFSZ
#define ETH_BUFSZ       1536
#endif
#if ETH_BUFSZ < 256 || (ETH_BUFSZ & 0x1f) != 0
#error ETH_BUFSZ must be 32bytes boundary && greater than equal 256
#endif

#define TBUFSZ          ETH_BUFSZ
#define RBUFSZ          ETH_BUFSZ

#define TXBUF_CNT       ETH_TXDESC_CNT
#define RXBUF_CNT       ETH_RXDESC_CNT

#if (TXBUF_CNT * ETH_BUFSZ) < 2048 || (RXBUF_CNT * ETH_BUFSZ) < 2048
#error buffer-size * num-of-buffer must be greater than equal 2048
        /* because Tx-BDs may be underrun */
#endif

/*************************************************************************
 * Timeout (in ms units) definition for various wait events
 * used in this driver program.
 ************************************************************************/
#define PHY_RST_WAIT_TMO        500    /* 500ms timeout to poll PHY Reset over status */


/*
 * Bit control definitions
 */

#define BIT_INIT_FLAG       0x0001U     /* Initialization done status flag */
#define BIT_RX_ON           0x0002U     /* Rx running status */
#define BIT_TX_ON           0x0004U     /* Tx running status */
#define BIT_RX_OVRN         0x0008U     /* Rx overrun status */
#define BIT_TX_BUSY         0x0010U     /* Waiting for Tx event */
#define BIT_LINK_DOWN       0x0100U     /* PHY media link down status */

/*************************************************************************
 * IRQ synchronization event flag bit definitions
 ************************************************************************/

#define EVENT_SOFT      0x0001      /* Software INT test event */
#define EVENT_RX        0x0002      /* Rx OK event */
#define EVENT_RXERR     0x0004      /* Rx error event */
#define EVENT_TX        0x0008      /* Tx OK event */
#define EVENT_TXERR     0x0010      /* Tx error event */
#define EVENT_MII       0x0020      /* MII complete event */
#define EVENT_LINKUP    0x0200      /* Link regained */

/*************************************************************************
 * Control & Status structure definition
 ************************************************************************/

struct t_lan_ctl {
    UH      bit;            /* Bit mapped control / status definitions  */
    UH      physts;         /* PHY media status                         */
    UINT    txpkt_cnt;      /* Transmit packet count                    */
    UINT    txbyte_cnt;     /* Transmit total data byte count           */
    UINT    rxpkt_cnt;      /* Received packet count                    */
    UINT    rxpkt_errcnt;   /* Received/dropped error packet count      */
    UINT    rxbyte_cnt;     /* Received total data byte count           */
    UW      dly_cnt;
    UB     *mac;

    volatile struct st_edmac __evenaccess *edmac_p;
    volatile struct st_etherc __evenaccess *ether_p;
    T_ETH_BD *rbd_top;
#if DEBUG_ETH == 1
    T_ETH_BD *rbd_p;
#endif

    T_ETH_BD *tbd_top;
    T_ETH_BD *tbd_p;
};

/*************************************************************************
 * Local data and function declarations
 ************************************************************************/
volatile static struct t_lan_ctl  t_lan;

struct _eth_bd {
    T_ETH_BD rxbd[RXBUF_CNT];
  #if ((RXBUF_CNT * 16) & 0x0f) / 4 != 0
  #define PADDING_1 ((RXBUF_CNT * 16) & 0x0f) / 4
    UW padding_1[PADDING_1];  /* for align 16bytes */
  #endif
    T_ETH_BD txbd[TXBUF_CNT];
    UW padding_2[3];
};
struct _eth_bd eth_bd;

struct _eth_buffer {
    UW rxbuf[(RBUFSZ * RXBUF_CNT) / 4];
    UW txbuf[(TBUFSZ * TXBUF_CNT) / 4];
    UW padding_3[7];
};
struct _eth_buffer eth_buffer;

static UH eth_devnum;       /* Device Number*/


/*
 * control macros
 */

#ifndef DEBUG_ETH
#define DEBUG_ETH   0
#endif

#define DEF_TXINT   (EDMAC_EESR_TABT | EDMAC_EESIPR_TCIP)
#define DEF_RXINT   (EDMAC_EESIPR_RABTIP | EDMAC_EESIPR_FRIP | EDMAC_EESIPR_RDEIP |\
                     EDMAC_EESIPR_RFOFIP)
#define DEF_INT     0

#define BD_RERRS    (RX_BD_RFOF | RX_BD_ABORT | RX_BD_RRF | RX_BD_RTLF |\
                     RX_BD_RTSF | RX_BD_PRE   | RX_BD_CERF)

#define ALIGN(p,n)  ((((unsigned long)(p)) + (((unsigned long)(n)) - 1))\
                      & (~(((unsigned long)(n)) - 1)))

#define ETHER_IEN   IEN(ETHER,EINT)
#define ETHER_IPR   IPR(ETHER,EINT)
#define IOSYNC(a)   (a)?1:0

/*
 * delay for adjusted timing
 */

void mdio_delay(void)
{
    unsigned long cnt = t_lan.dly_cnt;
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;

    while (cnt--)
        ether->PSR.LONG;  /* dummy read */
}

/*
 * received a bit cycle
 */

static unsigned int mdio_recv_1(void)
{
    unsigned int d;
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;

    d = ether->PIR.BIT.MDI;
    ether->PIR.BIT.MDC = 1;
    mdio_delay();
    ether->PIR.BIT.MDC = 0;
    mdio_delay();

    return d;
}

/*
 * transmitted a bit cycle
 */

static void mdio_xmit_1(unsigned int d)
{
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;

    ether->PIR.LONG = d ? (ETHER_PIR_MDO|ETHER_PIR_MMD) : ETHER_PIR_MMD;
                                                /* MMD=1,MDO=d,MDC=0 */
    ether->PIR.BIT.MDC = 1;
    mdio_delay();
    ether->PIR.BIT.MDC = 0;
    mdio_delay();
}

/*
 * bus free
 */

static void mdio_bus_free(void)
{
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;

    ether->PIR.LONG = 0;        /* MMD=0,MDC=0 (bus free) */
    ether->PIR.BIT.MDC = 1;
    mdio_delay();
    ether->PIR.LONG = 0;
    mdio_delay();
}

/*
 * received N bits
 */

static UH mdio_recv_n(int i)
{
    UH d = 0, bit;

    for (bit = 0x8000; i != 0; i--, bit >>= 1)
        if (mdio_recv_1())
            d |= bit;

    return d;
}

/*
 * transmitted N bits
 */

static void mdio_xmit_n(int i, UH d)
{
    for (; i != 0; i--, d <<= 1)
        mdio_xmit_1((d & 0x8000) ? 1 : 0);
}

/*************************************************************************
 * PHY Register initialization / read / write access function.
 *      phy_get     = Read   data to 16-bit PHY Register
 *      phy_put     = Write  data to 16-bit MAC Register
 ************************************************************************/

/*
 * read from PHY
 */

static UH phy_get(const UINT reg)
{
    UH d;
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;

    mdio_xmit_n(16, 0xffff);    /* preamble */
    mdio_xmit_n(16, 0xffff);    /* preamble */
    mdio_xmit_n(14, MII_ST | MII_OP_R | MII_TA | (ETH_PHY_ADDR << MII_PA_SHIFT) | (reg << MII_RA_SHIFT));
    mdio_bus_free();            /* TA cycle */
    mdio_recv_1();              /* TA cycle */
    d = mdio_recv_n(16);

    return d;
}

/*
 * write to PHY
 */

static void phy_put(const UINT reg, UH d)
{
    mdio_xmit_n(16, 0xffff);    /* preamble */
    mdio_xmit_n(16, 0xffff);    /* preamble */
    mdio_xmit_n(16, MII_ST | MII_OP_W | MII_TA | (ETH_PHY_ADDR << MII_PA_SHIFT) | (reg << MII_RA_SHIFT));
    mdio_xmit_n(16, d);
    mdio_bus_free();
}

/*************************************************************************
 * Function  : phy_ini
 * Purpose   : PHY device initialization
 * Arguments : none
 * Return    : E_OK     = PHY setup successful
 *             E_TMOUT  = PHY reset command error (timeout)
 *
 * Comments  : only auto-negotiation mode
 ************************************************************************/

static ER phy_ini(void)
{
    int i;

    phy_put(PHY_BMCR, BMCR_RESET);  /* Apply PHY reset command */
    i = PHY_RST_WAIT_TMO/10;
    for (;;) {
        dly_tsk(10);
        if (!(phy_get(PHY_BMCR) & BMCR_RESET)) {
            break;
        }
        if (i-- <= 0) {
            return E_TMOUT;     /* Error: PHY reset command error */
        }
    }

    phy_get(PHY_BMSR);  /* Dummy status read */
    t_lan.bit |= BIT_LINK_DOWN;     /* PHY link down */
    t_lan.physts = PHY_STS_LINK_DOWN;

#if (ETH_PHY_MODE == 1)
    /* 10M Half Duplex manual mode */
    phy_put(PHY_BMCR, 0);
#elif (ETH_PHY_MODE == 2)
    /* 10M Full/Half (Duplex auto select mode) */
    phy_put(PHY_BMCR, BMCR_ANE);
    phy_put(PHY_ANAR, ANAR_10_FD | ANAR_10 | ANAR_SF_802_3u);
    phy_put(PHY_BMCR, BMCR_ANE | BMCR_RS_ANP);
#elif (ETH_PHY_MODE == 3)
    /* 100M Half Duplex manual mode */
    phy_put(PHY_BMCR, BMCR_SPD);
#elif (ETH_PHY_MODE == 4)
    /* 100M Full/Half (Duplex auto select mode) */
    phy_put(PHY_BMCR, BMCR_ANE);
    phy_put(PHY_ANAR, ANAR_TX_FD | ANAR_TX | ANAR_SF_802_3u);
    phy_put(PHY_BMCR, BMCR_ANE | BMCR_RS_ANP);
#else
    /* Auto select mode (10HD/10FD/100HD/100FD) */
    phy_put(PHY_BMCR, BMCR_ANE);
    phy_put(PHY_ANAR, ANAR_TX_FD | ANAR_TX | ANAR_10_FD | ANAR_10 | ANAR_SF_802_3u);
    phy_put(PHY_BMCR, BMCR_ANE | BMCR_RS_ANP);
#endif
    return E_OK;
}

/*************************************************************************
 * Stopped controller
 ************************************************************************/

static void eth_stop(void)
{
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;
    volatile struct st_edmac __evenaccess *edmac = t_lan.edmac_p;

    edmac->EESIPR.LONG = DEF_INT;
    IOSYNC(edmac->EESIPR.LONG);
    edmac->EDRRR.BIT.RR = 0;        /* stop Rx */
    while (edmac->EDTRR.BIT.TR)     /* wait Tx */
        dly_tsk(1);
    ether->ECMR.LONG &= ~(ETHER_ECMR_TE | ETHER_ECMR_RE);   /* disable Rx/Tx */
    dly_tsk(1); /* wait for 1-frame time period */
}

/*************************************************************************
 * Restarted controller for changed duplex
 ************************************************************************/

static void eth_restart(UH physts)
{
    int i;
    T_ETH_BD *bd_p;
    char *rbuf_p, *tbuf_p;
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;
    volatile struct st_edmac __evenaccess *edmac = t_lan.edmac_p;
    UB *mac = t_lan.mac;

    t_lan.tbd_p = t_lan.tbd_top;
    tbuf_p = (char *)(t_lan.tbd_top->buf_p);
    rbuf_p = (char *)(t_lan.rbd_top->buf_p);

    bd_p = t_lan.tbd_top;
    for (i = 0; i < TXBUF_CNT; i++, bd_p++) {
        bd_p->buf_p = tbuf_p;
        tbuf_p += TBUFSZ;
        bd_p->fs = 0;
        bd_p->sz = 0;
        bd_p->fl = 0;
        bd_p->next = bd_p + 1;
    }
    bd_p--;
    bd_p->fs |= TX_BD_DLE;
    bd_p->next = t_lan.tbd_top;

    bd_p = t_lan.rbd_top;
    for (i = 0; i < RXBUF_CNT; i++, bd_p++) {
        bd_p->buf_p = rbuf_p;
        rbuf_p += RBUFSZ;
        bd_p->fs = RX_BD_ACT;
        bd_p->sz = RBUFSZ;
        bd_p->fl = 0;
        bd_p->next = bd_p + 1;
    }
    bd_p--;
    bd_p->fs |= RX_BD_DLE;
    bd_p->next = t_lan.rbd_top;

    edmac->EDMR.BIT.SWR = 1;    /* Reset */
    dly_tsk(1);                 /* wait 64 bus clock for reset sequence progress */

    edmac->RDLAR = (void *)t_lan.rbd_top;
    edmac->TDLAR = (void *)t_lan.tbd_top;

  #ifdef _UC3_ENDIAN_LITTLE
    edmac->EDMR.LONG = EDMAC_EDMR_DE | EDMAC_EDMR_DL16;
  #else
    edmac->EDMR.LONG = EDMAC_EDMR_DL16:
  #endif
    edmac->EESR.LONG   = 0xffffffff;    /* clear all of event */
    edmac->EESIPR.LONG = DEF_INT;
    edmac->TRSCER.LONG = 0;             /* copied all of status bits */
    edmac->TFTR.LONG   = 0;             /* store & foward */
    edmac->RMCR.LONG   = EDMAC_RMCR_RNR;/* multiple recieve mode */
#if 0
    edmac->TRIMD.LONG  = 0;
#endif

    ether->MAHR = ((UW)mac[0] << 24) | ((UW)mac[1] << 16) |
                  ((UW)mac[2] <<  8) | ((UW)mac[3]      );
    ether->MALR.LONG = ((UW)mac[4] <<  8) | ((UW)mac[5]);

    ether->RFLR.LONG = ETH_MAXLEN + 4;      /* 1514 + FCS */

    if (physts == PHY_STS_LINK_DOWN){       /* if cold-start */
        ether->ECMR.LONG = ETHER_ECMR_RTM | ETHER_ECMR_DM;  /* default is full-duplex */
        return;
    }

    if (physts == PHY_STS_100FD) {
        ether->ECMR.BIT.DM = 1;      /* full duplex */
        ether->ECMR.BIT.RTM = 1;     /* 100M */
    } else {
        ether->ECMR.BIT.DM = 0; /* half duplex */
    }
    if (physts == PHY_STS_100FD || physts == PHY_STS_10FD) {
        ether->ECMR.BIT.DM = 1; /* full duplex */
    } else {
        ether->ECMR.BIT.DM = 0; /* half duplex */
    }

#if ETH_FILTER_MODE == 1
    ether->ECMR.BIT.PRM = 1;
#endif
    ether->ECMR.LONG |= (ETHER_ECMR_TE | ETHER_ECMR_RE);    /* enable Rx/Tx */
    edmac->EDRRR.BIT.RR = 1;        /* restart Rx */

    edmac->EESR.LONG = 0xffffffff;  /* clear event */
    edmac->EESIPR.LONG = DEF_INT;
}

/*************************************************************************
 * Function  : phy_sts_hdr
 * Purpose   : Performs necessary setup after PHY status has changed
 * Arguments : none
 * Return    : none
 * Comments  : none
 ************************************************************************/

static void phy_sts_hdr(void)
{
    UH bmsr, physts;
    T_NET_DEV *dev = &gNET_DEV[eth_devnum-1];

    phy_get(PHY_BMSR);    /* released latched status */
    bmsr = phy_get(PHY_BMSR);
    if (!(bmsr & BMSR_LINK_STAT)) {
        if (t_lan.physts != PHY_STS_LINK_DOWN) {
            eth_stop();
            t_lan.physts = PHY_STS_LINK_DOWN;
            /* inform this event by COMMAND event interface */
            if (dev->cbk) {
                dev->cbk(eth_devnum, EV_DEV_LINK_CHG, PHY_STS_LINK_DOWN);
            }
        }
        t_lan.bit |= BIT_LINK_DOWN;
        return;
    }

#if (ETH_PHY_MODE == 1)
    physts = PHY_STS_10HD;
  
#elif (ETH_PHY_MODE == 3)
    physts = PHY_STS_100HD;
  
#else
    /* wait until autonegotiation is done its part */
    while (!((bmsr = phy_get(PHY_BMSR)) & BMSR_ANEG_COMP)) {
        if (!(t_lan.bit & BIT_INIT_FLAG)) {
            ext_tsk();
        }
        dly_tsk(1);
    }
    bmsr = phy_get(PHY_ANLPAR);
    bmsr &= phy_get(PHY_ANAR);
    if (bmsr & ANLPAR_B_TX_FD)
        physts = PHY_STS_100FD;
    else if (bmsr & ANLPAR_B_TX)
        physts = PHY_STS_100HD;
    else if (bmsr & ANLPAR_B_10_FD)
        physts = PHY_STS_10FD;
    else
        physts = PHY_STS_10HD;
#endif
    if (t_lan.physts != physts) {
        if (t_lan.physts != PHY_STS_LINK_DOWN)
            eth_stop();  /* Oops!! but go stop-sequence if it is happen */
        eth_restart(physts);  /* restart */

        t_lan.physts = physts;
        t_lan.bit &= ~BIT_LINK_DOWN;
        if (t_lan.bit & BIT_TX_BUSY) {
            set_flg(ID_ETH_FLG, EVENT_LINKUP);
        }
        
        /* invoke device callback */
        if (dev->cbk) {
            dev->cbk(eth_devnum, EV_DEV_LINK_CHG, (VP)t_lan.physts);
        }
    }
}

/*************************************************************************
 * Function  : _ddr_rx_eth_intr
 * Purpose   : Handles interrupt events
 * Arguments : none
 * Return    : none
 * Comments  : none
 ************************************************************************/

void _ddr_rx_eth_intr(void)
{
    UW ipr, sr;
    volatile struct st_etherc __evenaccess *ether = t_lan.ether_p;
    volatile struct st_edmac __evenaccess *edmac = t_lan.edmac_p;

    sr = edmac->EESR.LONG;
    ipr = edmac->EESIPR.LONG;
    sr &= ipr;
    edmac->EESR.LONG = sr;      /* clear event */


    /* FR (Rx Complete) */

    if (sr & DEF_RXINT){
        iset_flg(ID_ETH_FLG, EVENT_RX);
        ipr &= ~DEF_RXINT;
    }

    /* TC (Tx Complete) */

    if (sr & DEF_TXINT){
        iset_flg(ID_ETH_FLG, EVENT_TX);
        ipr &= ~DEF_TXINT;
    }

    edmac->EESIPR.LONG = ipr;
}

/*************************************************************************
 * Task      : et_ctl_tsk
 * Purpose   : Controls asynchronous reception
 * Arguments : none
 * Return    : none
 *
 * Comments  : none
 ************************************************************************/

void dev_ctl_tsk(VP_INT exinf)
{
    FLGPTN event;
    UH len;
    UB *buf_p;
    ER ercd;
    T_NET_DEV *dev;
    T_NET_BUF *pkt;
    T_ETH_BD *rbd_p, *sbd_p, *ebd_p;
    volatile struct st_edmac __evenaccess *edmac = t_lan.edmac_p;

    dev = &gNET_DEV[eth_devnum-1];

    rbd_p = t_lan.rbd_top;

    for (;;) {
        if (!(t_lan.bit & BIT_INIT_FLAG)) {
            break;
        }

        sbd_p = rbd_p;

#if DEBUG_ETH == 1
        t_lan.rbd_p = rbd_p;
#endif

        for (len = 0;;){

            /* check link */
            if (t_lan.bit & BIT_LINK_DOWN) {
                while (1) {
                    phy_sts_hdr();
                    if (!(t_lan.bit & BIT_LINK_DOWN)) {
                        sbd_p = rbd_p = t_lan.rbd_top;
                        break;
                    }
                    tslp_tsk(PHY_POLL_TMOUT);
                }
            }

            /* check data */
            if (rbd_p->fs & RX_BD_ACT) {    /* no rx-data */
                loc_cpu();
                edmac->EESIPR.LONG |= DEF_RXINT;
                unl_cpu();

                ercd = twai_flg(ID_ETH_FLG, EVENT_RX, TWF_ORW, &event, PHY_POLL_TMOUT);

                loc_cpu();
                edmac->EESIPR.LONG &= ~DEF_RXINT;
                IOSYNC(edmac->EESIPR.LONG);
                unl_cpu();

                if (ercd == E_OK)
                    clr_flg(ID_ETH_FLG, ~EVENT_RX);
                else
                if (ercd == E_TMOUT) {
                    phy_sts_hdr();
                }
                continue;
            }

            ebd_p = rbd_p;
            rbd_p = rbd_p->next;

            if (ebd_p->fs & RX_BD_FP_L)     /* that is last frame */
                break;
            len += ebd_p->sz;
            ebd_p->fl = ebd_p->sz;
        }

        if (ebd_p->fs & BD_RERRS) {         /* discard if Rx error */
            t_lan.rxpkt_errcnt++;
            goto _skip;
        }
#if ETH_FILTER_MODE == 0
        if (ebd_p->fs & RX_BD_RMAF) {
            goto _skip;
        }
#endif

        ebd_p->fl -= len;       /* re-size of last bd buffer */
        len += ebd_p->fl;

        /* Allocate buffer */
        ercd = net_buf_get(&pkt, len, TMO_POL);
        if (ercd != E_OK) {
            /* Stack memory not available. Drop this packet */
            goto _skip;
        }

        t_lan.rxpkt_cnt++;
        t_lan.rxbyte_cnt += len;
        buf_p = (UB *)pkt->buf + 2;  /* Leave 2 bytes */

        do {
            memcpy(buf_p, (void *)sbd_p->buf_p, sbd_p->fl);
            buf_p += sbd_p->fl;
            sbd_p->fs = (sbd_p->fs & RX_BD_DLE) | RX_BD_ACT;
            sbd_p = sbd_p->next;
        } while (sbd_p != rbd_p);

        if (!edmac->EDRRR.BIT.RR)
            edmac->EDRRR.BIT.RR = 1;    /* restart */

        /* send to upper layer */
        /* Transfer the received packet to TCP/IP */
        /* In received packet content,            */ 
        /* pkt->hdr should point Ethernet header  */
        /* pkt->dat should point IP               */
        pkt->hdr     = pkt->buf + 2;
        pkt->hdr_len = ETH_HDR_SZ;
        pkt->dat     = pkt->hdr + pkt->hdr_len;
        pkt->dat_len = len - pkt->hdr_len;
        pkt->dev = dev;
        snd_mbx(ID_ETH_RCV_MBX, (T_MSG *)pkt);
        rot_rdq(TPRI_SELF);
        continue;

_skip:
        do {
            sbd_p->fs = (sbd_p->fs & RX_BD_DLE) | RX_BD_ACT;
            sbd_p = sbd_p->next;
        } while (sbd_p != rbd_p);

        if (!edmac->EDRRR.BIT.RR)
            edmac->EDRRR.BIT.RR = 1;    /* restart */
    }

    ext_tsk();
}

/*************************************************************************
 * Transmit frame
 ************************************************************************/

ER eth_tx(T_NET_BUF *pkt)
{
    ER ercd;
    UH len, txlen, pad;
    UB *buf;
    FLGPTN event;
    T_ETH_BD *tbd_p, *sbd_p, *ebd_p;
    volatile struct st_edmac __evenaccess *edmac = t_lan.edmac_p;

    if (!(t_lan.bit & BIT_INIT_FLAG)) {
        return E_OBJ;       /* Error: Device Not initialized */
    }

    txlen = len = pkt->hdr_len;
    buf = pkt->hdr;

    if (len == 0 || len > ETH_MAXLEN) {
        return E_PAR;
    }

    t_lan.bit |= BIT_TX_BUSY;
    while (t_lan.bit & BIT_LINK_DOWN) {
        /* Wait until link is back */
        ercd = twai_flg(ID_ETH_FLG, EVENT_LINKUP, TWF_ORW, &event, ETH_TX_TMOUT);
        if (ercd != E_OK) {
            t_lan.bit &= ~BIT_TX_BUSY;
            return ercd;
        }
        clr_flg(ID_ETH_FLG, ~EVENT_LINKUP);
    }

    sbd_p = tbd_p = t_lan.tbd_p;

    if (len < 60) {
        pad = 60 - len;
        len = 60;
    } else {
        pad = 0;
    }

    while (len != 0) {
        if (edmac->EESR.LONG & DEF_TXINT)
            edmac->EESR.LONG = DEF_TXINT; /* clear event */
        if (tbd_p->fs & TX_BD_ACT) {      /* nothing buffer */
            loc_cpu();
            edmac->EESIPR.LONG |= DEF_TXINT;
            unl_cpu();

            ercd = twai_flg(ID_ETH_FLG, EVENT_TX|EVENT_LINKUP, TWF_ORW, &event, TMO_FEVR);

            loc_cpu();
            edmac->EESIPR.LONG &= ~DEF_TXINT;   /* disable Tx INT */
            IOSYNC(edmac->EESIPR.LONG);
            unl_cpu();

            if (ercd == E_OK)
                clr_flg(ID_ETH_FLG, ~(event & (EVENT_TX|EVENT_LINKUP)));

            if (event & EVENT_LINKUP) {
                t_lan.bit &= ~BIT_TX_BUSY;
                return E_TMOUT;         /* can not recovery */
            }
            continue;
        }

        tbd_p->fs &= TX_BD_DLE; /* clear flag */

        if (len <= TBUFSZ) {
            tbd_p->sz = len;
            memcpy(tbd_p->buf_p, buf, tbd_p->sz - pad);
            if (pad != 0)
                memset((char *)tbd_p->buf_p + (len - pad), 0, pad);
        }
        else {
            tbd_p->sz = TBUFSZ;
            memcpy((void *)tbd_p->buf_p, buf, tbd_p->sz);
        }

        len -= tbd_p->sz;
        buf += tbd_p->sz;

        ebd_p = tbd_p;
        tbd_p = tbd_p->next;
    }

    sbd_p->fs |= TX_BD_FP_F;    /* set 1st buffer status */
    ebd_p->fs |= TX_BD_FP_L;    /* last buffer in frame */

    loc_cpu();
    do {
        sbd_p->fs |= TX_BD_ACT;
        sbd_p = sbd_p->next;

    } while (sbd_p != tbd_p);
    unl_cpu();

    if (!edmac->EDTRR.BIT.TR)
        edmac->EDTRR.BIT.TR = 1;    /* restart */

    t_lan.tbd_p = tbd_p;
    t_lan.txpkt_cnt++;    /* Update Tx packet counter */
    t_lan.txbyte_cnt += txlen;
    t_lan.bit &= ~BIT_TX_BUSY;

    return E_OK;
}

/*************************************************************************
 * Function  : eth_ini
 * Purpose   : Initializes the hardware device.
 * Arguments : dev_num = Network interface control block id.
 * Return    : E_OK    = Initialization successful
 *             E_OBJ   = Device already initialized.
 *             E_PAR   = MAC Address error.
 *             E_CLS   = Initialization process cancelled.
 *             E_ID    = Kernel resource creation error.
 *                       dev_num is invalid.
 * Comments  : Network interface provides the mac address of the interface.
 *             call from uNET3 by gNET_DEV[].ini
 ************************************************************************/

ER eth_ini(UH dev_num)
{
    ER rtncd;
    UB *mac;
    T_NET *net;
    T_NET_DEV *dev;
    struct _eth_buffer *buf_p;
    struct _eth_bd *bd_p;

    if (dev_num == 0) {
        return E_ID;
    }
    eth_devnum = dev_num;

    net = &gNET[eth_devnum-1];
    if (net == NULL) {
        return E_PAR;
    }
    dev = net->dev;
    if (dev == NULL) {
        return E_PAR;
    }
    mac = &dev->cfg.eth.mac[0];

    /* Task for reception control */
#ifndef NET_C_OS
    ID_ETH_CTL_TSK = acre_tsk((T_CTSK *)&c_eth_ctl_tsk);
    if (ID_ETH_CTL_TSK <= 0)
        return E_ID;     /* Error: Ctl Task creation failed */

    ID_ETH_SND_TSK = acre_tsk((T_CTSK *)&c_eth_snd_tsk);
    if (ID_ETH_SND_TSK <= 0)
        return E_ID;     /* Error: Rx Task creation failed */

    ID_ETH_RCV_TSK = acre_tsk((T_CTSK *)&c_eth_rcv_tsk);
    if (ID_ETH_RCV_TSK <= 0)
        return E_ID;     /* Error: Rx Task creation failed */
#endif

    /* Send and Receive Queue mailbox */
#ifndef NET_C_OS
    /* Create Send MailBox */
    ID_ETH_SND_MBX = acre_mbx((T_CMBX *)&c_eth_snd_mbx);
    if (ID_ETH_SND_MBX <= 0)
        return E_ID;     /* Error: Send MBX creation failed */

    /* Create Receive MailBox */
    ID_ETH_RCV_MBX = acre_mbx((T_CMBX *)&c_eth_rcv_mbx);
    if (ID_ETH_RCV_MBX <= 0)
        return E_ID;     /* Error: Recv MBX creation failed */
#endif

    /* create event flag for sync with interrupt operation */
#ifndef NET_C_OS
    ID_ETH_FLG = acre_flg((T_CFLG *)&c_eth_flg);
    if (ID_ETH_FLG <= 0)
        return E_ID;     /* Error: Event flag creation failed */
#endif

#ifndef NET_C_OS
    ID_ETH_ISR = acre_isr((T_CISR *)&_ddr_rx62n_eth_cisr);
    if (ID_ETH_ISR <= E_OK) {
        return E_ID;
    }
#endif

    if (t_lan.bit & BIT_INIT_FLAG) {
        return E_OBJ;       /* Error: Already initialized */
    }
    
    memset((void *)&t_lan, 0, sizeof(t_lan));   /* Clear control/status structure */
    t_lan.ether_p = &ETHERC;
    t_lan.edmac_p = &EDMAC;

    loc_cpu();
    #if (RX_SRS==630)       /* RX63N */
    SYSTEM.PRCR.WORD = 0xa502;
    #endif
    MSTP_EDMAC = 0;
    #if (RX_SRS==630)       /* RX63N */
    SYSTEM.PRCR.WORD = 0xa500;
    #endif
    unl_cpu();

    t_lan.dly_cnt = (ICLK / 5) / 2500000;       /* (ICLK/5) / 2.5MHz */

    buf_p = (struct _eth_buffer *)ALIGN(&eth_buffer, 32);
    bd_p  = (struct _eth_bd *)ALIGN(&eth_bd, 16);

    t_lan.rbd_top = &bd_p->rxbd[0];
    t_lan.tbd_p = t_lan.tbd_top = &bd_p->txbd[0];
    bd_p->rxbd[0].buf_p = (char *)&buf_p->rxbuf[0];
    bd_p->txbd[0].buf_p = (char *)&buf_p->txbuf[0];

    /*
     * If the user specified MAC address is valid, then over-write the user MAC
     * address to MAC Address register. Else, if user specified address is either
     * 00-00-00-00-00-00 or FF-FF-FF-FF-FF-FF, then return error.
     */
    if (((mac[0]|mac[1]|mac[2]|mac[3]|mac[4]|mac[5]) == 0x00) ||
        ((mac[0]&mac[1]&mac[2]&mac[3]&mac[4]&mac[5]) == 0xFF)) {

        return E_PAR;
    }
    t_lan.mac = mac;

    /* setup the PHY controller */

    rtncd = phy_ini();
    if (rtncd != E_OK) {
        return rtncd;   /* Error: PHY initialization failed */
    }

    loc_cpu();
    ETHER_IPR = IPL_ETHERNET;
    ETHER_IEN = 1;
    unl_cpu();

    t_lan.bit |= (BIT_INIT_FLAG | BIT_RX_ON | BIT_TX_ON);
    sta_tsk(ID_ETH_CTL_TSK, 0);
    sta_tsk(ID_ETH_SND_TSK, 0);
    sta_tsk(ID_ETH_RCV_TSK, 0);

    return E_OK;
}

/*************************************************************************
 * Function  : eth_cls
 * Purpose   : UnInitializes the device and its resources.
 * Arguments : none
 * Return    : E_OK    = UnInitialization successful
 * comment   : call from uNET3 by gNET_DEV[].cls
 ************************************************************************/

ER eth_cls(UH dev_num)
{
    T_NET_BUF *pkt;

    loc_cpu();
    ETHER_IEN = 0;
    IOSYNC(ETHER_IEN);
    ETHER_IPR = 0;
    unl_cpu();

    eth_stop(); /* stop sequence issue */

#ifndef NET_C_OS
    dis_dsp();
    ter_tsk(ID_ETH_CTL_TSK);
    del_tsk(ID_ETH_CTL_TSK);
    ter_tsk(ID_ETH_SND_TSK);
    del_tsk(ID_ETH_SND_TSK);
    ter_tsk(ID_ETH_RCV_TSK);
    del_tsk(ID_ETH_RCV_TSK);
    del_mbx(ID_ETH_SND_MBX);
    del_mbx(ID_ETH_RCV_MBX);
    del_flg(ID_ETH_FLG);
    del_isr(ID_ETH_ISR);
    ena_dsp();
#else
    dis_dsp();
    ter_tsk(ID_ETH_CTL_TSK);
    ter_tsk(ID_ETH_SND_TSK);
    ter_tsk(ID_ETH_RCV_TSK);
    while (prcv_mbx(ID_ETH_SND_MBX, (T_MSG **)&pkt) == E_OK)
        net_buf_ret(pkt);
    while (prcv_mbx(ID_ETH_RCV_MBX, (T_MSG **)&pkt) == E_OK)
        net_buf_ret(pkt);
    clr_flg(ID_ETH_FLG, 0);
    ena_dsp();
#endif

    loc_cpu();
    MSTP_EDMAC = 1;
    unl_cpu();

    t_lan.bit &= ~(BIT_INIT_FLAG | BIT_RX_ON | BIT_TX_ON);
    return E_OK;
}

/*************************************************************************
 * Function  : eth_snd
 * Purpose   : 
 * Arguments : 
 * Return    : E_WBLK
 * comment   : call from uNET3 by gNET_DEV[].out
 ************************************************************************/

ER eth_snd(UH dev_num, T_NET_BUF *pkt)
{
    /* Add to Device send queue */
    snd_mbx(ID_ETH_SND_MBX, (T_MSG *)pkt);
    return E_WBLK;
}

/*************************************************************************
 * Function  : eth_cfg
 * Purpose   : 
 * Arguments : 
 * Return    : 
 * comment   : call from uNET3 by gNET_DEV[].ctl
 ************************************************************************/

ER eth_cfg(UH dev_num, UH opt, VP val)
{
    ER rtncd = E_OK;

    if (!(t_lan.bit & BIT_INIT_FLAG)) {
        return E_OBJ;       /* Error: Device Not initialized */
    }

    switch (opt) {
        case 0:
        default:
            rtncd = E_NOSPT;
            break;
    }

    return rtncd;
}

/*************************************************************************
 * Function  : eth_ref
 * Purpose   : 
 * Arguments : 
 * Return    : 
 * comment   : call from uNET3 by gNET_DEV[].ref
 ************************************************************************/

ER eth_ref(UH dev_num, UH opt, VP val)
{
    ER rtncd = E_OK;
    struct t_lan_stat *stat;

    if (!(t_lan.bit & BIT_INIT_FLAG)) {
        return E_OBJ;       /* Error: Device Not initialized */
    }

    switch (opt) {
        case GET_STATISTICS :
            stat = (struct t_lan_stat *)val;
            loc_cpu();
            stat->txpkt_cnt = t_lan.txpkt_cnt;
            stat->txbyte_cnt = t_lan.txbyte_cnt;
            stat->rxpkt_cnt = t_lan.rxpkt_cnt;
            stat->rxpkt_errcnt = t_lan.rxpkt_errcnt;
            stat->rxbyte_cnt = t_lan.rxbyte_cnt;
            stat->physts = (UINT)t_lan.physts;
            unl_cpu();
            break;

        case GET_LINK_STS :
            *(UB *)val = t_lan.physts;
            break;

        default:
            rtncd = E_NOSPT;
            break;
    }

    return rtncd;
}

/*************************************************************************
 * Receive Task
 ************************************************************************/

void dev_rcv_tsk(VP_INT exinf)
{
    T_NET_BUF *pkt;
    ER ercd;

    for (;;) {

        ercd = rcv_mbx(ID_ETH_RCV_MBX, (T_MSG **)&pkt);
        if (ercd != E_OK) {
            break;
        }

        net_pkt_rcv(pkt);
    }
}

/*************************************************************************
 * Send Task
 ************************************************************************/
/*
    use task to
     ** queue the packet if dev out is need to be blocked
     ** if transmission process requires huge task stack size.
*/

void dev_snd_tsk(VP_INT exinf)
{
    T_NET_BUF *pkt;
    ER ercd;

    for (;;) {

        ercd = rcv_mbx(ID_ETH_SND_MBX, (T_MSG **)&pkt);
        if (ercd != E_OK) {
            break;
        }

        ercd = eth_tx(pkt);
        if (ercd != E_OK) {
            pkt->ercd = ercd;
        }

        loc_tcp();
        net_buf_ret(pkt);
        ulc_tcp();
    }
}

