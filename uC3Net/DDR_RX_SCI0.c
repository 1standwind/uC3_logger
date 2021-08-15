/***************************************************************************
    Micro C Cube Compact, DEVICE DRIVER
    Serial Interface for RX SCI

    Copyright (c)  2011-2013, eForce Co., Ltd. All rights reserved.

    Version Information
            2011.09.22: Created.
            2012.07.08: Bug fix. (cannot wake up when it doesn't request 
                                  status buffer, if RX error happen)
            2012.11.19: Correspond to use multi channel. 
            2013.02.22: Applicated for EWRX.
            2013.09.26: Applicated for RX62T series in _ddr_rx_sci0_init function.
                        Applicated for RX62N/621 series in _ddr_rx_sci0_init function.
                        Enable/Disable of Recieve interrupt was improved. 
 ***************************************************************************/

#include "kernel.h"

#include "DDR_COM.h"
#include "DDR_RX_SCI0_cfg.h"
#include "DDR_RX_SCI0.h"
#include "device_id.h"

#include <string.h>

#define IOSYNC(a)   (a)?1:0


#define _ERIE0              IEN(SCI0,ERI0)
#define _RXIE0              IEN(SCI0,RXI0)
#define _TXIE0              IEN(SCI0,TXI0)
#define _TEIE0              IEN(SCI0,TEI0)

#ifdef SCI_0

#if (TXBUF_SZ0==0)
#define _ddr_rx_sci0_tbuf0  0
#else
VB  _ddr_rx_sci0_tbuf0[TXBUF_SZ0];
#endif
#if (RXBUF_SZ0==0)
#define _ddr_rx_sci0_rbuf0  0
#define _ddr_rx_sci0_sbuf0  0
#else
VB  _ddr_rx_sci0_rbuf0[RXBUF_SZ0];
UB  _ddr_rx_sci0_sbuf0[RXBUF_SZ0];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data0;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata0 = {_ddr_rx_sci0_tbuf0, _ddr_rx_sci0_rbuf0,
            _ddr_rx_sci0_sbuf0, TXBUF_SZ0, RXBUF_SZ0, XOFF_SZ0, XON_SZ0, TXSEM0, RXSEM0};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi0 = {&PORT2.PORT.BYTE, 0x02, &PORT2.DR.BYTE, 0x01,
             IR_SCI0_TXI0, IER_SCI0_TXI0, 0x01, IR_SCI0_RXI0, IER_SCI0_RXI0, 0x80};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng0 = {(volatile struct st_sci *)&SCI0,
            &_ddr_rx_sci0_pfi0, &_ddr_rx_sci0_cdata0, &_ddr_rx_sci0_data0};
#endif


#define _ERIE1              IEN(SCI1,ERI1)
#define _RXIE1              IEN(SCI1,RXI1)
#define _TXIE1              IEN(SCI1,TXI1)
#define _TEIE1              IEN(SCI1,TEI1)

#ifdef SCI_1

#if (TXBUF_SZ1==0)
#define _ddr_rx_sci0_tbuf1  0
#else
VB  _ddr_rx_sci0_tbuf1[TXBUF_SZ1];
#endif
#if (RXBUF_SZ1==0)
#define _ddr_rx_sci0_rbuf1  0
#define _ddr_rx_sci0_sbuf1  0
#else
VB  _ddr_rx_sci0_rbuf1[RXBUF_SZ1];
UB  _ddr_rx_sci0_sbuf1[RXBUF_SZ1];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data1;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata1 = {_ddr_rx_sci0_tbuf1, _ddr_rx_sci0_rbuf1,
            _ddr_rx_sci0_sbuf1, TXBUF_SZ1, RXBUF_SZ1, XOFF_SZ1, XON_SZ1, TXSEM1, RXSEM1};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi1 = {&PORT3.PORT.BYTE, 0x01, &PORT2.DR.BYTE, 0x40,
             IR_SCI1_TXI1, IER_SCI1_TXI1, 0x10, IR_SCI1_RXI1, IER_SCI1_RXI1, 0x08};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng1 = {(volatile struct st_sci *)&SCI1,
            &_ddr_rx_sci0_pfi1, &_ddr_rx_sci0_cdata1, &_ddr_rx_sci0_data1};
#endif


#define _ERIE2              IEN(SCI2,ERI2)
#define _RXIE2              IEN(SCI2,RXI2)
#define _TXIE2              IEN(SCI2,TXI2)
#define _TEIE2              IEN(SCI2,TEI2)

#ifdef SCI_2

#if (TXBUF_SZ2==0)
#define _ddr_rx_sci0_tbuf2  0
#else
VB  _ddr_rx_sci0_tbuf2[TXBUF_SZ2];
#endif
#if (RXBUF_SZ2==0)
#define _ddr_rx_sci0_rbuf2  0
#define _ddr_rx_sci0_sbuf2  0
#else
VB  _ddr_rx_sci0_rbuf2[RXBUF_SZ2];
UB  _ddr_rx_sci0_sbuf2[RXBUF_SZ2];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data2;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata2 = {_ddr_rx_sci0_tbuf2, _ddr_rx_sci0_rbuf2,
            _ddr_rx_sci0_sbuf2, TXBUF_SZ2, RXBUF_SZ2, XOFF_SZ2, XON_SZ2, TXSEM2, RXSEM2};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi2 = {&PORT1.PORT.BYTE, 0x04, &PORT1.DR.BYTE, 0x08,
             IR_SCI2_TXI2, IER_SCI2_TXI2, 0x01, IR_SCI2_RXI2, IER_SCI2_RXI2, 0x80};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng2 = {(volatile struct st_sci *)&SCI2,
            &_ddr_rx_sci0_pfi2, &_ddr_rx_sci0_cdata2, &_ddr_rx_sci0_data2};
#endif


#define _ERIE3              IEN(SCI3,ERI3)
#define _RXIE3              IEN(SCI3,RXI3)
#define _TXIE3              IEN(SCI3,TXI3)
#define _TEIE3              IEN(SCI3,TEI3)

