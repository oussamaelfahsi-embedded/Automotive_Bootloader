#include "ECU_Reset.h"


void Diag_EcuSoftReset(void) {
    DiagSetNoResponse();
    FblDiagDeinit();
    DiagConfigSetMSP();
    DiagGetResetHandler();
}

void DiagDeinit(){
     /* Destroye allocated section in RAM : Memory Buffer */
    Memory_Deinit();
    DiagResetServiceFlags();
    /* Disable The Timer */
    HAL_TIM_Base_Stop(&htim17);
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