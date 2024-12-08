#include "ReadDataByID.h"
#include <string.h>

uint8_t ReadDataByID_Main(){
    uint16_t tmp_id = GET_DATA_ID();
    uint8_t tmp_var.size;
    uint8_t tmp_var.table;
    DIDs_Info tmp_var;
    tmp_var = Init_Dids(tmp_id);

    if( tmp_var.secure == 1  && !GET_SECURITYACCESS_VALID()){
        SendDiagNegativeResponce(SAD);
        return 1;
    }

    if(tmp_id > NUMBER_OF_DATA ){
        /*Error */
        SendDiagNegativeResponce(ROOR);
        return 1 ;
    }
    uint8_t UDS_Frame[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if(tmp_var.table == 0 ){
        /* Variable's value is represneted in 4 Bytes  */
        uint32_t Value ;
        if(tmp_var.size == 1){
            Value = *((uint8_t*)DataToRead[tmp_id]);
        }else if(tmp_var.size == 2 ){
            Value = *((uint16_t*)DataToRead[tmp_id]);
        }else{
            Value = *((uint32_t*)DataToRead[tmp_id]);
        }
        UDS_Frame[0] =  (Value>>24) & 0xFF;  // Value MSB 
        UDS_Frame[1] =  (Value>>16) & 0xFF;
        UDS_Frame[2] =  (Value>>8) & 0xFF;
        UDS_Frame[3] =  (Value) & 0xFF;    // Value LSB
        SendDiagPositiveResponce(tmp_id , UDS_Frame  );
    }else{
        uint8_t* fromData = NULL;
        if(tmp_id == DID_VIN_NUM ){
            fromData = ((uint8_t*)DataToRead[DID_VIN_NUM]);
        }
        /* if the variable is a table */
        if((tmp_var.size*tmp_var.table) > 5  ){
            /* Send a negative response to indicate that the Response Too Long */
            SendDiagNegativeResponce( RTL );
            uint8_t pData[tmp_var.size * tmp_var.table];
            memcpy(pData , fromData  , tmp_var.size*tmp_var.table);
            TraitAndSendData( tmp_id,  pData ,  tmp_var.size*tmp_var.table);
        }else{
            /* tmp_var.size*tmp_var.table = 5 or less  */
            memcpy(UDS_Frame, fromData , tmp_var.size*tmp_var.table);
            SendDiagPositiveResponce(tmp_id , UDS_Frame  );
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

DIDs_Info Init_Dids(uint8_t DID){
        DIDs_Info tmp;
        switch( DID ){
        case DID_APP_VER :
            /* Application  */
            Str_Application_ver.size = GET_SIZE_APPL_VER();
            Str_Application_ver.table = 0;
            Str_Application_ver.secure = 0;
            tmp = Str_Application_ver;
            break;
        case DID_BL_VER:
            /*Bootloader version  */
            Str_Bootloader_ver.size = GET_SIZE_BL_VER();
            Str_Bootloader_ver.table = 0;
            Str_Bootloader_ver.secure = 0;
            tmp = Str_Bootloader_ver;
            break;
        case DID_VIN_NUM:
            /* VIN_NUMBER */
            Str_Vin_number.size  = GET_SIZE_VIN_NUM();
            Str_Vin_number.table = GET_SIZE_TABLE_VIN_NUM() ;
            Str_Vin_number.secure = 0;
            tmp = Str_Vin_number;
            break;
        case DID_ACTIVE_SESS:
            /* Active Session */
            Str_Active_session.size  = GET_SIZE_ACTIVE_SESS();
            Str_Active_session.table = 0;
            Str_Active_session.secure  = 0;
            tmp = Str_Active_session;
            break; 
        case DID_HW_VER:
            /* Hardware Version */
            Str_Hw_version.size = GET_SIZE_HW_VER();
            Str_Hw_version.table = 0;
            Str_Hw_version.secure = 0;
            tmp = Str_Hw_version;
            break; 
        
    }
    return tmp;
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
