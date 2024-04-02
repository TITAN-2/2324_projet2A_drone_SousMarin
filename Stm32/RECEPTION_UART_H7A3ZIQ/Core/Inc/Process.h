/*
 * Process.h
 *
 *  Created on: Jan 17, 2024
 *      Author: romingo
 */

#ifndef INC_PROCESS_H_
#define INC_PROCESS_H_

#include "RPICom.h"
#include "tim.h"
#include "stm32h7xx_hal_def.h"
#include "usart.h"
#include "dynamixel_protocol2.h"
#include "dynamixel_xl320.h"

#define MIN_Thrust 100
#define MAX_Thrust 50

#define RACK_SIZE_mm 500
#define SPEED_XL_rads 2
#define GEAR_RADIUS_mm 200

typedef struct {
	TIM_HandleTypeDef* TIM;
	uint32_t CHANNEL;
}TIM_PROP;

typedef struct {
	UART_HandleTypeDef* huartXL;
	uint8_t id;
	uint16_t pos;
}XL_DEF;
typedef struct{
	XL_DEF* XL_Front;
	XL_DEF*	XL_Back;
}XL_Network;

TIM_PROP TIM_PROP_G = {&htim2,TIM_CHANNEL_1};
TIM_PROP TIM_PROP_D = {&htim2,TIM_CHANNEL_3};
XL_DEF XL_Front  = {NULL,1,0};
XL_DEF XL_Back = {&huart2,2,0};
XL_Network XL_Net = {&XL_Front,&XL_Back};
void Thrust_Init(void);
void Depth_Init(void);
void Process(receiveMsg* message);
void Process_Init(void);
void AdjustThrust(TIM_PROP* tim_prop ,uint8_t thrust);
void AdjustAngle(uint8_t thrust,int8_t angle);
void AdjustDepth(XL_Network* XL_Net,uint8_t depth);
#endif /* INC_PROCESS_H_ */
