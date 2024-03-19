/*
 * Process.c
 *
 *  Created on: Jan 17, 2024
 *      Author: romingo
 */

#include "Process.h"

void Thrust_Init(void){

	HAL_TIM_PWM_Start(TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL);
	HAL_TIM_PWM_Start(TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL, MIN_Thrust);
	__HAL_TIM_SET_COMPARE(TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL, MIN_Thrust);
	HAL_Delay(1000);

	__HAL_TIM_SET_COMPARE(TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL, MAX_Thrust);
	__HAL_TIM_SET_COMPARE(TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL, MAX_Thrust);

	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(TIM_PROP_G.TIM, TIM_PROP_G.CHANNEL, MIN_Thrust);
	__HAL_TIM_SET_COMPARE(TIM_PROP_D.TIM, TIM_PROP_D.CHANNEL, MIN_Thrust);
	HAL_Delay(1000);
}
void Process_Init(void){
	Thrust_Init();
}

void Process(commandMsg* message) {
    AdjustThrust(&TIM_PROP_G, message->thrust);
    AdjustThrust(&TIM_PROP_D, message->thrust);
    AdjustAngle(message->thrust,message->angle);
}

void AdjustThrust(struct TIM_PROP* tim_prop, uint8_t thrust) {
	float thrust_ms = 1-(thrust * ((float)(MIN_Thrust - MAX_Thrust) / 100.0)) + MIN_Thrust;
    __HAL_TIM_SET_COMPARE(tim_prop->TIM, tim_prop->CHANNEL,thrust_ms);
}

void AdjustAngle(uint8_t thrust,int8_t angle){
	uint8_t thrust_G = thrust;
	uint8_t thrust_D = thrust;

	if (angle<50){//On tourne a gauche
		thrust_G = thrust;
		thrust_D = 0; // Ajouter une diminution progressive des pousses moteurs
	}
	if (angle>50){//On tourne a droite
		thrust_G = 0;
		thrust_D = thrust;
	}
	AdjustThrust(&TIM_PROP_G, thrust_G);
	AdjustThrust(&TIM_PROP_D, thrust_D);
}