#ifdef SCI_3

#if (TXBUF_SZ3==0)
#define _ddr_rx_sci0_tbuf3  0
#else
VB  _ddr_rx_sci0_tbuf3[TXBUF_SZ3];
#endif
#if (RXBUF_SZ3==0)
#define _ddr_rx_sci0_rbuf3  0
#define _ddr_rx_sci0_sbuf3  0
#else
VB  _ddr_rx_sci0_rbuf3[RXBUF_SZ3];
UB  _ddr_rx_sci0_sbuf3[RXBUF_SZ3];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data3;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata3 = {_ddr_rx_sci0_tbuf3, _ddr_rx_sci0_rbuf3,
            _ddr_rx_sci0_sbuf3, TXBUF_SZ3, RXBUF_SZ3, XOFF_SZ3, XON_SZ3, TXSEM3, RXSEM3};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi3 = {&PORT2.PORT.BYTE, 0x20, &PORT2.DR.BYTE, 0x08,
             IR_SCI3_TXI3, IER_SCI3_TXI3, 0x10, IR_SCI3_RXI3, IER_SCI3_RXI3, 0x08};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng3 = {(volatile struct st_sci *)&SCI3,
            &_ddr_rx_sci0_pfi3, &_ddr_rx_sci0_cdata3, &_ddr_rx_sci0_data3};
#endif


#define _ERIE5              IEN(SCI5,ERI5)
#define _RXIE5              IEN(SCI5,RXI5)
#define _TXIE5              IEN(SCI5,TXI5)
#define _TEIE5              IEN(SCI5,TEI5)

#ifdef SCI_5

#if (TXBUF_SZ5==0)
#define _ddr_rx_sci0_tbuf5  0
#else
VB  _ddr_rx_sci0_tbuf5[TXBUF_SZ5];
#endif
#if (RXBUF_SZ5==0)
#define _ddr_rx_sci0_rbuf5  0
#define _ddr_rx_sci0_sbuf5  0
#else
VB  _ddr_rx_sci0_rbuf5[RXBUF_SZ5];
UB  _ddr_rx_sci0_sbuf5[RXBUF_SZ5];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data5;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata5 = {_ddr_rx_sci0_tbuf5, _ddr_rx_sci0_rbuf5,
            _ddr_rx_sci0_sbuf5, TXBUF_SZ5, RXBUF_SZ5, XOFF_SZ5, XON_SZ5, TXSEM5, RXSEM5};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi5 = {&PORTC.PORT.BYTE, 0x04, &PORTC.DR.BYTE, 0x08,
             IR_SCI5_TXI5, IER_SCI5_TXI5, 0x10, IR_SCI5_RXI5, IER_SCI5_RXI5, 0x08};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng5 = {(volatile struct st_sci *)&SCI5,
            &_ddr_rx_sci0_pfi5, &_ddr_rx_sci0_cdata5, &_ddr_rx_sci0_data5};
#endif


#define _ERIE6              IEN(SCI6,ERI6)
#define _RXIE6              IEN(SCI6,RXI6)
#define _TXIE6              IEN(SCI6,TXI6)
#define _TEIE6              IEN(SCI6,TEI6)

#ifdef SCI_6

#if (TXBUF_SZ6==0)
#define _ddr_rx_sci0_tbuf6  0
#else
VB  _ddr_rx_sci0_tbuf6[TXBUF_SZ6];
#endif
#if (RXBUF_SZ6==0)
#define _ddr_rx_sci0_rbuf6  0
#define _ddr_rx_sci0_sbuf6  0
#else
VB  _ddr_rx_sci0_rbuf6[RXBUF_SZ6];
UB  _ddr_rx_sci0_sbuf6[RXBUF_SZ6];
#endif

T_RX_SCI0_DMNG _ddr_rx_sci0_data6;
T_RX_SCI0_CMNG const _ddr_rx_sci0_cdata6 = {_ddr_rx_sci0_tbuf6, _ddr_rx_sci0_rbuf6,
            _ddr_rx_sci0_sbuf6, TXBUF_SZ6, RXBUF_SZ6, XOFF_SZ6, XON_SZ6, TXSEM6, RXSEM6};
T_RX_SCI0_PFI const _ddr_rx_sci0_pfi6 = {&PORT3.PORT.BYTE, 0x08, &PORT3.DR.BYTE, 0x04,
             IR_SCI6_TXI6, IER_SCI6_TXI6, 0x01, IR_SCI6_RXI6, IER_SCI6_RXI6, 0x80};
T_RX_SCI0_MNG const _ddr_rx_sci0_mng6 = {(volatile struct st_sci *)&SCI6,
            &_ddr_rx_sci0_pfi6, &_ddr_rx_sci0_cdata6, &_ddr_rx_sci0_data6};
#endif


/***************************************
        SCIデバイスドライバ初期化
 ***************************************/

