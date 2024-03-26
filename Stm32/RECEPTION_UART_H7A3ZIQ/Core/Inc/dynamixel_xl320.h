//***********************************************
// XL320_STM32
// Copyright (c) 2022 Ricardo SILVESTRE.
// Github @ricardossilvestre.
// All rights reserved.
//***********************************************

#ifndef STM32_SERVO_TEST_DYNAMIXEL_XL320_H
#define STM32_SERVO_TEST_DYNAMIXEL_XL320_H

#include "dynamixel_protocol2.h"
#include "main.h"

// TxPacket format {0xFF, 0xFF, 0xFD, 0x00, ID, Len_L, Len_H, Instruction, Param 1, ..., Param N, CRC_L, CRC_H};"

enum XL320_EEPROM {
XL_MODEL_NUMBER_L          = 0,
XL_MODEL_NUMBER_H          = 1,
XL_VERSION                 = 2,
XL_ID                      = 3,
XL_BAUD_RATE               = 4,
XL_RETURN_DELAY_TIME       = 5,
XL_CW_ANGLE_LIMIT_L        = 6,
XL_CW_ANGLE_LIMIT_H        = 7,
XL_CCW_ANGLE_LIMIT_L       = 8,
XL_CCW_ANGLE_LIMIT_H       = 9,
XL_CONTROL_MODE            = 11,
XL_LIMIT_TEMPERATURE       = 12,
XL_DOWN_LIMIT_VOLTAGE      = 13,
XL_UP_LIMIT_VOLTAGE        = 14,
XL_MAX_TORQUE_L            = 15,
XL_MAX_TORQUE_H            = 16,
XL_RETURN_LEVEL            = 17,
XL_ALARM_SHUTDOWN          = 18
};

enum XL320_RAM {
XL_TORQUE_ENABLE           = 24,
XL_LED                     = 25,
XL_D_GAIN                  = 27,
XL_I_GAIN                  = 28,
XL_P_GAIN                  = 29,
XL_GOAL_POSITION_L         = 30,
XL_GOAL_SPEED_L            = 32,
XL_GOAL_TORQUE             = 35,
XL_PRESENT_POSITION        = 37,
XL_PRESENT_SPEED           = 39,
XL_PRESENT_LOAD            = 41,
XL_PRESENT_VOLTAGE         = 45,
XL_PRESENT_TEMPERATURE     = 46,
XL_REGISTERED_INSTRUCTION  = 47,
XL_MOVING                  = 49,
XL_HARDWARE_ERROR          = 50,
XL_PUNCH                   = 51
};

#define Tx_MODE                     1
#define Rx_MODE                     0

/*	red		1
	green	2
	blue	4
	yellow	3
	cyan	6
	purple	5
	white	7*/
void XL320_set_led_ON(UART_HandleTypeDef *m_huart, uint8_t color);

void XL320_set_led_OFF(UART_HandleTypeDef *m_huart);

void XL320_set_pos(UART_HandleTypeDef *m_huart, uint16_t pos);

uint16_t XL320_read_load(UART_HandleTypeDef *m_huart);

#endif //STM32_SERVO_TEST_DYNAMIXEL_XL320_H
