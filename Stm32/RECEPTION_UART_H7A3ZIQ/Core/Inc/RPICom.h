/*
 * RPICom.h
 *
 *  Created on: 16 janv. 2024
 *      Author: romingo
 */

#ifndef INC_RPICOM_H_
#define INC_RPICOM_H_

#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UART_RPI_RX_BUFFER_SIZE 8
#define UART_RPI_TX_BUFFER_SIZE 10
#define UART_RPI_DEBUG_BUFFER_SIZE 1024

#define HEADER_CheckValue1 0xFF
#define HEADER_CheckValue2 0xFE

typedef struct {
    uint8_t thrust;
    int8_t angle;
    uint8_t depth;
    uint8_t paquetNumber;
} receiveMsg;

typedef struct {
	uint8_t posX;
	uint8_t posY;
	uint8_t angleDelta;
	uint8_t temp;
	uint8_t batteryPower;
	uint8_t paquetNumber;
}sendMsg;

typedef struct {
	UART_HandleTypeDef* huart;
	UART_HandleTypeDef* huartDebug;

	receiveMsg 	receiveMessage;
	sendMsg 	sendMessage;
	uint8_t errorNumberRx;
	uint8_t RxBuffer[UART_RPI_RX_BUFFER_SIZE];
	uint8_t TxBuffer[UART_RPI_TX_BUFFER_SIZE];
	uint8_t DebugBuffer[UART_RPI_DEBUG_BUFFER_SIZE];
} RPICom_HandleTypeDef;

extern RPICom_HandleTypeDef hRPICom;

void RPICom_Init(UART_HandleTypeDef* huart, UART_HandleTypeDef* huartDebug);
void RPICom_DecodeBinaryMessage(void);
void RPICom_UartActivate(RPICom_HandleTypeDef* hRPI);
void acquireData(void);
int randomize(int min, int max);
void RPICom_SendBinaryMessage(void);

#endif /* INC_RPICOM_H_ */
