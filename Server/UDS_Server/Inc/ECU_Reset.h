#ifndef  __ECU_RESET_H
#define  __ECU_RESET_H

#include <stdint.h>
#include "fbl_diag_core.h"
#include "UDS_Shared.h"
#include "fbl_diag_core.h"

uint8_t *RxData ;
volatile uint8_t DiagResetFlag;


/* Reset State Handler    */
#define ResetStateNone           0x00
#define ResetStateReadyToRun     0x01
#define ResetStateInProgress     0x02

#define DiagSetResetNone()        (DiagResetFlag=ResetStateNone;)
#define DiagSetResetReady()       (DiagResetFlag=ResetStateReadyToRun;)
#define DiagSetResetInProgress() (DiagResetFlag=ResetStateInProgress;) 
#define DiagGetResetReady()       (DiagResetFlag==ResetStateReadyToRun;)
/* Negative responses Codes NRC         */
#define SFNS    0x12  /* Sub-function Not Supported                */
#define IMLOIF  0x13  /* Incorrect Message Length Or Invalid Format*/
#define CNC     0x22  /* Conditions Not Correct                    */
#define SAD     0x33  /*Security Acess Denied                      */

/* Reset Types           */
#define HardReset      0x01
#define SoftReset      0x02

#define Diag_ECUReset_GetSID             RxData[0]
#define Diag_ECUReset_GetSubFct          RxData[1]
#define ECUReset_POSITIVE_RESPONSE_SID      0x51

#define DiagClrError()                      ( diagErrorCode = DiagErrorNone )
#define FblDiagClrServiceInProgress()       ( diagServiceInProgress=DiagServiceStopped)
#define DiagConfigSetMSP()                        (__set_MSP(ECUReset_Config[0]);)
#define DiagGetResetHandler()               (*(void(*)(void))((volatile uint32_t*)ECUReset_Config[1]);)
#define __SoftReset()                       (Diag_EcuSoftReset();)

/* Function prototype   */

#endif