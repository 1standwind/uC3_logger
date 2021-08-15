/***********************************************************************
    Device Driver Manager
 ***********************************************************************/

#include "kernel.h"
#include "device_id.h"
#include "DDR_COM.h"
#include "DDR_RX_SCI0.h"

ER ctr_dev(ID DevID, ID FuncID, VP_INT ControlData);

extern ER _ddr_rx_sci0drv(ID, VP_INT, T_RX_SCI0_MNG const *, ID);
extern T_RX_SCI0_MNG const _ddr_rx_sci0_mng0;

ER ctr_dev(ID DevID, ID FuncID, VP_INT pk_ControlData)
{
    ER ercd;

    switch(DevID) {
        case DID_UART0: ercd = _ddr_rx_sci0drv(FuncID, pk_ControlData, &_ddr_rx_sci0_mng0, DevID);
            break;
        default: ercd = E_NOSPT;
    }
    return ercd;
}

/* end */