void _ddr_rx_sci0_init(void)
{
  #ifdef SCI_0
    memset(&_ddr_rx_sci0_data0, 0x00, sizeof(_ddr_rx_sci0_data0));
    SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE0 == 0
        PORT2.DR.BIT.B0 = 1;
        PORT2.DDR.BIT.B0 = 1;
        PORT2.ICR.BIT.B1 = 1;
      #endif
    #elif RX_SRS == 625                /* RX62t */
      #if RELOCATE0 == 0
        PORTB.DR.BIT.B2 = 1;
        PORTB.DDR.BIT.B2 = 1;
        PORTB.ICR.BIT.B1 = 1;
      #endif
    #endif
    SCI0.SCR.BYTE = 0;
    ICU.IPR[0x80].BIT.IPR = IPL_SCI0;
    _ERIE0 = _RXIE0 = _TEIE0 = 1;
  #endif


  #ifdef SCI_1
    memset(&_ddr_rx_sci0_data1, 0x00, sizeof(_ddr_rx_sci0_data1));
    SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE1 == 0
        IOPORT.PFFSCI.BIT.SCI1S = 0;
        PORT2.DR.BIT.B6 = 1;
        PORT2.DDR.BIT.B6 = 1;
        PORT3.ICR.BIT.B0 = 1;
      #elif RELOCATE1 == 1
        IOPORT.PFFSCI.BIT.SCI1S = 1;
        PORTF.DR.BIT.B0 = 1;
        PORTF.DDR.BIT.B0 = 1;
        PORTF.ICR.BIT.B2 = 1;
      #endif
    #elif RX_SRS == 625                /* RX62t */
      #if RELOCATE1 == 0
        PORTD.DR.BIT.B3 = 1;
        PORTD.DDR.BIT.B3 = 1;
        PORTD.ICR.BIT.B5 = 1;
      #endif
    #endif
    SCI1.SCR.BYTE = 0;
    ICU.IPR[0x81].BIT.IPR = IPL_SCI1;
    _ERIE1 = _RXIE1 = _TEIE1 = 1;
  #endif


  #ifdef SCI_2
    memset(&_ddr_rx_sci0_data2, 0x00, sizeof(_ddr_rx_sci0_data2));
    SYSTEM.MSTPCRB.BIT.MSTPB29 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE2 == 0
        IOPORT.PFFSCI.BIT.SCI2S = 0;
        PORT1.DR.BIT.B3 = 1;
        PORT1.DDR.BIT.B3 = 1;
        PORT1.ICR.BIT.B2 = 1;
      #elif RELOCATE2 == 1
        IOPORT.PFFSCI.BIT.SCI2S = 1;
        PORT5.DR.BIT.B0 = 1;
        PORT5.DDR.BIT.B0 = 1;
        PORT5.ICR.BIT.B2 = 1;
      #endif
    #elif RX_SRS == 625                /* RX62t */
      #if RELOCATE2 == 0
        IOPORT.PFFSCI.BIT.SCI2S = 0;
        PORTB.DR.BIT.B5 = 1;
        PORTB.DDR.BIT.B5 = 1;
        PORTB.ICR.BIT.B6 = 1;
      #elif RELOCATE2 == 1
        IOPORT.PFFSCI.BIT.SCI2S = 1;
        PORT8.DR.BIT.B1 = 1;
        PORT8.DDR.BIT.B1 = 1;
        PORT8.ICR.BIT.B0 = 1;
      #endif
    #endif
    SCI2.SCR.BYTE = 0;
    ICU.IPR[0x82].BIT.IPR = IPL_SCI2;
    _ERIE2 = _RXIE2 = _TEIE2 = 1;
  #endif


  #ifdef SCI_3
    memset(&_ddr_rx_sci0_data3, 0x00, sizeof(_ddr_rx_sci0_data3));
    SYSTEM.MSTPCRB.BIT.MSTPB28 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE3 == 0
        IOPORT.PFFSCI.BIT.SCI3S = 0;
        PORT1.DR.BIT.B7 = 1;
        PORT1.DDR.BIT.B7 = 1;
        PORT1.ICR.BIT.B6 = 1;
      #elif RELOCATE3 == 1
        IOPORT.PFFSCI.BIT.SCI3S = 1;
        PORT2.DR.BIT.B3 = 1;
        PORT2.DDR.BIT.B3 = 1;
        PORT2.ICR.BIT.B5 = 1;
      #endif
    #endif
    SCI3.SCR.BYTE = 0;
    ICU.IPR[0x83].BIT.IPR = IPL_SCI3;
    _ERIE3 = _RXIE3 = _TEIE3 = 1;
  #endif


  #ifdef SCI_5
    memset(&_ddr_rx_sci0_data5, 0x00, sizeof(_ddr_rx_sci0_data5));
    SYSTEM.MSTPCRB.BIT.MSTPB26 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE5 == 0
        PORTC.DR.BIT.B3 = 1;
        PORTC.DDR.BIT.B3 = 1;
        PORTC.ICR.BIT.B2 = 1;
      #endif
    #endif
    SCI5.SCR.BYTE = 0;
    ICU.IPR[0x85].BIT.IPR = IPL_SCI5;
    _ERIE5 = _RXIE5 = _TEIE5 = 1;
  #endif


  #ifdef SCI_6
    memset(&_ddr_rx_sci0_data6, 0x00, sizeof(_ddr_rx_sci0_data6));
    SYSTEM.MSTPCRB.BIT.MSTPB25 = 0;
    #if RX_SRS == 620 || RX_SRS == 621 /* RX621,RX62n */
      #if RELOCATE6 == 0
        IOPORT.PFFSCI.BIT.SCI6S = 1;
        PORT3.DR.BIT.B2 = 1;
        PORT3.DDR.BIT.B2 = 1;
        PORT3.ICR.BIT.B3 = 1;
      #elif RELOCATE6 == 1
        IOPORT.PFFSCI.BIT.SCI6S = 0;
        PORT0.DR.BIT.B0 = 1;
        PORT0.DDR.BIT.B0 = 1;
        PORT0.ICR.BIT.B1 = 1;
      #endif
    #endif
    SCI6.SCR.BYTE = 0;
    ICU.IPR[0x86].BIT.IPR = IPL_SCI6;
    _ERIE6 = _RXIE6 = _TEIE6 = 1;
  #endif
}

#if (defined(SCI_0)||defined(SCI_1)||defined(SCI_2)||defined(SCI_3)||defined(SCI_5)||defined(SCI_6))

/***************************************
        SCI受信終了確認処理
 ***************************************/

