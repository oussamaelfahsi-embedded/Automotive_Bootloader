#include "UDS_Shared.h"


volatile unsigned char  UDS_Rx_Frame[UDS_FrameLength];  
volatile unsigned char  UDS_Tx_Frame[UDS_FrameLength];  
unsigned char CurrentServiceID; 

// Interface to read the received data stored at UDS_Rx_Frame variable  
unsigned char* Intr_Read_ReceivedData_UDS_Rx_Frame(  ){
    return UDS_Frame;
}
// Interface to write data at UDS_Tx_Frame 
void Intr_WriteTxData_UDS_Tx_Frame(unsigned char TxData[]){
    UDS_SetTxFrame(TxData);
}

// interface to send UDS_Tx_Frame on communication line 
void Intr_SendData(){
    Diag_Send_Responce();
}

// interface to set the Current (which the ECU Treat ) Service ID 
void SetCurrentServiceID(unsigned char CurrSrvID){
    CurrentServiceID = CurrSrvID; 
}
// Interface to Read the Current Service ID 
unsigned char GetCurrentServiceID(){
    return CurrentServiceID;
}

// To Send a Negative Response 
void SendDiagNegativeResponce(  unsigned char NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] =  0x7Fu;
    UDS_Frame[1] = GetCurrentServiceID();
    UDS_Frame[2] = NRC;
    Intr_WriteTxData_UDS_Tx_Frame(UDS_Frame);
    Intr_SendData();
}

// To Send a Positive Responce 
void SendDiagPositiveResponce(  unsigned char FrameToSend[]  ){
    Intr_WriteTxData_UDS_Tx_Frame(FrameToSend);
    Intr_SendData();
}

