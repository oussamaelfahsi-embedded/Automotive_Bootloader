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
#define MSP_VALUE           /* Based on the each layer */

#define BM_RESET_HANDLER /* Boot Manger @RESET_HANDLER */
#define FBL_RESET_HANDLER /* Flash Bootloader @RESET_HANDLER*/
#define APP_RESET_HANDLER /* Application @RESET_HANDLER*/

#define IS_FOR_APP         0x01
#define IS_FOR_FBL         0x00


/* Memory Organization */
typedef struct{
    uint8_t sector_size; /* sector size in KBytes */
    uint16_t bank_size;  /*Size of each bank */
    uint32_t Flash_start_addr;
    uint32_t num_banks;   /* Number of banks */

} MemoryOrganization;

MemoryOrganization MCU_FLASH;




#endif