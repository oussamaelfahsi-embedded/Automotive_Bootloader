#ifndef UDS_SHARED_H
#define UDS_SHARED_H

#include <stdint.h>
#include "com.h"

#define UDS_FrameLength       8

volatile uint8_t   UDS_Rx_Frame[UDS_FrameLength];
volatile uint8_t UDS_Tx_Frame[UDS_FrameLength];
/* UDS Data Handlers    */

#define UDS_GetRxFrame()    UDS_Rx_Frame
#define UDS_SetTxFrame(Data)       \
        for(int i = 0 ; i < UDS_FrameLength ; i++ ){ \
            UDS_Tx_Frame[i] = Data[i]; }\


#define Diag_Send_Responce()      Com_Transmit(  UDS_Tx_Frame )   


#endif