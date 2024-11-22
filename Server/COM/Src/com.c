#include "com.h"
#include "config.h"


UART_HandleTypeDef huart2;
TIM_HandleTypeDef  Req_Timeout;
uint8_t Rx_Buffer[8] ;

void Com_Init(){

    #if COM_UART_ENABLE 
        /* UART protocol is used to communicate with Host */
        Com_UART_SetUp();
        HAL_UART_Receive_IT(&huart2, Rx_Buffer, 8 );

    #else 
        /* CAN protocol is used to communicate with Host    */
        Com_CAN_Init();

    #endif 
}



void Com_UART_SetUp(  ){
    
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }

}


void Com_CAN_Init(){

}


uint8_t Com_Transmit(  uint8_t* UDS_Data ){

    #if COM_UART_ENABLE 
        /* UART protocol is used to communicate with Host */
        if ( Com_UART_Transmit( UDS_Data ) != COM_OK ){
            return COM_ERROR ;
        }
    #else 
        /* CAN protocol is used to communicate with Host    */
        Com_CAN_Init();

    #endif 

    return COM_OK;
}

uint8_t Com_UART_Transmit( const uint8_t* TxData ){ /* TxData : 8 Bytes */


    if ( HAL_UART_Transmit( &huart2, TxData , UDS_FRAME_SIZE , HAL_MAX_DELAY ) != HAL_OK  ){
        return COM_ERROR;
    }
    return COM_OK ;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  HAL_TIM_Base_Start( &Req_Timeout );
}


void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  
}