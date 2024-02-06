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

#define MIN_Thrust 50
#define MAX_Thrust 100


struct TIM_PROP{
	TIM_HandleTypeDef* TIM;
	uint32_t CHANNEL;
};

struct TIM_PROP TIM_PROP_G ={&htim2,TIM_CHANNEL_1};
struct TIM_PROP TIM_PROP_D={&htim2,TIM_CHANNEL_2};


void Thrust_Init(void);
void Process(commandMsg* message);
void Process_Init(void);
void AdjustThrust(struct TIM_PROP* tim_prop ,uint8_t thrust);
void AdjustAngle(uint8_t thrust,int8_t angle);
#endif /* INC_PROCESS_H_ */
