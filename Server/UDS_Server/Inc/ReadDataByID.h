#ifndef __READDATABYID_H
#define __READDATABYID_H

#include "BM_Shared.h"

static uint8_t *RxData ;

#define NUMBER_OF_DATA   5
#define DIDs_POSITIVE_RESPONSE_SID    0x62u

/* DIDs and their corresponding variables  */
#define DID_APP_VER        0x00u  /* APPLICATION VERSION  */
#define DID_BL_VER         0x01u
#define DID_VIN_NUM        0x02u
#define DID_ACTIVE_SESS    0x03u
#define DID_HW_VER         0x04u

#define GET_SIZE_VAR(__var)   (sizeof(__var))   
#define GET_SIZE_APPL_VER()  GET_SIZE_VAR( uint32_t )    
#define GET_SIZE_BL_VER()    GET_SIZE_VAR(uint32_t)   
#define GET_SIZE_VIN_NUM()   GET_SIZE_VAR(uint8_t)   
#define GET_SIZE_ACTIVE_SESS()   GET_SIZE_VAR(uint32_t)   
#define GET_SIZE_HW_VER()   GET_SIZE_VAR(uint32_t)   

/* Table store address of variable readable by the client */
uint32_t* DataToRead[NUMBER_OF_DATA] = {
    &APPLICATION_VER,            // Application version
    &BOOTLODAER_VER,             // Bootloader version
    VIN_NUMBER,                 // VIN Number
    &ACTIVE_SESSION,             // Active Session
    &HW_VERSION                  // Hardware Version
}  ;



#define GET_DATA_ID()   ((uint16_t)( (RxData[1]<<8)|RxData[2] ))
#define GET_SID()       (RxData[0])
#define GET_SIZE_DATA(__DID)    ( DIDs_Size[__DID] )

/* NRCs */
#define ROOR        0x31u  // Request out of range 
#define SAD         0x33u  // Security Access Denied 
#define CNC         0x22   // Conditions Not Correct 
#define RTL         0x14   // Response Too Long 


#endif 