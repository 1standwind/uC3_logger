/************************************************************************
    MICRO C CUBE / COMPACT, DEVICE DRIVER header
    Ethernet driver for RX6xx

    Copyright (c)  2011, eForce Co.,Ltd.  All rights reserved.

    Version Information
            2011.11.08: Created.
 ************************************************************************/

#ifndef _DDR_RX_ETH_H_
#define _DDR_RX_ETH_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 *--- E-DMAC controller bit field definition ---
 */

/*
 * EDMR
 */

#define EDMAC_EDMR_DE       0x00000040UL
#define EDMAC_EDMR_DL16     0x00000000UL
#define EDMAC_EDMR_DL32     0x00000010UL
#define EDMAC_EDMR_DL64     0x00000020UL
#define EDMAC_EDMR_SWR      0x00000001UL

/*
 * EESR
 * EESIPR
 */

#define EDMAC_EESR_TWB      0x40000000UL
#define EDMAC_EESR_TABT     0x04000000UL
#define EDMAC_EESR_RABT     0x02000000UL
#define EDMAC_EESR_RFCOF    0x01000000UL
#define EDMAC_EESR_ADE      0x00800000UL
#define EDMAC_EESR_ECI      0x00400000UL
#define EDMAC_EESR_TC       0x00200000UL
#define EDMAC_EESR_TDE      0x00100000UL
#define EDMAC_EESR_TFUF     0x00080000UL
#define EDMAC_EESR_FR       0x00040000UL
#define EDMAC_EESR_RDE      0x00020000UL
#define EDMAC_EESR_RFOF     0x00010000UL
#define EDMAC_EESR_CND      0x00000800UL
#define EDMAC_EESR_DLC      0x00000400UL
#define EDMAC_EESR_CD       0x00000200UL
#define EDMAC_EESR_TRO      0x00000100UL
#define EDMAC_EESR_RMAF     0x00000080UL
#define EDMAC_EESR_RRF      0x00000010UL
#define EDMAC_EESR_RTLF     0x00000008UL
#define EDMAC_EESR_RTSF     0x00000004UL
#define EDMAC_EESR_PRE      0x00000002UL
#define EDMAC_EESR_CERF     0x00000001UL

#define EDMAC_EESIPR_TWBIP      EDMAC_EESR_TWB
#define EDMAC_EESIPR_TABTIP     EDMAC_EESR_TABT
#define EDMAC_EESIPR_RABTIP     EDMAC_EESR_RABT
#define EDMAC_EESIPR_RFCOFIP    EDMAC_EESR_RFCOF
#define EDMAC_EESIPR_ADEIP      EDMAC_EESR_ADE
#define EDMAC_EESIPR_ECIP       EDMAC_EESR_ECI
#define EDMAC_EESIPR_TCIP       EDMAC_EESR_TC
#define EDMAC_EESIPR_TDEIP      EDMAC_EESR_TDE
#define EDMAC_EESIPR_TFUFIP     EDMAC_EESR_TFUF
#define EDMAC_EESIPR_FRIP       EDMAC_EESR_FR
#define EDMAC_EESIPR_RDEIP      EDMAC_EESR_RDE
#define EDMAC_EESIPR_RFOFIP     EDMAC_EESR_RFOF
#define EDMAC_EESIPR_CNDIP      EDMAC_EESR_CND
#define EDMAC_EESIPR_DLCIP      EDMAC_EESR_DLC
#define EDMAC_EESIPR_CDIP       EDMAC_EESR_CD
#define EDMAC_EESIPR_TROIP      EDMAC_EESR_TRO
#define EDMAC_EESIPR_RMAFIP     EDMAC_EESR_RMAF
#define EDMAC_EESIPR_RRFIP      EDMAC_EESR_RRF
#define EDMAC_EESIPR_RTLFIP     EDMAC_EESR_RTLF
#define EDMAC_EESIPR_RTSFIP     EDMAC_EESR_RTSF
#define EDMAC_EESIPR_PREIP      EDMAC_EESR_PRE
#define EDMAC_EESIPR_CERFIP     EDMAC_EESR_CERF

