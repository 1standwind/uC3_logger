/***********************************************************************
    Micro C Cube Compact, DEVICE DRIVER
    Serial Interface for RX62N SCI

    Copyright (c)  2011-2012, eForce Co., Ltd. All rights reserved.

    Version Information
            2011.10.26: Created.
            2012.08.06: Supported for EWRX Compiler.
            2012.11.19: Correspond to use multi channel. 
            2013.03.22: Applicated for EWRX.
            2013.09.26: Add txiir,txiier,txibit,rxiir,rxiier,rxibit in T_RX_SCI0_PFI.
 ***********************************************************************/

#ifndef DDR_RX_SCI0_H
#define DDR_RX_SCI0_H

#if defined(__RENESAS__)
#define __sfr
#endif
#include "hw_dep.h"

typedef union t_rx_sci0_msts {
    UH              word;
    struct {
        UH          init_flg:1;
        UH          ena_tx:1;
        UH          ena_rx:1;
        UH          sft_flw:1;
        UH          hrd_flw:1;
        UH          sns_brk:1;
        UH          tx_xoff:1;
        UH          rx_xoff:1;
        UH          req_xon_xoff:1;
        UH          er_buf_ovr:1;
        UH          dummy:6;
    } bit;
} T_RX_SCI0_MSTS;

typedef struct t_rx_sci0_cmng {
    VB              *tbuf;
    VB              *rbuf;
    UB              *sbuf;
    UH              tsize;
    UH              rsize;
    UH              xoff_size;
    UH              xon_size;
    UH              tsemid;
    UH              rsemid;
} T_RX_SCI0_CMNG;

typedef struct t_rx_sci0_dmng {
    T_COM_SND       *SndData;
    T_COM_RCV       *RcvData;
    T_RX_SCI0_MSTS  status;
    UH              sndp;
    UH              rcvp;
    UH              tcnt;
    UH              rcnt;
    UH              tlockid;
    UH              rlockid;
    UB              id; 
} T_RX_SCI0_DMNG;

typedef struct t_rx_sci0_pfi {
    volatile UB __sfr __evenaccess *rxdr;
    UB          rxbit;
    volatile UB __sfr __evenaccess *txdr;
    UB          txbit;
    UB              txiir;
    UB              txiier;
    UB              txibit;
    UB              rxiir;
    UB              rxiier;
    UB              rxibit;
} T_RX_SCI0_PFI;

typedef struct t_rx_sci0_mng {
    volatile struct st_sci  *port;
    T_RX_SCI0_PFI const     *pfi;
    T_RX_SCI0_CMNG const    *cdata;
    T_RX_SCI0_DMNG          *data;
} T_RX_SCI0_MNG;

#endif /* DDR_RX_SCI0_H */
