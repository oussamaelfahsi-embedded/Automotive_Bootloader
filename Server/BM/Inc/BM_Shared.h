#include <stdint.h>


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

