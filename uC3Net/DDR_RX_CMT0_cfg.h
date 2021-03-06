/***********************************************************************
    Interval Timer code for RX CMT Timer
 ***********************************************************************/

/* TIMER Setup */

#define PCLK    48000000	/* PCLKB */
#define TICK    1	/* Tick time */
#define IPL_TIM     0x1	/* Priority */
#define CH_TIM  0          /* CMT timer ch. */
#define RX_SRS    620  /* RX62N */

#include "kernel_id.h"
#include "rx62n.h"