BOOL _ddr_rx_sci0_check_chr(T_COM_RCV *ReceiveData, VB chr, UB sts)
{
    BOOL flag = FALSE;

    if ((sts & (T_COM_EROR|T_COM_ERP|T_COM_ERF|T_COM_BRK)) != 0)
        flag = TRUE;
    else if (ReceiveData->rcnt == 0)
        flag = TRUE;
    else if (ReceiveData->eos != 0)
    {
       if ((ReceiveData->eos->flg[0] != 0) && (ReceiveData->eos->chr[0] == chr))
            flag = TRUE;
        else if ((ReceiveData->eos->flg[1] != 0) && (ReceiveData->eos->chr[1] == chr))
            flag = TRUE;
        else if ((ReceiveData->eos->flg[2] != 0) && (ReceiveData->eos->chr[2] == chr))
            flag = TRUE;
        else if ((ReceiveData->eos->flg[3] != 0) && (ReceiveData->eos->chr[3] == chr))
            flag = TRUE;
    }
    return flag;
}

/*******************************************
        SCI受信エラーステータス解析
 *******************************************/

UB _ddr_rx_sci0_getssr(T_RX_SCI0_MNG const *pk_SCImng)
{
    volatile struct st_sci *port;
    UB ssr;
    UB sts = 0;

    port = pk_SCImng->port;

    ssr = port->SSR.BYTE;
    if ((ssr & 0x08) != 0)
        sts |= T_COM_ERP;
    if ((ssr & 0x10) != 0) {
        if ((*pk_SCImng->pfi->rxdr & pk_SCImng->pfi->rxbit) == 0)
            sts |= T_COM_BRK;
        else
            sts |= T_COM_ERF;
    }
    if ((ssr & 0x20) != 0)
        sts |= T_COM_EROR;
    port->SSR.BYTE = (ssr & ~0x38) | 0xc0;
    return sts;
}

/***********************************
        SCI送信終了割込み処理
 ***********************************/

void _ddr_rx_sci0_tei(T_RX_SCI0_MNG *pk_SCImng)
{
    pk_SCImng->port->SCR.BIT.TEIE = 0;
    isig_sem((ID)pk_SCImng->cdata->tsemid);
}

/*******************************
        バッファコピー処理
 *******************************/

BOOL _ddr_rx_sci0_copy(T_RX_SCI0_MNG const *pk_SCImng, T_COM_SND *TransmiteData)
{
    T_RX_SCI0_CMNG const  *cdata;
    T_RX_SCI0_DMNG        *data;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;

    for(; TransmiteData->tcnt != 0; ) {
        if (data->tcnt < cdata->tsize) {
            cdata->tbuf[data->sndp++] = *TransmiteData->tbuf++;
            TransmiteData->tcnt--;
            if (data->sndp >= cdata->tsize)
                data->sndp = 0;
            data->tcnt++;
        } else
            break;
    }
    return (TransmiteData->tcnt == 0) ? TRUE : FALSE;
}

/***********************************
        受信XOFFのチェック処理
 ***********************************/

void _ddr_rx_sci0_chk_rxoff(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if (!port->SCR.BIT.TE)    /* if we send break */
        return;

    if ((data->status.bit.rx_xoff == 0) &&
        (data->rcnt >= cdata->xoff_size)) {
        data->status.bit.rx_xoff = 1;
        if (port->SSR.BIT.TDRE == 1) {
            port->TDR = XOFF;
        } else
            data->status.bit.req_xon_xoff = 1;
    }
}

/***********************************
        受信XONのチェック処理
 ***********************************/

void _ddr_rx_sci0_chk_rxon(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if (!port->SCR.BIT.TE)    /* if we send break */
        return;

    if ((data->status.bit.rx_xoff == 1) &&
        (data->rcnt <= cdata->xon_size)) {
        data->status.bit.rx_xoff = 0;
        if (port->SSR.BIT.TDRE == 1) {
            port->TDR = XON;
        } else
            data->status.bit.req_xon_xoff = 1;
    }
}

/***********************************************
        ロカール送信バッファからの送信処理
 ***********************************************/

void _ddr_rx_sci0_send_local_buf(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    port->TDR = *data->SndData->tbuf++;
    if (--data->SndData->tcnt == 0) {
        data->SndData = 0;
        loc_cpu();

        ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
        IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);

        unl_cpu();
        isig_sem((ID)cdata->tsemid);
    }
}

/***********************************************
        内部送信バッファからの送信処理
 ***********************************************/

void _ddr_rx_sci0_send_drv_buf(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    UH sndp;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    sndp = data->sndp - data->tcnt;
    if (data->tcnt > data->sndp)
        sndp += cdata->tsize;
    port->TDR = cdata->tbuf[sndp];
    data->tcnt--;
    if (data->SndData != 0) {
        if (_ddr_rx_sci0_copy(pk_SCImng, data->SndData) == TRUE) {
            data->SndData = 0;
            isig_sem((ID)cdata->tsemid);
        }
    }
    if ((data->tcnt == 0) && (data->SndData == 0)) {
        loc_cpu();

        ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
        IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);

        unl_cpu();
    }

}

/***********************************************
        SCI送信データエンプティ割込み処理
 ***********************************************/

void _ddr_rx_sci0_txi(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if (port->SSR.BIT.TDRE == 1) {
        if (data->status.bit.req_xon_xoff == 1) {
            port->TDR = (data->status.bit.rx_xoff == 0) ? XON : XOFF;
            data->status.bit.req_xon_xoff = 0;
            if ((data->tcnt == 0) && (data->SndData == 0)) {
                loc_cpu();

                ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
                IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);                

                unl_cpu();
            }
        } else if (data->status.bit.tx_xoff == 0) {
            if (cdata->tsize == 0) {
                if (data->SndData != 0)
                    _ddr_rx_sci0_send_local_buf(pk_SCImng);
            } else {
                if (data->tcnt != 0)
                    _ddr_rx_sci0_send_drv_buf(pk_SCImng);
            }
        } else {
            loc_cpu();

            ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
            IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);         

            unl_cpu();
        }
    }
}

/***************************************
        SCI受信エラー割込み処理
 ***************************************/

