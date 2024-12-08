#include "ReadDataByID.h"
#include <string.h>

uint8_t ReadDataByID_Main(){
    uint16_t __DID = GET_DATA_ID();
    uint8_t __size;
    uint8_t __table;
    if(!GET_SECURITYACCESS_VALID()){
        SendDiagNegativeResponce(SAD);
        return 1;
    }

    if(__DID > NUMBER_OF_DATA ){
        /*Error */
        SendDiagNegativeResponce(ROOR);
        return 1 ;
    }

    switch( __DID ){
        case DID_APP_VER :
            __size = GET_SIZE_APPL_VER();
            __table = 0;
            break;
        case DID_BL_VER:
            __size = GET_SIZE_BL_VER();
            __table = 0;
            break;
        case DID_VIN_NUM:
            __size = GET_SIZE_VIN_NUM();
            __table = 32;
            uint8_t* fromData = ((uint8_t*)DataToRead[DID_VIN_NUM]);
            break;
        case DID_ACTIVE_SESS:
            __size = GET_SIZE_ACTIVE_SESS();
            __table = 0;
            break; 
        case DID_HW_VER:
            __size = GET_SIZE_HW_VER();
            __table = 0;
            break; 
        
    }

    uint8_t UDS_Frame[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if(__table == 0 ){
        /* Variable's value is represneted in 4 Bytes  */
        uint32_t Value ;
        if(__size == 1){
            Value = *((uint8_t*)DataToRead[__DID]);
        }else if(__size == 2 ){
            Value = *((uint16_t*)DataToRead[__DID]);
        }else{
            Value = *((uint32_t*)DataToRead[__DID]);
        }
        UDS_Frame[0] =  (Value>>24) & 0xFF;  // Value MSB 
        UDS_Frame[1] =  (Value>>16) & 0xFF;
        UDS_Frame[2] =  (Value>>8) & 0xFF;
        UDS_Frame[3] =  (Value) & 0xFF;    // Value LSB
        SendDiagPositiveResponce(__DID , UDS_Frame  );
    }else{
        /* if the variable is a table */
        if((__size*__table) > 5  ){
            /* Send a negative response to indicate that the Response Too Long */
            SendDiagNegativeResponce( RTL );
            uint8_t pData[__size * __table];
            memcpy(pData , fromData  , __size*__table);
            TraitAndSendData( __DID,  pData ,  __size*__table);
        }else{
            /* __size*__table = 5 or less  */
            memcpy(UDS_Frame, fromData , __size*__table);
            SendDiagPositiveResponce(__DID , UDS_Frame  );
        }
    }
    return 0;
}

void TraitAndSendData( uint16_t DID,  uint8_t* pData , uint8_t size){
    uint8_t UDS_Data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t offset = 0;
    for(int i = 0 ; i < size/5 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            UDS_Data[j] = pData[(i*5)+j];
            offset++;
        }
        SendDiagPositiveResponce( DID , UDS_Data  );
    }
    if(offset != size ){
        int j = 0 ;
        for(int i = offset ; i <size ; i++ ){
            UDS_Data[j] = pData[i];
            j++;
        }
        SendDiagPositiveResponce( DID , UDS_Data  );
    }


}

void SendDiagPositiveResponce(  uint16_t DID , uint32_t* Value  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = DIDs_POSITIVE_RESPONSE_SID;
    UDS_Frame[1] = (DID>>8) & 0xFF;// DID MSB 
    UDS_Frame[2] =  DID & 0xFF;      // DID LSB 

    UDS_Frame[3] = Value[4];  // Value MSB 
    UDS_Frame[4] = Value[3];
    UDS_Frame[5] = Value[2];
    UDS_Frame[6] = Value[1];    
    UDS_Frame[7] = Value[0];    // Value LSB
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}

void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = 0x7F;
    UDS_Frame[1] = GET_SID();
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}
