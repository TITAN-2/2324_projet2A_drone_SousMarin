/*
 * Process.c
 *
 *  Created on: Jan 17, 2024
 *      Author: romingo
 */

#include "Process.h"

void Thrust_Init(void){
	HAL_TIM_PWM_Start(&TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL);
	HAL_TIM_PWM_Start(&TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL);
	__HAL_TIM_SET_COMPARE(&TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL, 50);
	__HAL_TIM_SET_COMPARE(&TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL, 50);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(&TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL, 100);
	__HAL_TIM_SET_COMPARE(&TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL, 100);
	HAL_Delay(1000);
}
void Process_Init(void){
	Thrust_Init();
}

void Process(commandMsg* message){
	AdjustThrust(TIM_PROP_G,message->thrust);
	AdjustAngle(message->angle);

	HAL_UART_Transmit(&huart3,&message->thrust,8,10); //receive data from data buffer interrupt mode
	HAL_UART_Transmit(&huart3,&message->angle,16,10);
}

void AdjustThrust(struct TIM_PROP tim_prop ,uint8_t thrust){

}

void AdjustAngle(int8_t angle){
	uint8_t thrust_G = angle*1/90;
	uint8_t thrust_D = angle*-1/90;
	AdjustThrust(TIM_PROP_G,thrust_G);
	AdjustThrust(TIM_PROP_D,thrust_D);

}
