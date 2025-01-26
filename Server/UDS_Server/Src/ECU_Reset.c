#include "ECU_Reset.h"
#include "UDS_Shared.h"

volatile unsigned char DiagResetFlag;
static unsigned char tmpReceivedData[8]; // holds the received data from a communication line. 
unsigned char UDS_Frame[8];

unsigned char ECUResetMain(){
    ECUReset_Init();
    unsigned char subfct = tmpReceivedData[1]; // Reset Type
    if(subfct != HardReset && subfct != SoftReset ){
        SendDiagNegativeResponce(SFNS);
        return UDS_OK;
    }
    ResetRxMessage(UDS_Frame);
    UDS_Frame[0] = ECUReset_POSITIVE_RESPONSE_SID;
    UDS_Frame[1] = Sub_Fct;
    SendDiagPositiveResponce(UDS_Frame);
    if(subfct == SoftReset ){
        __SoftReset();
    }else{
        __HardReset();
    }
    return 0;
}

void ECUReset_Init(){
    RxData = Intr_Read_ReceivedData_UDS_Rx_Frame();
    CopyDataBetwenTwoTables(tmpReceivedData , RxData );
    SetCurrentServiceID(tmpReceivedData[0]); 
}

void Diag_EcuHardReset(){
    DiagSetResetNone();
    DiagSetNoResponse();
    HAL_NVIC_SystemReset();
}

void Diag_EcuSoftReset() {
    DiagSetResetNone();
    DiagSetNoResponse();
    FblDiagDeinit();
    DiagConfigSetMSP();
    DiagGetResetHandler();
}

void DiagDeinit(){
     /* Destroye allocated section in RAM : Memory Buffer */
    Memory_Deinit();
    DiagResetServiceFlags();
    /* Stop The Timer */
    HAL_TIM_Base_Stop(&htim17);
    /* Disable The Timer Interrupt */
    HAL_TIM_Base_Stop_IT(&htim17);
}

void DiagResetServiceFlags(){
    /* Clear negative response indicator */
    DiagClrError();
   /* Reset internal state in case no response was sent */
   FblDiagClrServiceInProgress();
   /* diagResponse state back to idle */
   diagResponseFlag = DiagResponseIdle;
}

void Memory_Deinit(){

}

void ResetRxMessage(unsigned char* RxMssg[] ){
    RxMssg[0] = 0xFFu;
    RxMssg[1] = 0xFFu;
    RxMssg[2] = 0xFFu;
    RxMssg[3] = 0xFFu;
    RxMssg[4] = 0xFFu;
    RxMssg[5] = 0xFFu;
    RxMssg[6] = 0xFFu;
    RxMssg[7] = 0xFFu;

}

void __SoftReset(){
    Diag_EcuSoftReset();
}

void __HardReset(){
    Diag_EcuHardReset();
}

/*
void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = 0x7F;
    UDS_Frame[1] = Diag_ECUReset_GetSID();
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}

void SendDiagPositiveResponce(  uint8_t Sub_Fct  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = ECUReset_POSITIVE_RESPONSE_SID;
    UDS_Frame[1] = Sub_Fct;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}
*/