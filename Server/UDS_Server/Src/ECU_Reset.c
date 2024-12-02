#include "ECU_Reset.h"


uint8_t ECUResetMain(){
    ECUReset_ReadData();
    uint8_t subfct = Diag_ECUReset_GetSID();
    if(subfct != HardReset && subfct != SoftReset ){
        SendDiagNegativeResponce(SFNS);
        return 1;
    }
    DiagSetPositiveResponse();
    SendDiagPositiveResponce(Diag_ECUReset_GetSubFct());
    if(subfct == SoftReset ){
        __SoftReset();
    }else{
        __HardReset();
    }
    return 0;
}

void Diag_EcuHardReset(){
    DiagSetResetNone();
    DiagSetNoResponse();
    HAL_NVIC_SystemReset();
}

void Diag_EcuSoftReset(void) {
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