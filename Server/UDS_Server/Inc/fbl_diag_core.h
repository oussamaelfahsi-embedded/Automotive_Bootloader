#ifndef FBL_DIAG_CORE
#define FBL_DIAG_CORE

#include <stdint.h>


/* FblDiagSession : is like the ECU State 
*  FblDiagSession = FblDiagProgrammingSession in case of a Prog request
*/
volatile uint8_t FblDiagSession ;
/* Bootloader States */
#define FblDiagStateDefaultSession     0U
#define FblDiagProgrammingSession      1U
#define FblDiagExtendedSession         2U


/* Negatif Responce */
#define NEGATIVE_RESPONSE_SID          0x7F
/* Diagnostic services */
#define DiagSidDiagnosticSessionControl                 0x10u    /**< Service ID - Diagnostic session control */
#define DiagSidEcuReset                                 0x11u    /**< Service ID - ECU reset */
#define DiagSidSecurityAccess                           0x27u    /**< Service ID - Security Access */
#define DiagSidRoutineControl                           0x31u    /**< Service ID - Routine Control */
#define DiagSidRequestDownload                          0x34u    /**< Service ID - Request Download */
#define DiagSidTransferData                             0x36u    /**< Service ID - Transfer Data */
#define DiagSidRequestTransferExit                      0x37u    /**< Service ID - Request Transfer Exit */


#define DiagStateIsProgrammingSession()          (FblDiagSession == FblDiagProgrammingSession )
#define DiagStateIsDefaultSession()              (FblDiagSession == FblDiagStateDefaultSession )
#define DiagStateIsExtendedSession()             (FblDiagSession == FblDiagExtendedSession )
#define DiagSetStateSession(state)               (FblDiagSession = state)
#define DiagGetCurrentStateSession                FblDiagSession

/* Response Flag Handling       */
#define DiagSetNoResponse()             (diagResponseFlag |= DiagPutNoResponse)
#define DiagSetPositiveResponse()       (diagResponseFlag |= DiagPutNoResponse)

#endif