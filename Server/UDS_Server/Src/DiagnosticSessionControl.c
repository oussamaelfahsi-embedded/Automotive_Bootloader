#include "DiagnosticSessionControl.h"
#include "UDS_Shared.h"
#include "com.h"
#include "config.h"


void DiagSessCntrl_MainFct( void ){

    uint8_t Resp = CHECK_NOK; 
    Diag_Set_Next_Session();
    /* Verifing the Sub Fnct */
    if(  DiagCheckSubFunctionCode( Diag_SessControl_GetNextSession ) != CHECK_OK ){
        SendDiagNegativeResponce(SFNS);
    }
    if( GetDaigSessionFlag() == CHECK_NOK ){
        SendDiagNegativeResponce(CNC);
    }
    if(DiagGetCurrentStateSession == Diag_SessControl_GetNextSession  ){
        Resp = DiagSessionReInit( Diag_SessControl_GetNextSession);
    }else {
        Resp = DiagSessionSwitch(Diag_SessControl_GetNextSession);
    }
    if(Resp !=CHECK_OK){
        SendDiagNegativeResponce( CNC);
    }
    /**/
    return Resp;
}

uint8_t DiagCheckSubFunctionCode( uint8_t Sub_Fct_Code ){
    if(Sub_Fct_Code > FblDiagExtendedSession || Sub_Fct_Code < FblDiagStateDefaultSession ){
        return CHECK_NOK;
    }
    return CHECK_OK ;
}

void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = NEGATIVE_RESPONSE_SID;
    UDS_Frame[1] = Diag_SessControl_GetSID;
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}

uint8_t SendDiagPositiveResponce(  uint8_t Sub_Fct  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = SESSCRT_POSITIVE_RESPONSE_SID;
    UDS_Frame[1] = Sub_Fct;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}


uint8_t DiagSessionReInit(uint8_t __state){ 
    uint32_t Msp_Set;
    #if IS_FOR_APP
    Msp_Set = *((volatile uint32_t*)APP_MSP);
    #elif IS_FOR_FBL 
    Msp_Set = *((volatile uint32_t*)FBL_MSP);
    #endif 
    __set_MSP(Msp_Set);
    /* Disable Timer */
    if(HAL_TIM_Base_Stop(&Timer) != HAL_OK){
        return CHECK_NOK;
    }
    if(SendDiagPositiveResponce(__state) != CHECK_OK ){
        return CHECK_NOK;
    }
    if(DiagSessionRunResetHandler( __state ) != CHECK_OK){
        return CHECK_NOK;
    }
    return CHECK_OK;
}

uint8_t DiagSessionSwitch(uint8_t __state ){
    /* Set the New Session */
    uint32_t Msp_Set;  
    #if IS_FOR_APP 
    Msp_Set = *((volatile uint32_t*)BM_MSP);
    #elif IS_FOR_FBL
    Msp_Set = *((volatile uint32_t*)FBL_MSP);
    #endif 
    __set_MSP(Msp_Set);
    /* Stop Timer       */ 
    if(HAL_TIM_Base_Stop(&Timer) != HAL_OK){
        return CHECK_NOK;
    }
    /* Set the FLAG in the new session control */
    DiagSetStateSession(__state);
    if(SendDiagPositiveResponce(__state) != CHECK_OK ){
        return CHECK_NOK;
    }
    if(DiagSessionRunResetHandler( __state ) != CHECK_OK){
        return CHECK_NOK;
    }
    return CHECK_OK;
}

uint8_t  DiagSessionRunResetHandler(uint8_t session ){
    void (*reset_handler)(void) = ((void*)0);
    volatile uint32_t RESET_HANDLER_ADDR;
    #if IS_FOR_APP
        if(session == DEFAULT_SESSION){
            RESET_HANDLER_ADDR = APP_RESET_HANDLER;
        }else {
            RESET_HANDLER_ADDR = BM_RESET_HANDLER;
        }
        reset_handler =*(void(*)(void))((volatile uint32_t*)RESET_HANDLER_ADDR);
    #elif IS_FOR_FBL
        reset_handler =*(void(*)(void))((volatile uint32_t*)FBL_RESET_HANDLER);
    #endif 
    if(reset_handler !=  ((void*)0)){
    reset_handler();
    }
    return CHECK_NOK    
}


