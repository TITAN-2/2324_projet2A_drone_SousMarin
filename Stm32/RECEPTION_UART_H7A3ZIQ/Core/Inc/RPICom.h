/*
 * receive_message_Raspi.h
 *
 *  Created on: 16 janv. 2024
 *      Author: romingo
 */

#ifndef INC_RPICOM_H_
#define INC_RPICOM_H_

#include "usart.h"
#include "RPICom.h"

#define UART_RPI_RX_BUFFER_SIZE 8
#define UART_RPI_TX_BUFFER_SIZE 64
#define HEADER_CheckValue1 0xFF
#define HEADER_CheckValue2 0xFE


typedef struct {
    uint8_t thrust;
    int8_t angle;
    uint8_t depth;
    uint8_t paquetNumber;
} commandMsg;

typedef struct {
	UART_HandleTypeDef* huart;
	UART_HandleTypeDef* huartDebug;
	commandMsg binaryMessage;
	uint8_t errorNumber;
	uint8_t RxBuffer[UART_RPI_RX_BUFFER_SIZE];
	uint8_t TxBuffer[UART_RPI_TX_BUFFER_SIZE];
} RPICom_HandleTypeDef;

extern RPICom_HandleTypeDef hRPICom;

void RPICom_Init(UART_HandleTypeDef* huart, UART_HandleTypeDef* huartDebug);
void RPICom_DecodeBinaryMessage(void);
void RPICom_UartActivate(RPICom_HandleTypeDef* hRPI);


#endif /* INC_RPICOM_H_ */
