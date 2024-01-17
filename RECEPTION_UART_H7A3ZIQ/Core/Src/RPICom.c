#include <RPICom.h>

//uint8_t uartRpiRxBuffer[UART_RPI_RX_BUFFER_SIZE];
//commandMsg binaryMessage;
RPICom_HandleTypeDef hRPICom;

void RPICom_Init(UART_HandleTypeDef* huart, UART_HandleTypeDef* huartDebug){
	hRPICom.huart = huart;
	hRPICom.huartDebug = huartDebug;
	memcpy(&(hRPICom.binaryMessage), NULL, sizeof(commandMsg));
	hRPICom.errorNumber = 0;
	memcpy(hRPICom.RxBuffer, NULL, UART_RPI_RX_BUFFER_SIZE);
	memcpy(hRPICom.TxBuffer, NULL, UART_RPI_TX_BUFFER_SIZE);
	RPICom_UartActivate(&hRPICom);
}

void RPICom_UartActivate(RPICom_HandleTypeDef* hRPI){
	HAL_UART_Receive_IT(hRPI->huart, hRPI->RxBuffer, UART_RPI_RX_BUFFER_SIZE);
}

uint8_t checkSum(void){
	uint8_t checkSum = 0;
	for (int i = 0; i < UART_RPI_RX_BUFFER_SIZE-1; i++) { //Il ne faut pas prendre en compte le bit ou checkSum est present
		checkSum ^= hRPICom.RxBuffer[i];
	}
	return checkSum;
}

void RPICom_DecodeBinaryMessage(void)
{
	hRPICom.errorNumber = '0';
	// Check Header
	if (HEADER_CheckValue1 != hRPICom.RxBuffer[0] || HEADER_CheckValue2 != hRPICom.RxBuffer[1]){//On envoie une erreur à la raspberry
		hRPICom.errorNumber ='1';
	}
	// Check Length
	if (UART_RPI_RX_BUFFER_SIZE != hRPICom.RxBuffer[2]){// Il nous manque des informations
		hRPICom.errorNumber='2';
	}

	hRPICom.binaryMessage.thrust = hRPICom.RxBuffer[3];
	hRPICom.binaryMessage.angle = hRPICom.RxBuffer[4];
	hRPICom.binaryMessage.depth = ((uint16_t)hRPICom.RxBuffer[5]+(uint16_t)(hRPICom.RxBuffer[6]<<8));
	hRPICom.binaryMessage.paquetNumber = hRPICom.RxBuffer[7];

	//Checksum
	if(checkSum() !=  hRPICom.RxBuffer[8]){
		hRPICom.errorNumber='3';
	}

	//Response RPI
	uint8_t stringLength = snprintf((char *)hRPICom.TxBuffer, UART_RPI_TX_BUFFER_SIZE, "Paquet: %u\r\nStatut: %c\r\n", hRPICom.binaryMessage.paquetNumber,hRPICom.errorNumber);
	HAL_UART_Transmit(hRPICom.huartDebug, hRPICom.TxBuffer, stringLength, 10);
	RPICom_UartActivate(&hRPICom);

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
