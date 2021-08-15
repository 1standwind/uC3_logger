/***********************************************************************
    Serial driver configuration
 ***********************************************************************/

#ifndef _DDR_RX_USART_CFG_H_
#define _DDR_RX_USART_CFG_H_

#include "kernel_id.h"
#include "rx62n.h"

#define RX_SRS    620  /* RX62N */

/* UART settings */
#define PCLK         48000000

#define SCI_0
#define TXBUF_SZ0   256
#define RXBUF_SZ0   256
#define XOFF_SZ0    192
#define XON_SZ0     64
#define TXSEM0      ID_UART0_TSEM
#define RXSEM0      ID_UART0_RSEM
#define IPL_SCI0    1
#define RELOCATE0   0

#endif /* _DDR_RX_USART_CFG_H_ */

/* end */
