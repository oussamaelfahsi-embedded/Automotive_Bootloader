#include "SecurityAccess.h"

uint8_t SecurityUnlockedSteps  = SeedRequet;
uint16_t SeedValue;
uint8_t CostKey[6] = {}; 
uint8_t Calckey[6]; 
uint8_t keysFlag = 0x01u;
/*
Step 1 : request seed 
Step 2 : Send Key 
 */


void SecurityAccess_Main(){
    /* Check Sub-Fonction           */
    if(RxData[1] !=  SecurityUnlockedSteps ){
        /* Request Sequence Error  */
        SendDiagNegativeResponce(RSE);
    }
    /*                        */
    switch(SecurityUnlockedSteps){
        case SeedRequet: 
            SeedValue = 0xFE59u;
            /* Send the seed value */
            SendDiagPositiveResponce( RequetSeed , &SeedValue );
            /* Calculate the key   */
            keyCalculationAlgorithm();
            /* Set the next step   */
            SecurityUnlockedSteps = SendKeyRequest;
            /* Demand the key      */
            SendDiagPositiveResponce( Sendkey , NULL );
        break ;
        case Receivekey: 
        /*Comparing Keys          */
        for(int i = 0 ; i < 6 ; i++ ){
            if(Calckey[i] != RxData[i+2] ){
                keysFlag = 0x00;
            }
        }
        if(keysFlag == 0x00u){
            /* Invalid Key          */
            RTE_Write_DataElement_SecurityAccessState(SECURITYACCESS_DENIED );// ECU Locked 
            SendDiagNegativeResponce(IK);
        }else{
            RTE_Write_DataElement_SecurityAccessState(SECURITYACCESS_VALID );// ECU UnLocked 
            SendDiagPositiveResponce(NULL);
        }

        break ;

    }

}






void keyCalculationAlgorithm(){
    /* 
    - AES S-Box
    - The key is on 6 Bytes. {k0, K1, K2, K3, K4, K5}
    - The Seed Key on 2 Bytes.
        Step 01 : 
            key[0] = Key[0] xor SeedKey[0]  
            key[1] = Key[1] xor SeedKey[1] 
            and : 
            key[0] =AES S-BOX[Key[0]][Key[0]>>4]
            key[1] =AES S-BOX[Key[1]][Key[1]>>4]
        Step 02 :
            key[2] = Key[2] xor Key[0]  
            key[3] = Key[3] xor Key[1] 
            and : 
            key[2] =AES S-BOX[Key[2]][Key[2]>>4]
            key[3] =AES S-BOX[Key[3]][Key[3]>>4]
        ...
    
    */

    uint8_t AES_SBox[16][16]={
        {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
        {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
        {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
        {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
        {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
        {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
        {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
        {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
        {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
        {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
        {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
        {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
        {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
        {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
        {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
        {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
    };
    uint8_t index0, index1;
    for(int i = 0 ; i < 6 ; i++ ){
        if(i == 0 || i == 1 ){
            /*key[0] = Key[0] xor SeedKey[0]  and Key[1] = Key[1] xor SeedKey[1] */
            Calckey[i] = CostKey[i] ^ (SeedKey>>i*8) ;
            index0 = Calckey[i]& 0x0Fu;
            index1 = Calckey[i]& 0xF0u;
            index1 = index1>>4;
            Calckey[i] = AES_SBox[index0][index1];
        }else {
            Calckey[i] = CostKey[i] ^  Calckey[i-2];
            index0 = Calckey[i]& 0x0Fu;
            index1 = Calckey[i]& 0xF0u;
            index1 = index1>>4;
            Calckey[i] = AES_SBox[index0][index1];
        }
    }

}

void SendDiagPositiveResponce(   uint8_t securityAccessType, const  uint16_t* Value  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = SA_POS_RS_ID;
    UDS_Frame[1] = RxData[1]; /* securityAccessType */
    if(Value != NULL ){
    UDS_Frame[2] = (Value>>8);
    UDS_Frame[3] = (Value);
    }

    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}


void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = 0x7Fu;
    UDS_Frame[1] = 0x27u;
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}

unsigned char SecurityAccess_Denied(){
    unsigned char SA_state = RTE_Read_DataElement_SecurityAccessState();
    return !(SA_state&SECURITYACCESS_VALID);
}




