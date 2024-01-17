/*
 * receive_message_Raspi.h
 *
 *  Created on: 16 janv. 2024
 *      Author: romingo
 */

#ifndef INC_RPICOM_H_
#define INC_RPICOM_H_

//#define UART_RX huart4
//#define UART_TX
//#define UART_TERMINAL huart3
#define UART_RPI_RX_BUFFER_SIZE 8
#define HEADER_CheckValue1 0xFF
#define HEADER_CheckValue2 0xFE
#include "usart.h"


typedef struct {

    uint8_t thrust;
    int8_t angle;
    uint16_t depth;
    uint8_t paquetNumber;
} commandMsg;

void RPICom_Init(void);
void RPICom_DecodeBinaryMessage(void);
void RPICom_UartActivate(void);
//void RPICom_Process(BinaryMessage* message);
//void RPICom_AdjustThrust(int8_t thrust);
//void RPICom_AdjustAngle(int8_t angle);

#endif /* INC_RPICOM_H_ */
