#include <stdint.h>


/* Security Access */
volatile uint8_t SecurityAccessState;
#define SECURITYACCESS_VALID         0x00u
#define SECURITYACCESS_DENIED        0x01u  

#define SET_SECURITYACCESS_STATE(_State)        (SecurityAccessState=_State)

#define GET_SECURITYACCESS_VALID()    (SecurityAccessState&SECURITYACCESS_VALID)   


#define FblBmNoOfMagicBytes         8 
/* Diagnostic Response States            */
#define DiagErrorNone               0x00
#define DiagErrorYes                0x01

/* Positive Response States           */
#define DiagResponseIdle                 0x00u             /**< Response not yet configured */
#define DiagResponseRequested            0x01u          /**< Response should be sent */
#define DiagPutNoResponse                0x02u          /**< No response should be sent */
#define DiagPutNoFuncResponse            0x03u          /**< No response should be sent for functional requests */
#define DiagPutPosResponse               0x04u          /**< Positive response requested */
#define DiagPutNegResponse               0x05u          /**< Negative response requested - supersedes positive response request */
#define DiagRepeatResponse               0x06u          /**< Response should be repeated */

/**/

volatile uint8_t           fblBmMagicFlag[FblBmNoOfMagicBytes];

volatile uint32_t          ECUReset_Config[2]; /* MSP, @Reset Handler*/
#define DiagSetMSPValue(__Value)     (ECUReset_Config[0]=__Value;)
#define DiagSetResetHandAdrr(__addr) (ECUReset_Config[1]=__addr;)

volatile uint8_t          diagErrorCode;
/* Flag indicating the state of service executing*/
volatile uint8_t          diagServiceInProgress;
/** Flag indicating the state of positive response */
volatile uint8_t          diagResponseFlag;


/* DIDs   */

typedef struct {
    uint8_t size ;
    uint8_t table;
    uint8_t secure;
}DIDs_Info;

uint32_t  APPLICATION_VER  ;
uint32_t  BOOTLODAER_VER   ;
uint8_t   VIN_NUMBER[32]   ;
uint32_t  ACTIVE_SESSION   ;
uint32_t  HW_VERSION       ;

DIDs_Info Str_Application_ver;
DIDs_Info Str_Bootloader_ver;
DIDs_Info Str_Vin_number;
DIDs_Info Str_Active_session;
DIDs_Info Str_Hw_version;


#define SET_APPLICATION_VER(__newValue)    (APPLICATION_VER=__newValue)   // Application version
#define SET_BOOTLODAER_VER(__newValue)     (BOOTLODAER_VER=__newValue)    // Bootloader version
#define SET_VIN_NUMBER(__newValue)         (VIN_NUMBER=__newValue)        // VIN Number
#define SET_ACTIVE_SESSION(__newValue)     (ACTIVE_SESSION=__newValue)    // Active Session
#define SET_HW_VERSION(__newValue)         (HW_VERSION=__newValue)        //Hardware Version    