void _ddr_rx_sci0_eri(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    UH rcvp;
    VB chr;
    UB sts;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    sts = _ddr_rx_sci0_getssr(pk_SCImng);
    if (data->status.bit.er_buf_ovr == 1)
        sts |= T_COM_EROVB;
    if ((data->status.bit.sns_brk == 0) || ((sts & T_COM_BRK) == 0)) {
        data->status.bit.sns_brk = ((sts & T_COM_BRK) != 0) ? 1 : 0;
        if (data->RcvData != 0) {
            data->RcvData->rcnt--;
            *data->RcvData->rbuf++ = chr = port->RDR;
            if (data->RcvData->sbuf != 0)
                *data->RcvData->sbuf++ = sts;
            data->status.bit.er_buf_ovr = 0;
            if (_ddr_rx_sci0_check_chr(data->RcvData, chr, sts) == TRUE) {
                data->RcvData = 0;
                isig_sem((ID)cdata->rsemid);
            }
        } else if (data->rcnt < cdata->rsize) {
            rcvp = data->rcvp + data->rcnt;
            if (rcvp >= cdata->rsize)
                rcvp -= cdata->rsize;
            cdata->rbuf[rcvp] = chr = port->RDR;
            if (cdata->sbuf != 0)
                cdata->sbuf[rcvp] = sts;
            data->status.bit.er_buf_ovr = 0;
            data->rcnt++;
            if (data->status.bit.sft_flw == 1)
                _ddr_rx_sci0_chk_rxoff(pk_SCImng);
            if (data->rcnt == cdata->rsize) {
                loc_cpu();
                ICU.IER[pk_SCImng->pfi->rxiier].BYTE &= ~pk_SCImng->pfi->rxibit;
                IOSYNC(ICU.IER[pk_SCImng->pfi->rxiier].BYTE);
                unl_cpu();
            }
        } else {
            data->status.bit.er_buf_ovr = 1;
        }
    }
}

/*******************************************
        SCI受信データフル割込み処理
 *******************************************/

void _ddr_rx_sci0_rxi(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    UH rcvp;
    VB chr;
    UB sts;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if ((data->rcnt < cdata->rsize) ||
        (data->RcvData != 0)) {
        sts = _ddr_rx_sci0_getssr(pk_SCImng);
        chr = port->RDR;

        if ((data->status.bit.sft_flw == 1) && (chr == XON)) {
            data->status.bit.tx_xoff = 0;
            if (port->SSR.BIT.TDRE == 1) {
                if (data->status.bit.req_xon_xoff == 1) {
                    port->TDR = (data->status.bit.rx_xoff == 0) ? XON : XOFF;
                    data->status.bit.req_xon_xoff = 0;
                } else if (data->tcnt != 0) {
                    _ddr_rx_sci0_send_drv_buf(pk_SCImng);
                } else if ((cdata->tsize == 0) &&
                           (data->SndData != 0)) {
                    _ddr_rx_sci0_send_local_buf(pk_SCImng);
                }
            }
        } else if ((data->status.bit.sft_flw == 1) && (chr == XOFF)) {
            data->status.bit.tx_xoff = 1;
            loc_cpu();

            ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
            IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);

            unl_cpu();
        } else {
            if (data->RcvData != 0) {
                data->RcvData->rcnt--;
                *data->RcvData->rbuf++ = chr;
                if (data->RcvData->sbuf != 0)
                    *data->RcvData->sbuf++ = (data->status.bit.er_buf_ovr == 0) ? sts : (sts | T_COM_EROVB);
                data->status.bit.er_buf_ovr = 0;
                if (_ddr_rx_sci0_check_chr(data->RcvData, chr, sts) == TRUE) {
                    data->RcvData = 0;
                    isig_sem((ID)cdata->rsemid);
                }
            } else if (data->rcnt < cdata->rsize) {
                rcvp = data->rcvp + data->rcnt;
                if (rcvp >= cdata->rsize)
                    rcvp -= cdata->rsize;
                cdata->rbuf[rcvp] = chr;
                if (cdata->sbuf != 0)
                    cdata->sbuf[rcvp] = (data->status.bit.er_buf_ovr == 0) ? sts : (sts | T_COM_EROVB);
                data->status.bit.er_buf_ovr = 0;
                data->rcnt++;
                if (data->status.bit.sft_flw == 1)
                    _ddr_rx_sci0_chk_rxoff(pk_SCImng);
            }
        }
    } else {
        data->status.bit.er_buf_ovr = 1;

        loc_cpu();
        ICU.IER[pk_SCImng->pfi->rxiier].BYTE &= ~pk_SCImng->pfi->rxibit;
        IOSYNC(ICU.IER[pk_SCImng->pfi->rxiier].BYTE);
        unl_cpu();

    }
}

/*******************************
        一文字送信処理
 *******************************/

void _ddr_rx_sci0_send_char(T_RX_SCI0_MNG const *pk_SCImng, T_COM_SND *TransmiteData)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if (data->status.bit.req_xon_xoff == 1) {
        port->TDR = (data->status.bit.rx_xoff == 0) ? XON : XOFF;
        data->status.bit.req_xon_xoff = 0;
    } else if (data->status.bit.tx_xoff == 0) {
        port->TDR = *TransmiteData->tbuf++;
        TransmiteData->tcnt--;
    }
}

/*******************************
        文字列の取り出し処理
 *******************************/

BOOL _ddr_rx_sci0_recv_strings(T_RX_SCI0_MNG const *pk_SCImng, T_COM_RCV *ReceiveData)
{
    T_RX_SCI0_CMNG const *cdata;
    T_RX_SCI0_DMNG       *data;
    BOOL flag;
    VB chr;
    UB sts;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;

    for (flag = FALSE; flag == FALSE; ) {
        if (ReceiveData->rcnt == 0)
            flag = TRUE;
        else if (data->rcnt == 0)
            break;
        else {
            *ReceiveData->rbuf++ = chr = cdata->rbuf[data->rcvp];
            sts = cdata->sbuf[data->rcvp];
            if (ReceiveData->sbuf != 0)
                *ReceiveData->sbuf++ = sts;
            if (++data->rcvp >= cdata->rsize)
                data->rcvp = 0;
            data->rcnt--;
            ReceiveData->rcnt--;
            if (data->status.bit.sft_flw == 1)
                _ddr_rx_sci0_chk_rxon(pk_SCImng);
            flag = _ddr_rx_sci0_check_chr(ReceiveData, chr, sts);
        }
    }
    return flag;
}

