#include "RPICom.h"

RPICom_HandleTypeDef hRPICom;
void RPICom_Init(UART_HandleTypeDef* huart, UART_HandleTypeDef* huartDebug){
	hRPICom.huart = huart;
	hRPICom.huartDebug = huartDebug;
	memcpy(&(hRPICom.receiveMessage), NULL, sizeof(receiveMsg));
	memcpy(hRPICom.RxBuffer, NULL , UART_RPI_RX_BUFFER_SIZE);
	memcpy(hRPICom.TxBuffer, NULL, UART_RPI_TX_BUFFER_SIZE);
	memcpy(hRPICom.DebugBuffer, NULL, UART_RPI_DEBUG_BUFFER_SIZE);
	hRPICom.sendMessage.paquetNumber = 0;
	hRPICom.errorNumberRx = 0;

	RPICom_UartActivate(&hRPICom);
}

void RPICom_UartActivate(RPICom_HandleTypeDef* hRPI){
	HAL_UART_Receive_IT(hRPI->huart, hRPI->RxBuffer, UART_RPI_RX_BUFFER_SIZE);
}

uint8_t checkSum(uint8_t buffer[],uint8_t buffer_size){
	uint8_t checkSum = 0;
	for (int i = 0; i < buffer_size-1; i++) { //Il ne faut pas prendre en compte le bit ou checkSum est present
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
	hRPICom.receiveMessage.thrust = hRPICom.RxBuffer[3];
	hRPICom.receiveMessage.angle = hRPICom.RxBuffer[4];
	hRPICom.receiveMessage.depth = hRPICom.RxBuffer[5];
	hRPICom.receiveMessage.paquetNumber = hRPICom.RxBuffer[6];

	//Checksum
	if(checkSum(hRPICom.RxBuffer,UART_RPI_RX_BUFFER_SIZE) !=  hRPICom.RxBuffer[7]){
		hRPICom.errorNumberRx='3';
	}

	//Response RPI
	uint8_t stringLength = snprintf((char *)hRPICom.DebugBuffer, UART_RPI_DEBUG_BUFFER_SIZE, "[RECEPTION]\nPaquet: %u\r\nThrust: %u\nAngle: %u\r\nStatut: %c\r\n", hRPICom.receiveMessage.paquetNumber,hRPICom.receiveMessage.thrust,hRPICom.receiveMessage.angle,hRPICom.errorNumberRx);
	HAL_UART_Transmit(hRPICom.huartDebug, hRPICom.DebugBuffer, stringLength, 10);


	// Vous avez maintenant un message complet dans la structure receiveMessage
	// Vous pouvez faire quelque chose avec ces données, par exemple, appeler une fonction de traitement

	if (hRPICom.errorNumberRx =='0'){
		Process(&hRPICom.receiveMessage);
	}
	RPICom_UartActivate(&hRPICom);
}
void acquireData(void){ //Factoriser surement dans un fichier AcquireData
	//interroge inertielle
	 hRPICom.sendMessage.posX = randomize(0,100);
	 hRPICom.sendMessage.posY = randomize(0,100);
	 hRPICom.sendMessage.angleDelta = 1;
	//interroge capteur temp
	 hRPICom.sendMessage.temp = randomize(15,30);
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
	/**
	uint8_t stringLength = snprintf((char *)hRPICom.DebugBuffer, UART_RPI_DEBUG_BUFFER_SIZE, "[SEND]\n\rPaquet:\t%u\n\rBatteryPower:\t%u\n\rPosition:\t%u-%u-%u°\n\r",hRPICom.sendMessage.paquetNumber,hRPICom.sendMessage.batteryPower,hRPICom.sendMessage.posX,hRPICom.sendMessage.posY,hRPICom.sendMessage.angleDelta);
	HAL_UART_Transmit(hRPICom.huartDebug, hRPICom.DebugBuffer, stringLength, 10);
	 **/
}

int randomize(int min, int max) {
	srand(time(NULL));
    return min + rand() % (max - min + 1);
}