/*
 * TRSCER
 */

#define EDMAC_TRSCER_CNDCE  0x00000800UL
#define EDMAC_TRSCER_DLCCE  0x00000400UL
#define EDMAC_TRSCER_CDCE   0x00000200UL
#define EDMAC_TRSCER_TROCE  0x00000100UL
#define EDMAC_TRSCER_RMAFCE 0x00000080UL
#define EDMAC_TRSCER_RRFCE  0x00000010UL
#define EDMAC_TRSCER_RTLFCE 0x00000008UL
#define EDMAC_TRSCER_RTSFCE 0x00000004UL
#define EDMAC_TRSCER_PRECE  0x00000002UL
#define EDMAC_TRSCER_CERFCE 0x00000001UL

/*
 * FDR
 */

#define EDMAC_FDR_TFD_SHIFT 8
#define EDMAC_FDR_RFD_SHIFT 0

/*
 * RMCR
 */

#define EDMAC_RMCR_RNC      0x00000002UL
#define EDMAC_RMCR_RNR      0x00000001UL

/*
 * TRIMD
 */

#define EDMAC_TRIMD_TIM     0x00000010UL
#define EDMAC_TRIMD_TIS     0x00000001UL


/*
 *--- EtherC controller bit field definition ---
 */

/*
 * ECMR
 */

#define ETHER_ECMR_TPC      0x00100000UL
#define ETHER_ECMR_ZPF      0x00080000UL
#define ETHER_ECMR_PFR      0x00040000UL
#define ETHER_ECMR_RXF      0x00020000UL
#define ETHER_ECMR_TXF      0x00010000UL
#define ETHER_ECMR_PRCEF    0x00001000UL
#define ETHER_ECMR_MPDE     0x00000200UL
#define ETHER_ECMR_RE       0x00000040UL
#define ETHER_ECMR_TE       0x00000020UL
#define ETHER_ECMR_ILB      0x00000008UL
#define ETHER_ECMR_RTM      0x00000004UL
#define ETHER_ECMR_DM       0x00000002UL
#define ETHER_ECMR_PRM      0x00000001UL

/*
 * ECSR
 * ECSIPR
 */

#define ETHER_ECSR_BFR      0x00000020UL
#define ETHER_ECSR_PSRTO    0x00000010UL
#define ETHER_ECSR_LCHNG    0x00000004UL
#define ETHER_ECSR_MPD      0x00000002UL
#define ETHER_ECSR_ICD      0x00000001UL

#define ETHER_ECSIPR_BFR    ETHER_ECSR_BFR
#define ETHER_ECSIPR_PSRTO  ETHER_ECSR_PSRTO
#define ETHER_ECSIPR_LCHNG  ETHER_ECSR_LCHNG
#define ETHER_ECSIPR_MPD    ETHER_ECSR_MPD
#define ETHER_ECSIPR_ICD    ETHER_ECSR_ICD

/*
 * PIR
 */

#define ETHER_PIR_MDI       0x00000008UL
#define ETHER_PIR_MDO       0x00000004UL
#define ETHER_PIR_MMD       0x00000002UL
#define ETHER_PIR_MDC       0x00000001UL


/*
 * Receive descriptor bit field
 */

#define RX_BD_ACT       0x80000000UL    /* Active */
#define RX_BD_DLE       0x40000000UL    /* last descriptor */
#define RX_BD_FP_M      0x00000000UL    /* [FP00]middle buffer for multi buffer */
#define RX_BD_FP_L      0x10000000UL    /* [FP01]last buffer for multi buffer */
#define RX_BD_FP_F      0x20000000UL    /* [FP10]1st  buffer for multi buffer */
#define RX_BD_FP_ONE    0x30000000UL    /* [FP11]1farme per buffer */
#define RX_BD_FE        0x08000000UL    /* frame error */