/*******************************
        SCI送信処理
 *******************************/

ER _ddr_rx_sci0_snd(T_COM_SND *TransmiteData, T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ID tskid;
    ER ercd = E_OK;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    if ((data->status.bit.init_flg == 0) || (sns_dpn() == TRUE))
        ercd = E_OBJ;
    else {
        get_tid(&tskid);
        dis_dsp();
        if (((data->tlockid == 0) || (data->tlockid == (UH)tskid)) &&
            (data->status.bit.ena_tx == 1) &&
            (data->SndData == 0)) {
            loc_cpu();

            ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
            IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);            
            ICU.IER[pk_SCImng->pfi->rxiier].BYTE &= ~pk_SCImng->pfi->rxibit;
            IOSYNC(ICU.IER[pk_SCImng->pfi->rxiier].BYTE);

            unl_cpu();
            if ((data->tcnt == 0) && (port->SSR.BIT.TDRE == 1))
                _ddr_rx_sci0_send_char(pk_SCImng, TransmiteData);
            loc_cpu();
            ICU.IER[pk_SCImng->pfi->rxiier].BYTE |= pk_SCImng->pfi->rxibit;            
            unl_cpu();
            if (_ddr_rx_sci0_copy(pk_SCImng, TransmiteData) == FALSE) {
                data->SndData = TransmiteData;
                loc_cpu();

                ICU.IER[pk_SCImng->pfi->txiier].BYTE |= pk_SCImng->pfi->txibit;

                unl_cpu();
                ena_dsp();
                if ((ercd = twai_sem((ID)cdata->tsemid, TransmiteData->time)) != E_OK) {
                    loc_cpu();

                    ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
                    IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);

                    unl_cpu();
                    data->SndData = 0;
                    ercd = pol_sem((ID)cdata->tsemid);
                }
            } else {
                if (data->tcnt != 0) {
                    loc_cpu();

                    ICU.IER[pk_SCImng->pfi->txiier].BYTE |= pk_SCImng->pfi->txibit;
                    
                    unl_cpu();
                }
                ena_dsp();
            }
        } else {
            ena_dsp();
            ercd = E_OBJ;
        }
    }
    return ercd;
}

/*******************************
        SCI受信処理
 *******************************/

ER _ddr_rx_sci0_rcv(T_COM_RCV *ReceiveData, T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    ID tskid;
    ER ercd = E_OK;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;

    if ((data->status.bit.init_flg == 0) || (sns_dpn() == TRUE) ||
        (data->status.bit.ena_rx   == 0))
        ercd = E_OBJ;
    else {
        get_tid(&tskid);
        dis_dsp();
        if (((data->rlockid == 0) || (data->rlockid == (UH)tskid)) &&
            (data->RcvData == 0)) {
            loc_cpu();

            ICU.IER[pk_SCImng->pfi->rxiier].BYTE &= ~pk_SCImng->pfi->rxibit;
            IOSYNC(ICU.IER[pk_SCImng->pfi->rxiier].BYTE);

            unl_cpu();
            if (_ddr_rx_sci0_recv_strings(pk_SCImng, ReceiveData) == FALSE) {
                data->RcvData = ReceiveData;
                loc_cpu();
                ICU.IER[pk_SCImng->pfi->rxiier].BYTE |= pk_SCImng->pfi->rxibit;
                unl_cpu();
                ena_dsp();
                if ((ercd = twai_sem((ID)cdata->rsemid, ReceiveData->time)) != E_OK) {
                    data->RcvData = 0;
                    ercd = pol_sem((ID)cdata->rsemid);
                }
            } else {
                loc_cpu();
                ICU.IER[pk_SCImng->pfi->rxiier].BYTE |= pk_SCImng->pfi->rxibit;
                unl_cpu();
                ena_dsp();
            }
        } else {
            ena_dsp();
            ercd = E_OBJ;
        }
    }
    return ercd;
}

/***********************************
        送信バッファの送出処理
 ***********************************/

ER _ddr_rx_sci0_cln_tx_buf(T_RX_SCI0_MNG const *pk_SCImng, TMO time)
{
    T_RX_SCI0_CMNG const   *cdata;
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ID tskid;
    ER ercd;

    cdata = pk_SCImng->cdata;
    data = pk_SCImng->data;
    port = pk_SCImng->port;

    get_tid(&tskid);
    if (((data->tlockid == 0) ||
         (data->tlockid == (UH)tskid)) &&
        ( data->SndData == 0)) {
        loc_cpu();
        if (data->tcnt != 0 || port->SSR.BIT.TEND == 0) {
            port->SCR.BIT.TEIE = 1;
            unl_cpu();
            if ((ercd = twai_sem((ID)cdata->tsemid, time)) != E_OK) {
                ercd = pol_sem((ID)cdata->tsemid);
            }
        } else {
            unl_cpu();
            ercd = E_OK;
        }
    } else
        ercd = E_OBJ;
    return ercd;
}

/***************************************
        送信バッファのリセット処理
 ***************************************/

