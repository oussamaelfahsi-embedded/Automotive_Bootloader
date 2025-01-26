#include "DiagnosticSessionControl.h"
#include "UDS_Shared.h"
#include "com.h"
#include "config.h"

unsigned char *RxData ;
static unsigned char tmpReceivedData[8];

void DiagSessCntrl_MainFct( void ){

    unsigned char Resp = UDS_NOK; 
    SessionCnrtl_Init();
    /* Verifing the Sub Fnct */
    if(  DiagCheckSubFunctionCode( Diag_SessControl_GetNextSession() ) != UDS_OK ){
        SendDiagNegativeResponce(SFNS); //  Defined in UDS_Shared File
    }
    if( GetDaigSessionFlag() == UDS_NOK ){
        SendDiagNegativeResponce(CNC);
    }
    if(DiagGetCurrentStateSession == Diag_SessControl_GetNextSession()  ){
        Resp = DiagSessionReInit( Diag_SessControl_GetNextSession());
    }else {
        Resp = DiagSessionSwitch(Diag_SessControl_GetNextSession());
    }
    if(Resp !=UDS_OK){
        SendDiagNegativeResponce( CNC);
    }
    /**/
    return Resp;
}

// 
void SessionCnrtl_Init(){
    RxData = Intr_Read_ReceivedData_UDS_Rx_Frame();     //  Read Received Data 
    CopyDataBetwenTwoTables(tmpReceivedData , RxData ); //  Copy the Received Data into a Local Variable 
    SetCurrentServiceID(tmpReceivedData[0]);            //  Set the Current Service ID to the var CurrentServiceID
}

unsigned char DiagCheckSubFunctionCode( unsigned char Sub_Fct_Code ){
    if(Sub_Fct_Code > FblDiagExtendedSession || Sub_Fct_Code < FblDiagStateDefaultSession ){
        return UDS_NOK;
    }
    return UDS_OK ;
}


unsigned char DiagSessionReInit(unsigned char __state){ 
    unsigned int MSP_Value;
    unsigned char _check=UDS_NOK;
    #if IS_FOR_APP
    MSP_Value = *((volatile unsigned int*)APP_MSP);
    _check=DiagSetResetHandlerAddr(IS_FOR_APP );
    #elif IS_FOR_FBL 
    MSP_Value = *((volatile unsigned int*)FBL_MSP);
    _check=DiagSetResetHandlerAddr(IS_FOR_FBL );
    #endif 
    DiagSetMSPValue(MSP_Value);
    DiagSetStateSession(__state);
    if(_check == UDS_OK){
        DiagSetResetReady();
        if(DiagGetResetReady()){
        DiagSetResetNone();
        // The UDS Frame to Send as a responce 
        unsigned char tmp_UDSFrame[8]={0x50u, Diag_SessControl_GetNextSession(),0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        SendDiagPositiveResponce(tmp_UDSFrame);
        __SoftReset();
        }
    }
    return UDS_OK;
}


unsigned char DiagSessionSwitch(unsigned char __state ){
    DiagSetResetInProgress();
    unsigned char _check = UDS_NOK ;
    unsigned int  MSP_Value;  
    #if IS_FOR_APP 
        MSP_Value = *((volatile unsigned int*)BM_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_BM );
    #elif IS_FOR_FBL
        MSP_Value = *((volatile unsigned int*)FBL_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_FBL );
    #endif 
    /* Set the MSP VALUE */
    DiagSetMSPValue(MSP_Value);

    /* Set the FLAG in the new session control */
    DiagSetStateSession(__state);
    if(_check == UDS_OK){
        DiagSetResetReady();
        if(DiagGetResetReady()){
        // The UDS Frame to Send as a responce 
        unsigned char tmp_UDSFrame[8]={0x50u, Diag_SessControl_GetNextSession(),0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        SendDiagPositiveResponce(tmp_UDSFrame);
        __SoftReset();
        }
    }
    return _check;
}

/* This function will be implemented in ECUReset service */
unsigned char  DiagSetResetHandlerAddr(unsigned char __layer ){
    void (*reset_handler)(void) = ((void*)0);
    volatile unsigned int RESET_HANDLER_ADDR;
    if(__layer == IS_FOR_APP ){
        RESET_HANDLER_ADDR = APP_RESET_HANDLER_ADDR;
    }else if(__layer == IS_FOR_FBL){
        RESET_HANDLER_ADDR = FBL_RESET_HANDLER_ADDR;
    }else{
        RESET_HANDLER_ADDR = BM_RESET_HANDLER_ADDR;
    }
    reset_handler =*(void(*)(void))((volatile unsigned int*)RESET_HANDLER_ADDR);
    if(reset_handler != ((void*)0) ) {
        DiagSetResetHandAdrr(RESET_HANDLER_ADDR);
    }else{
        return UDS_NOK;
    }
    return UDS_OK;    
}


