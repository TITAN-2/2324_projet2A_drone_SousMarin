/*
 * Process.h
 *
 *  Created on: Jan 17, 2024
 *      Author: romingo
 */

#ifndef INC_PROCESS_H_
#define INC_PROCESS_H_

#include "usart.h"
#include "RPICom.h"
#include "tim.h"

struct TIM_PROP{
	TIM_HandleTypeDef* TIM;
	HAL_TIM_ActiveChannel CHANNEL;
};

struct TIM_PROP TIM_PROP_G ={&htim2,HAL_TIM_ACTIVE_CHANNEL_1};
struct TIM_PROP TIM_PROP_D={&htim2,HAL_TIM_ACTIVE_CHANNEL_2};


void Thrust_Init(void);
void Process(commandMsg* message);
void Process_Init(void);
void AdjustThrust(struct TIM_PROP* tim_prop ,uint8_t thrust);
void AdjustAngle(int8_t angle);

#endif /* INC_PROCESS_H_ */