ER _ddr_rx_sci0_rst_tx_buf(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG  *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    if (((data->tlockid == 0) ||
         (data->tlockid == (UH)tskid)) &&
        ( data->SndData == 0)) {
        data->tcnt = 0;
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/***************************************
        受信バッファのリセット処理
 ***************************************/

ER _ddr_rx_sci0_rst_rx_buf(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG  *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    if (((data->rlockid == 0) ||
         (data->rlockid == (UH)tskid)) &&
        ( data->RcvData == 0)) {
        data->rcnt = 0;
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        送信の禁止処理
 *******************************/

ER _ddr_rx_sci0_dis_send(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG  *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    if (((data->tlockid == 0) ||
         (data->tlockid == (UH)tskid)) &&
        ( data->SndData == 0) &&
        ( data->tcnt    == 0)) {
        loc_cpu();
        data->status.bit.ena_tx = 0;
        unl_cpu();
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        受信の禁止処理
 *******************************/

ER _ddr_rx_sci0_dis_rcv(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ER ercd;
    ID tskid;
    UB d;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    get_tid(&tskid);
    if (((data->rlockid == 0) ||
         (data->rlockid == (UH)tskid)) &&
        ( data->RcvData == 0) &&
        ( data->rcnt    == 0)) {
        loc_cpu();
        data->status.bit.ena_rx = 0;
        d = port->SCR.BYTE;
        port->SCR.BYTE = d & ~0x30;     /* TE = RE = 0 */
        port->SCR.BYTE = d & ~0x50;     /* RIE = RE = 0 */
        unl_cpu();
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        送信の許可処理
 *******************************/

ER _ddr_rx_sci0_ena_send(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ER ercd;
    ID tskid;
    UB d;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    get_tid(&tskid);
    if ((data->tlockid == 0) ||
        (data->tlockid == (UH)tskid)) {
        loc_cpu();
        data->status.bit.ena_tx = 1;
        d = port->SCR.BYTE;
        /* disable TXI */  

        ICU.IER[pk_SCImng->pfi->txiier].BYTE &= ~pk_SCImng->pfi->txibit;
        IOSYNC(ICU.IER[pk_SCImng->pfi->txiier].BYTE);    /* disable TXI */

        /* disable TXI */
        port->SCR.BYTE = d & ~0x30;     /* TE = RE = 0 */
        port->SCR.BYTE = d | 0xa0;      /* TIE | TE */
        unl_cpu();
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        受信の許可処理
 *******************************/

ER _ddr_rx_sci0_ena_rcv(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ER ercd;
    ID tskid;
    UB d;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    get_tid(&tskid);
    if ((data->rlockid == 0) ||
        (data->rlockid == (UH)tskid)) {
        loc_cpu();
        data->status.bit.ena_rx = 1;
        d = port->SCR.BYTE;
        port->SCR.BYTE = d & ~0x30;     /* TE = RE = 0 */
        port->SCR.BYTE = d | 0x50;      /* RIE | RE */
        unl_cpu();
        ercd = E_OK;
    } else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        ブレークの送出処理
 *******************************/

ER _ddr_rx_sci0_snd_brk(T_RX_SCI0_MNG const *pk_SCImng, TMO time)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    ER ercd;
    ID tskid;
    UB d;
    int ena_tx;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    get_tid(&tskid);
    if (((data->tlockid == 0) ||
         (data->tlockid == (UH)tskid)) &&
        ( data->SndData == 0) &&
        ( data->tcnt    == 0)) {
        if ((port->SSR.BIT.TEND) != 0) {
            loc_cpu();
            ena_tx = data->status.bit.ena_tx;
            data->status.bit.ena_tx = 0;
            *pk_SCImng->pfi->txdr &= ~pk_SCImng->pfi->txbit;
            d = port->SCR.BYTE;
            port->SCR.BYTE = d & ~0x30;
            port->SCR.BYTE = d & ~0x20;     /* TE = 0 */
            unl_cpu();
            dly_tsk(time);
            loc_cpu();
            *pk_SCImng->pfi->txdr |= pk_SCImng->pfi->txbit;
            d = port->SCR.BYTE;
            port->SCR.BYTE = d & ~0x30;
            port->SCR.BYTE = d |  0x20;     /* TE = 1 */
            data->status.bit.ena_tx = ena_tx;
            unl_cpu();
            ercd = E_OK;
        } else
            ercd = E_OBJ;
    } else
        ercd = E_OBJ;
    return ercd;
}

/***********************************
        送信チャネルのロック処理
 ***********************************/

ER _ddr_rx_sci0_lock_trans(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    loc_cpu();
    if ((data->tlockid == 0) ||
        (data->SndData == 0)) {
        data->tlockid = (UH)tskid;
        ercd = E_OK;
    } else if (data->tlockid == (UH)tskid)
        ercd = E_OK;
    else
        ercd = E_OBJ;
    unl_cpu();
    return ercd;
}

/***********************************
        受信チャネルのロック処理
 ***********************************/

ER _ddr_rx_sci0_lock_recv(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    loc_cpu();
    if ((data->rlockid == 0) ||
        (data->RcvData == 0)) {
        data->rlockid = (UH)tskid;
        ercd = E_OK;
    } else if (data->rlockid == (UH)tskid)
        ercd = E_OK;
    else
        ercd = E_OBJ;
    unl_cpu();
    return ercd;
}

/***************************************
        送信チャネルのロック解除処理
 ***************************************/

ER _ddr_rx_sci0_unlock_trans(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    if (data->tlockid == (UH)tskid) {
        data->tlockid = 0;
        ercd = E_OK;
    } else if (data->tlockid == 0)
        ercd = E_OK;
    else
        ercd = E_OBJ;
    return ercd;
}

/***************************************
        受信チャネルのロック解除処理
 ***************************************/

ER _ddr_rx_sci0_unlock_recv(T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG *data;
    ER ercd;
    ID tskid;

    data = pk_SCImng->data;

    get_tid(&tskid);
    if (data->rlockid == (UH)tskid) {
        data->rlockid = 0;
        ercd = E_OK;
    } else if (data->rlockid == 0)
        ercd = E_OK;
    else
        ercd = E_OBJ;
    return ercd;
}

/*******************************
        SCI制御信号処理
 *******************************/

ER _ddr_rx_sci0_ctr(const T_COM_CTR *pk_SerialData, T_RX_SCI0_MNG const *pk_SCImng)
{
    ER ercd = E_OK;

    if (pk_SCImng->data->status.bit.init_flg == 0)
        ercd = E_OBJ;

    if (ercd == E_OK)
        if ((pk_SerialData->command & CLN_TXBUF) == CLN_TXBUF)
            ercd = _ddr_rx_sci0_cln_tx_buf(pk_SCImng, pk_SerialData->time);

    if (ercd == E_OK)
        if ((pk_SerialData->command & RST_TXBUF) == RST_TXBUF)
            ercd = _ddr_rx_sci0_rst_tx_buf(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & RST_RXBUF) == RST_RXBUF)
            ercd = _ddr_rx_sci0_rst_rx_buf(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & STP_TX) == STP_TX)
            ercd = _ddr_rx_sci0_dis_send(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & STP_RX) == STP_RX)
            ercd = _ddr_rx_sci0_dis_rcv(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & STA_TX) == STA_TX)
            ercd = _ddr_rx_sci0_ena_send(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & STA_RX) == STA_RX)
            ercd = _ddr_rx_sci0_ena_rcv(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & SND_BRK) == SND_BRK)
            ercd = _ddr_rx_sci0_snd_brk(pk_SCImng, pk_SerialData->time);

    if (ercd == E_OK)
        if ((pk_SerialData->command & LOC_TX) == LOC_TX)
            ercd = _ddr_rx_sci0_lock_trans(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & LOC_RX) == LOC_RX)
            ercd = _ddr_rx_sci0_lock_recv(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & UNL_TX) == UNL_TX)
            ercd = _ddr_rx_sci0_unlock_trans(pk_SCImng);

    if (ercd == E_OK)
        if ((pk_SerialData->command & UNL_RX) == UNL_RX)
            ercd = _ddr_rx_sci0_unlock_recv(pk_SCImng);

    return ercd;
}

/*******************************
        SCI状態参照処理
 *******************************/

ER _ddr_rx_sci0_ref(T_COM_REF *pk_SerialRef, T_RX_SCI0_MNG const *pk_SCImng)
{
    T_RX_SCI0_DMNG         *data;
    UH status = 0;

    data = pk_SCImng->data;

    if (data->status.bit.init_flg == 1) {
        pk_SerialRef->rxcnt = data->rcnt;
        pk_SerialRef->txcnt = data->tcnt;

        status |= T_COM_DSR|T_COM_INIT|T_COM_CTS;
        if (data->status.bit.ena_tx == 1)
            status |= T_COM_ENATX;
        if (data->status.bit.ena_rx == 1)
            status |= T_COM_ENARX;
        if (data->status.bit.rx_xoff == 1)
            status |= T_COM_RXOFF;
        if (data->status.bit.tx_xoff == 1)
            status |= T_COM_TXOFF;
    }
    pk_SerialRef->status = status;
    return E_OK;
}

/*******************************
        SCI初期化処理
 *******************************/

ER _ddr_rx_sci0_ini(const T_COM_SMOD *pk_SerialMode, T_RX_SCI0_MNG const *pk_SCImng, ID DevID)
{
    T_RX_SCI0_DMNG         *data;
    volatile struct st_sci *port;
    UW brr;
    UW clk;
    ER ercd = E_OK;
    T_RX_SCI0_MSTS status;
    UB smr;
    int i;

    data = pk_SCImng->data;
    port = pk_SCImng->port;

    port->SCR.BYTE = 0;
    status.word = 0;
    clk = PCLK;
    for (smr = 0; smr < 4; smr++) {
        brr = clk / (16 * pk_SerialMode->baud) + 1;
        if (brr <= 512)
            break;
        clk /= 4;
    }
    if (smr == 4)
        ercd = E_PAR;

    switch(pk_SerialMode->blen) {
        case BLEN7:
            smr |= 0x40;
        case BLEN8:
            break;
        default:
            ercd = E_PAR;
            break;
    }

    switch(pk_SerialMode->par) {
        case PAR_ODD:
            smr |= 0x30;
        case PAR_EVEN:
            smr |= 0x20;
        default:
            break;
    }

    switch(pk_SerialMode->sbit) {
        case SBIT2:
            smr |= 0x08;
        case SBIT1:
            break;
        default:
            ercd = E_PAR;
            break;
    }

    switch(pk_SerialMode->flow) {
        case FLW_XON:
            status.bit.sft_flw = 1;
        case FLW_NONE:
            break;
        default:
            ercd = E_PAR;
            break;
    }

    if (ercd == E_OK) {
        status.bit.init_flg = 1;
        data->status.word = status.word;
        port->SMR.BYTE = smr;
        port->BRR = (UB)brr/2-1;
        if (sns_dpn() == TRUE)
            for(i=0; i<10000;i++)
                port->BRR;          /* dummy read */
        else
            dly_tsk(1);
    }
    
    data->id = DevID; // forRX DevID
   
    return ercd;
}

/*******************************
 　     SCIドライバメイン
 *******************************/

ER _ddr_rx_sci0drv(ID FuncID, VP pk_ControlData, T_RX_SCI0_MNG const *pk_SCImng, ID DevID)
{
    ER ercd;

    switch(FuncID) {
        case TA_COM_INI:
            ercd = _ddr_rx_sci0_ini((const T_COM_SMOD *)pk_ControlData, pk_SCImng, DevID);
            break;
        case TA_COM_REF:
            ercd = _ddr_rx_sci0_ref((T_COM_REF *)pk_ControlData, pk_SCImng);
            break;
        case TA_COM_CTR:
            ercd = _ddr_rx_sci0_ctr((const T_COM_CTR *)pk_ControlData, pk_SCImng);
            break;
        case TA_COM_PUTS:
            ercd = _ddr_rx_sci0_snd((T_COM_SND *)pk_ControlData, pk_SCImng);
            break;
        case TA_COM_GETS:
            ercd = _ddr_rx_sci0_rcv((T_COM_RCV *)pk_ControlData, pk_SCImng);
            break;
        default: ercd = E_NOSPT;
    }
    return ercd;
}
#endif
