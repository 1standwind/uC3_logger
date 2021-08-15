/***********************************************************************
    Ethernet driver configuration for CH0
 ***********************************************************************/

#ifndef _DDR_RX_ETH_CFG_H_
#define _DDR_RX_ETH_CFG_H_

#include "COMMONDEF.h"
#include "rx62n.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RX_SRS    620  /* RX62N */

/*
    Configure PHY Address
*/
#define ETH_PHY_ADDR    0x0

/*
    Configure Interrupt priority level
*/
#define IPL_ETHERNET    1

/*
    Configure System Clock (Hz)
*/
#define ICLK            96000000

/*
    Configure PHY Mode
--------------------------------
    (Default)   0 = Auto select mode
                1 = 10M Half Duplex manual mode
                2 = 10M Full/Half (Duplex auto select mode)
                3 = 100M Half Duplex manual mode
                4 = 100M Full/Half (Duplex auto select mode)
*/
#define ETH_PHY_MODE    0

/*
    Configure Address Filter Mode
--------------------------------
    (Default)   0 = Filter disable (Perfect filtering)
                1 = promiscuous mode (receive all packets)
                2 = multicast filter mode
*/
#define ETH_FILTER_MODE    0

/*
    Configure Rx DMA Descriptor count
--------------------------------
    `buffer-size * num-of-buffer' must be greater than equal 2048
*/
#define ETH_TXDESC_CNT    4
#define ETH_RXDESC_CNT    4

#define ETH_BUFSZ    0x600

#ifdef __cplusplus
}
#endif
#endif   /* _DDR_RX_ETH_CFG_H_ */ 
