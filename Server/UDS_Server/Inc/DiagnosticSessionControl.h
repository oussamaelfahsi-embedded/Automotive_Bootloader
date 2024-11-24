/**
 * @file   DiagnosticSessionControl.h
 * @author Oussama elfahsi
 * @brief  : Header for UDS_SessionControlSrv.c file.
  *        This file contains the common defines of UDS Session Control Service.
  ******************************************************************************       
 * @version 0.1
 * @date 2024-11-09
 * 
 * @copyright Copyright (c) SIGMA EMBEDDED 2024
 * All rights reserved.
 * 
 */
#ifndef  __DIAGNOSTICSESSIONCONTROL_H
#define  __DIAGNOSTICSESSIONCONTROL_H

#include <stdint.h>
#include "fbl_diag_core.h"
#include "UDS_Shared.h"
#include "fbl_diag_core.h"
#include "ECU_Reset.h"

uint8_t *RxData ;
 

/* Negative responses Codes NRC */
#define SFNS    0x12 /* Sub-function Not Supported ------------------------------------*/
#define IMLOIF  0x13 /* incorrect Message Length Or Invalid Format --------------------*/
#define CNC     0x22 /* Conditions Not Correct ----------------------------------------*/

#define SessionCnrtl_ReadData()    { RxData = UDS_GetRxFrame; }

#define Diag_SessControl_GetSID             RxData[0]
#define Diag_SessControl_GetNextSession     RxData[1]
#define SESSCRT_POSITIVE_RESPONSE_SID       0x50

            
/* Functions Prototype  -------------------- ------------------------------------------*/
void DiagSessCntrl_MainFct( void );
uint8_t DiagCheckSubFunctionCode(uint8_t);
void SendDiagNegativeResponce(uint8_t);
uint8_t SendDiagPositiveResponce(uint8_t);
uint8_t DiagSessionReInit(uint8_t);
uint8_t DiagSessionSwitch(uint8_t);

#endif 