#ifndef REQUESTDOWNLOAD_H
#define REQUESTDOWNLOAD_H
#include "BM_Shared.h"
#include "SecurityAccess.h"

/*
Service Request Download 0x34 

Description :
Used by the client to initiate a data transfer from the client to the server (download). 
After the server has received the requestDownload request message the server shall take all necessary actions to receive data before 
it sends a positive response message.

--------------------------------------------------------------------------------------------------------------------------------------
Request message : 
0x34 1 Byte | DataFormatId 1 Byte | Address&LengthFormatId 1 Byte | MemoryAddress  | MemorySize  |

- DataFormatId : 0x01 ; Encrypted 
- Address&LengthFormatId 1 Bytes : 
    - bit 7:4 Length of the memorySize parameter.
    - bit 3:0 Length of the memoryAddress parameter.   
- MemoryAddress 
- MemorySize

Notes : 
    - Memory size in KBytes. 
    - In our implementation the MemAddress will take 4 Bytes.

*/


static uint8_t *RxData;
volatile uint32_t DownloadStartAddr; // store the start address to performing the download 
volatile uint32_t DownloadMemorySize; 

/*  SubFonction Supported             */
#define NoMethodUsed                 0x00u  
#define EncryptedMethod              0x01u

/* Variables used                     */
uint32_t MemAddress;
uint32_t MemSize;

/* NRC                               */
#define IMLOIF                     0x13u
#define CNC                        0x22u
#define ROOR                       0x31u 
#define SAD                        0x33u
#define UDNA                       0x70u // Upload Download Not Accepted 


#endif