#define RX_BD_RFOF      0x00000200UL    /* FIFO overflow */
#define RX_BD_ABORT     0x00000100UL    /* abort */
#define RX_BD_RMAF      0x00000080UL    /* multicast */
#define RX_BD_RRF       0x00000010UL    /* fractional bit */
#define RX_BD_RTLF      0x00000008UL    /* long frame */
#define RX_BD_RTSF      0x00000004UL    /* short frame */
#define RX_BD_PRE       0x00000002UL    /* PHY-LSI error */
#define RX_BD_CERF      0x00000001UL    /* CRC error */

/*
 * Transmit descriptor bit field
 */

#define TX_BD_ACT       0x80000000UL    /* Active */
#define TX_BD_DLE       0x40000000UL    /* last descriptor */
#define TX_BD_FP_M      0x00000000UL    /* [FP00]middle buffer for multi buffer */
#define TX_BD_FP_L      0x10000000UL    /* [FP01]last buffer for multi buffer */
#define TX_BD_FP_F      0x20000000UL    /* [FP10]1st  buffer for multi buffer */
#define TX_BD_FP_ONE    0x30000000UL    /* [FP11]1farme per buffer */
#define TX_BD_FE        0x08000000UL    /* frame error */
#define TX_BD_WBI       0x04000000UL    /* wtite-back interrupt */

#define TX_BD_ABORT     0x00000100UL    /* abort */
#define TX_BD_CND       0x00000008UL    /* no carrier */
#define TX_BD_DLC       0x00000004UL    /* lost carrier */
#define TX_BD_CD        0x00000002UL    /* collition */
#define TX_BD_TRO       0x00000001UL    /* retry over */


/* Buffer Descriptor structure */

typedef struct t_eth_bd {
    unsigned long fs;
  #ifdef _UC3_ENDIAN_LITTLE     /* here if little endian */
    unsigned short fl;          /* rx-frame length */
    unsigned short sz;          /* buffer size */
  #else
    unsigned short sz;
    unsigned short fl;
  #endif
    char *buf_p;
    struct t_eth_bd *next;      /* for user work (not used controller) */
} T_ETH_BD;


/*
 * MII control bits definition
 */

#define MII_ST          0x4000  /* Start of frame delimiter(only 01) */
#define MII_OP_R        0x2000  /* Operation code: 10(read) */
#define MII_OP_W        0x1000  /* Operation code: 01(write) */
#define MII_PA_SHIFT    7       /* PHY address (shift bit) */
#define MII_RA_SHIFT    2       /* Register address (shift bit) */
#define MII_TA          0x0002  /* Turn around (only 10) */


/*************************************************************************
 * Command / Status macro definitions:
 ************************************************************************/

struct t_lan_stat {
    UINT    txpkt_cnt;      /* Transmit packet count                    */
    UINT    txbyte_cnt;     /* Transmit total data byte count           */
    UINT    rxpkt_cnt;      /* Received packet count                    */
    UINT    rxpkt_errcnt;   /* Received/dropped error packet count      */
    UINT    rxbyte_cnt;     /* Received total data byte count           */
    UINT    physts;         /* PHY media status                         */
};


#define GET_STATISTICS      0x02U    /* Read the Driver statistics      */
#define GET_LINK_STS        0x03U    /* Read the Link Status            */


/*************************************************************************
 * Notification Event Command / Status definitions:
 ************************************************************************/

/* Callback events */

#define EV_DEV_LINK_CHG     0x01U    /* Link status change event        */

/* Link Status */
#define PHY_STS_LINK_DOWN   0x00U    /* PHY media link down status      */
#define PHY_STS_10HD        0x10U    /* PHY media is 10M/Half-Duplex    */
#define PHY_STS_10FD        0x20U    /* PHY media is 10M/Full-Duplex    */
#define PHY_STS_100HD       0x30U    /* PHY media is 100M/Half-Duplex   */
#define PHY_STS_100FD       0x40U    /* PHY media is 100M/Full-Duplex   */


#ifdef __cplusplus
}
#endif
#endif /* _DDR_RX_ETH_H_ */
