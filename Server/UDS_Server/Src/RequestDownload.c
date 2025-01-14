#include "RequestDownload.h"



void RequestDownload_Main(){

    uint8_t checkFlag = 0x00u  ;
    uin8_t tmp_NRC;
    uint8_t MemAddrLength;
    uint8_t MemSizeLength;
    DownloadStartAddr = (RxData[3]<<24)|(RxData[4]<<16)|(RxData[5]<<8)|RxData[6];

    // Data Format id & Address And length FormatId : are Valid 
    MemAddrLength = RxData[2] && 0x0F;
    MemSizeLength = RxData[2] && 0xF0;
    MemSizeLength = MemSizeLength>>4;
    
    if( RxData[1] != NoMethodUsed & RxData[1] != EncryptedMethod ){
        checkFlag = 0x01u;
        tmp_NRC = ROOR;
    }else if( MemAddrLength + MemSizeLength > 5  ){
        // Send an error Message  
        checkFlag = 0x01u;
        tmp_NRC = IMLOIF;
    }else if( !GET_SECURITYACCESS_VALID() ){
        checkFlag = 0x01;
        tmp_NRC = SAD; // Security Access denied 
    }else if( !(MEM_is_addr_Valid(  DownloadStartAddr  , Application_Section ) | MEM_is_addr_Valid(  DownloadStartAddr  , Calibration_Section )) ){
        checkFlag = 0x01u;
        tmp_NRC = ROOR;
    }else{
        // 0x50 : 5 is the number of Data bytes can the client end each time
        DownloadMemorySize = RxData[7];
        uint8_t UDS_Frame[8] = { 0x74, 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        SendDiagPositiveResponce(UDS_Frame);
    }

    if(checkFlag == 0x01u){
        SendDiagNegativeResponce(tmp_NRC);
    }


}

void SendDiagPositiveResponce(   uint8_t UDS_Frame[]  ){
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}



void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = 0x7Fu;
    UDS_Frame[1] = 0x34u;
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}