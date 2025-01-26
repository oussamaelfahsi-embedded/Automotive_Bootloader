#ifndef UDS_SHARED_H
#define UDS_SHARED_H

#include <stdint.h>
#include "com.h"

#define UDS_FrameLength       8

extern volatile unsigned char    UDS_Rx_Frame[UDS_FrameLength]; // To store the Received Frame on the communication line 
extern volatile unsigned char    UDS_Tx_Frame[UDS_FrameLength]; // To store the Data to be send on the com line
extern unsigned char CurrentServiceID;    // used to store the current service ID                 
/* UDS Data Handlers    */

#define UDS_GetRxFrame()    UDS_Rx_Frame
#define UDS_SetTxFrame(Data)       \
        for(int i = 0 ; i < UDS_FrameLength ; i++ ){ \
            UDS_Tx_Frame[i] = Data[i]; }\


#define Diag_Send_Responce()      Com_Transmit(  UDS_Tx_Frame )   


// Runnables : 
unsigned char* Intr_Read_ReceivedData_UDS_Rx_Frame(  );
void Intr_WriteTxData_UDS_Tx_Frame(unsigned char TxData[]);
void Intr_SendData();
void SetCurrentServiceID(unsigned cher CurrSrvID);
unsigned char GetCurrentServiceID(void);
void SendDiagNegativeResponce(  unsigned char NRC);
void SendDiagPositiveResponce(unsigned char [] );

#endif