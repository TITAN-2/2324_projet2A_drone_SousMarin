#include <RPICom.h>

RPICom_HandleTypeDef hRPICom;
void RPICom_Init(UART_HandleTypeDef* huart, UART_HandleTypeDef* huartDebug){
	hRPICom.huart = huart;
	hRPICom.huartDebug = huartDebug;
	memcpy(&(hRPICom.binaryMessage), NULL, sizeof(receiveMsg));
	hRPICom.errorNumberRx = 0;
	memcpy(hRPICom.RxBuffer, NULL, UART_RPI_RX_BUFFER_SIZE);
	memcpy(hRPICom.TxBuffer, NULL, UART_RPI_TX_BUFFER_SIZE);
	memcpy(hRPICom.DebugBuffer, NULL, UART_RPI_DEBUG_BUFFER_SIZE);
	hRPICom.sendMessage.paquetNumber = 0;

	RPICom_UartActivate(&hRPICom);
}

void RPICom_UartActivate(RPICom_HandleTypeDef* hRPI){
	HAL_UART_Receive_IT(hRPI->huart, hRPI->RxBuffer, UART_RPI_RX_BUFFER_SIZE);
}

uint8_t checkSum(uint8_t buffer[],uint8_t buffer_size){
	uint8_t checkSum = 0;
	for (int i = 0; i < buffer_size; i++) { //Il ne faut pas prendre en compte le bit ou checkSum est present
		checkSum ^= buffer[i];
	}
	return checkSum;
}

void RPICom_DecodeBinaryMessage(void)
{
	hRPICom.errorNumberRx = '0';
	// Check Header
	if (HEADER_CheckValue1 != hRPICom.RxBuffer[0] || HEADER_CheckValue2 != hRPICom.RxBuffer[1]){//On envoie une erreur à la raspberry
		hRPICom.errorNumberRx ='1';
	}
	// Check Length
	if (UART_RPI_RX_BUFFER_SIZE != hRPICom.RxBuffer[2]){// Il nous manque des informations
		hRPICom.errorNumberRx='2';
	}
	hRPICom.binaryMessage.thrust = hRPICom.RxBuffer[3];
	hRPICom.binaryMessage.angle = hRPICom.RxBuffer[4];
	hRPICom.binaryMessage.depth = hRPICom.RxBuffer[5];
	hRPICom.binaryMessage.paquetNumber = hRPICom.RxBuffer[6];

	//Checksum
	if(checkSum(hRPICom.RxBuffer,UART_RPI_RX_BUFFER_SIZE) !=  hRPICom.RxBuffer[7]){
		hRPICom.errorNumberRx='3';
	}

	//Response RPI
	uint8_t stringLength = snprintf((char *)hRPICom.DebugBuffer, UART_RPI_DEBUG_BUFFER_SIZE, "[RECEPTION]\nPaquet: %u\r\nThrust: %u\nAngle: %u\r\nStatut: %c\r\n", hRPICom.binaryMessage.paquetNumber,hRPICom.binaryMessage.thrust,hRPICom.binaryMessage.angle,hRPICom.errorNumberRx);
	hRPICom.DebugBuffer[UART_RPI_DEBUG_BUFFER_SIZE-1] = 0; //Securité de print
	HAL_UART_Transmit(hRPICom.huartDebug, hRPICom.DebugBuffer, stringLength, 10);


	// Vous avez maintenant un message complet dans la structure BinaryMessage
	// Vous pouvez faire quelque chose avec ces données, par exemple, appeler une fonction de traitement

	if (hRPICom.errorNumberRx =='0'){
		Process(&hRPICom.binaryMessage);
	}
	RPICom_UartActivate(&hRPICom);
}
void acquireData(void){ //Factoriser surement dans un fichier AcquireData
	//interroge inertielle
	 hRPICom.sendMessage.posX = 10;
	 hRPICom.sendMessage.posY = 20;
	 hRPICom.sendMessage.angleDelta = 1;
	//interroge capteur temp
	 hRPICom.sendMessage.temp = 25;
	//interroge batterie
	 hRPICom.sendMessage.batteryPower = 90;
	 hRPICom.sendMessage.paquetNumber+=1;

}
void RPICom_SendBinaryMessage(void){ //Declencher par TIM3_IT ttes les 1s
	acquireData();
	hRPICom.TxBuffer[0]=HEADER_CheckValue1;
	hRPICom.TxBuffer[1]=HEADER_CheckValue2;
	hRPICom.TxBuffer[2]=UART_RPI_TX_BUFFER_SIZE;

	memcpy(hRPICom.TxBuffer + 3, &hRPICom.sendMessage, sizeof(hRPICom.sendMessage));

	hRPICom.TxBuffer[9]=checkSum(hRPICom.TxBuffer,sizeof(hRPICom.sendMessage)+3);

	HAL_UART_Transmit_IT(hRPICom.huart, hRPICom.TxBuffer, UART_RPI_TX_BUFFER_SIZE);
	//Response RPI
	uint8_t stringLength = snprintf((char *)hRPICom.DebugBuffer, UART_RPI_DEBUG_BUFFER_SIZE, "[SEND]\nPaquet: %u\r\nBatteryPower: %u\nPosition: %u-%u-%u°\r\nMessage transmis: %x\n", hRPICom.sendMessage.paquetNumber,hRPICom.sendMessage.batteryPower,hRPICom.sendMessage.posX,hRPICom.sendMessage.posY,hRPICom.sendMessage.angleDelta,hRPICom.sendMessage);
	hRPICom.DebugBuffer[UART_RPI_DEBUG_BUFFER_SIZE-1] = 0; //Securité de print
	HAL_UART_Transmit(hRPICom.huartDebug, hRPICom.DebugBuffer, stringLength, 10);
}

