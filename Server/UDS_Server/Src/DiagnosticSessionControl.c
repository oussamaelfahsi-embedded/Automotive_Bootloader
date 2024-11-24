#include "DiagnosticSessionControl.h"
#include "UDS_Shared.h"
#include "com.h"
#include "config.h"


void DiagSessCntrl_MainFct( void ){

    uint8_t Resp = CHECK_NOK; 
    SessionCnrtl_ReadData();
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
    uint32_t MSP_Value;
    uint8_t _check=CHECK_NOK;
    #if IS_FOR_APP
    MSP_Value = *((volatile uint32_t*)APP_MSP);
    _check=DiagSetResetHandlerAddr(IS_FOR_APP );
    #elif IS_FOR_FBL 
    MSP_Value = *((volatile uint32_t*)FBL_MSP);
    _check=DiagSetResetHandlerAddr(IS_FOR_FBL );
    #endif 
    DiagSetMSPValue(MSP_Value);
    DiagSetStateSession(__state);
    if(_check == CHECK_OK){
        DiagSetResetReady();
        if(DiagGetResetReady()){
        DiagSetResetNone();
        __SoftReset();
        }
    }
    return CHECK_OK;
}


uint8_t DiagSessionSwitch(uint8_t __state ){
    DiagSetResetInProgress();
    uint8_t _check = CHECK_NOK ;
    uint32_t MSP_Value;  
    #if IS_FOR_APP 
        MSP_Value = *((volatile uint32_t*)BM_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_BM );
    #elif IS_FOR_FBL
        MSP_Value = *((volatile uint32_t*)FBL_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_FBL );
    #endif 
    /* Set the MSP VALUE */
    DiagSetMSPValue(MSP_Value);

    /* Set the FLAG in the new session control */
    DiagSetStateSession(__state);
    if(_check == CHECK_OK){
        DiagSetResetReady();
        if(DiagGetResetReady()){
        DiagSetResetNone();
        __SoftReset();
        }
    }
    return _check;
}

/* This function will be implemented in ECUReset service */
uint8_t  DiagSetResetHandlerAddr(uint8_t __layer ){
    void (*reset_handler)(void) = ((void*)0);
    volatile uint32_t RESET_HANDLER_ADDR;
    if(__layer == IS_FOR_APP ){
        RESET_HANDLER_ADDR = APP_RESET_HANDLER_ADDR;
    }else if(__layer == IS_FOR_FBL){
        RESET_HANDLER_ADDR = FBL_RESET_HANDLER_ADDR;
    }else{
        RESET_HANDLER_ADDR = BM_RESET_HANDLER_ADDR;
    }
    reset_handler =*(void(*)(void))((volatile uint32_t*)RESET_HANDLER_ADDR);
    if(reset_handler != ((void*)0) ) {
         DiagSetResetHandAdrr(RESET_HANDLER_ADDR);
    }else{
        return CHECK_NOK;
    }
    return CHECK_OK    
}


