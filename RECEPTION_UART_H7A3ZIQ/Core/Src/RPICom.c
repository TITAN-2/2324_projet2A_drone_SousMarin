#include <RPICom.h>

uint8_t uartRpiRxBuffer[UART_RPI_RX_BUFFER_SIZE];
commandMsg binaryMessage;

void RPICom_Init(void){
	RPICom_UartActivate();
}

void RPICom_UartActivate(void){
	HAL_UART_Receive_IT(&huart4, uartRpiRxBuffer, UART_RPI_RX_BUFFER_SIZE);
}


void RPICom_DecodeBinaryMessage(void)
{
	// A RAJOUTER --> Check header
	// A RAJOUTER --> Length
	binaryMessage.thrust = uartRpiRxBuffer[0];
	binaryMessage.angle = uartRpiRxBuffer[1];
	binaryMessage.depth = ((uint16_t)uartRpiRxBuffer[2]+(uint16_t)(uartRpiRxBuffer[3]<<8));
	binaryMessage.paquetNumber = uartRpiRxBuffer[4];
	// A RAJOUTER --> Checksum
	// A RAJOUTER --> Réponse à la RPI
	HAL_UART_Transmit(&huart3, &binaryMessage, 5, 10);

	// HAL_UART_Transmit(&huart3,&recvd_data,sizeof(recvd_data));
	// HAL_UART_Transmit(&huart3,&binaryMessage,sizeof(binaryMessage));

	// Vous avez maintenant un message complet dans la structure BinaryMessage
	// Vous pouvez faire quelque chose avec ces données, par exemple, appeler une fonction de traitement


	//Process(&binaryMessage);
}



//HAL_UART_Receive_IT(&huart4,&recvd_data,sizeof(recvd_data));


/*void Process(BinaryMessage* message){
	AdjustThrust(message->thrust);
	AdjustAngle(message->angle);

	HAL_UART_Transmit_IT(&huart3,message->thrust,1); //receive data from data buffer interrupt mode
	HAL_UART_Transmit_IT(&huart3,message->angle,1);
}*/

//void AdjustThrust(int thrust){}

//void AdjustAngle(int angle){}
