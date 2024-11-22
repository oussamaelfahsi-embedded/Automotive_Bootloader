#ifndef __CONFIG_H
#define __CONFIG_H


#define CHECK_OK                   0x00
#define CHECK_NOK                  0x01

#define COM_ENABLE                 0x01
#define COM_DISENABLE              0x00


#define COM_UART_ENABLE      COM_ENABLE
#define COM_CAN_ENBALE    COM_DISENABLE

/* UART PORT SUPPORTED BY TARGET  */
#define COM_UART1                 0x01
#define COM_UART2                 0x02

/* memory layout */
#define BM_MSP  /*  Boot Manger  @MSP*/
#define FBL_MSP /* Flash Bootloader @MSP*/
#define APP_MSP /*Application @MSP */

#define BM_RESET_HANDLER /* Boot Manger @RESET_HANDLER */
#define FBL_RESET_HANDLER /* Flash Bootloader @RESET_HANDLER*/
#define APP_RESET_HANDLER /* Application @RESET_HANDLER*/

#define IS_FOR_APP         0x01
#define IS_FOR_FBL         0x00


#endif