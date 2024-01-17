#include <RPICom.h>

uint8_t uartRpiRxBuffer[UART_RPI_RX_BUFFER_SIZE];
commandMsg binaryMessage;

void RPICom_Init(void){
	RPICom_UartActivate();
}

void RPICom_UartActivate(void){
	HAL_UART_Receive_IT(&huart4, uartRpiRxBuffer, UART_RPI_RX_BUFFER_SIZE);
}

uint8_t checkSum(void){
	uint8_t checkSum = 0;
	for (int i = 0; i < UART_RPI_RX_BUFFER_SIZE-1; i++) { //Il ne faut pas prendre en compte le bit ou checkSum est present
	        checkSum ^= uartRpiRxBuffer[i];
	    }
	return checkSum;
}

void RPICom_DecodeBinaryMessage(void)
{
	char error = '0';
	// Check Header
	if (  HEADER_CheckValue1 != uartRpiRxBuffer[0] || HEADER_CheckValue2 != uartRpiRxBuffer[1]){//On envoie une erreur à la raspberry
		error ='1';}
	// Check Length
	if (UART_RPI_RX_BUFFER_SIZE != uartRpiRxBuffer[2]){// Il nous manque des informations
	error='2';
	}

	binaryMessage.thrust = uartRpiRxBuffer[3];
	binaryMessage.angle = uartRpiRxBuffer[4];
	binaryMessage.depth = ((uint16_t)uartRpiRxBuffer[5]+(uint16_t)(uartRpiRxBuffer[6]<<8));
	binaryMessage.paquetNumber = uartRpiRxBuffer[7];

	//Checksum
	if (checkSum() !=  uartRpiRxBuffer[8]){
		error='3';
	}

	//Response RPI
	char RPI_Response[50];
	sprintf(RPI_Response, "Paquet: %u\r\nStatut: %c\r\n", binaryMessage.paquetNumber,error);
	HAL_UART_Transmit(&huart3, (uint8_t*)RPI_Response, sizeof(RPI_Response), 10);
	RPICom_UartActivate();

	// Vous avez maintenant un message complet dans la structure BinaryMessage
	// Vous pouvez faire quelque chose avec ces données, par exemple, appeler une fonction de traitement


	//Process(&binaryMessage);
}



//HAL_UART_Receive_IT(&huart4,&recvd_data,sizeof(recvd_data));


/*void Process(commandMsg* message){
	AdjustThrust(message->thrust);
	AdjustAngle(message->angle);

	HAL_UART_Transmit_IT(&huart3,message->thrust,1); //receive data from data buffer interrupt mode
	HAL_UART_Transmit_IT(&huart3,message->angle,1);
}*/

//void AdjustThrust(int thrust){}

//void AdjustAngle(int angle){}
