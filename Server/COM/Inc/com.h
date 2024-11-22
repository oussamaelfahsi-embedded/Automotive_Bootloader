#ifndef __COM_H
#define __COM_H

#include <stdint.h>

#include "stm32h7xx_hal_fdcan.h"

#define UDS_FRAME_SIZE        8 

#define COM_OK            0x00U
#define COM_ERROR         0x10U

/* In case using CAN */
struct CAN_Frame {
    uint16_t Arbitration_Field ; /* ID + RTR        12 Bytes  */
    uint8_t  Control_Field     ; /* IDE + R0 + DLC   6 Bits   */
    uint8_t  Data[8]           ; /* Data             8 Bytes  */
    uint16_t CRC               ; /* CRC             15 Bits   */
};

/* Functions Prototypes */
void Com_Init(void);
uint8_t Com_Transmit(uint8_t*);
uint8_t Com_UART_Transmit(const uint8_t*);



#endif