/***********************************************************************
    Initialization of the hardware-dependent
 ***********************************************************************/

#include "kernel.h"

#pragma address id_code=0xffffffa0 // ID codes (Default)
const unsigned long id_code[4] = {
        0xffffffff,
        0xffffffff,
        0xffffffff,
        0xffffffff,
};

extern void _ddr_rx_cmt0_init(void);
extern void _ddr_rx_sci0_init(void);


/***********************************************
        Initialize RX peripherals
 ***********************************************/
void init_peripheral(void)
{

}

/***********************************************
  Device driver Initialize entry
 ***********************************************/
void _ddr_init(void)
{
    _ddr_rx_cmt0_init();
    _ddr_rx_sci0_init();
}

/* end */
