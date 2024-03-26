//***********************************************
// XL320_STM32
// Copyright (c) 2022 Ricardo SILVESTRE.
// Github @ricardossilvestre.
// All rights reserved.
//***********************************************

#include "../Inc/dynamixel_xl320.h"

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max){
	return ((x - in_min) * (out_max - out_min)) / ((in_max - in_min) + out_min);
}

void DMA_receive_delay(){  //verify a good value for the delay
	HAL_Delay(1000);
}

/*	red		1
	green	2
	blue	4
	yellow	3
	cyan	6
	purple	5
	white	7*/
void XL320_set_led_ON(UART_HandleTypeDef *m_huart, uint8_t color){
	uint8_t TxPacket[13] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_BROADCAST_ID,
							0x06,
							0x00,
							DXL_INST_WRITE,
							DXL_LOBYTE(XL_LED),
							DXL_HIBYTE(XL_LED),
							color,
							0,
							0};
	uint16_t CRC_2 = update_crc(0, TxPacket, 11);
	TxPacket[11] = DXL_LOBYTE(CRC_2);
	TxPacket[12] = DXL_HIBYTE(CRC_2);

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 13, HAL_MAX_DELAY);
}

void XL320_set_led_OFF(UART_HandleTypeDef *m_huart){
	uint8_t TxPacket[13] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_BROADCAST_ID,
							0x06,
							0x00,
							DXL_INST_WRITE,
							DXL_LOBYTE(XL_LED),
							DXL_HIBYTE(XL_LED),
							0x00,
							0,
							0};
	uint16_t CRC_2 = update_crc(0, TxPacket, 11);
	TxPacket[11] = DXL_LOBYTE(CRC_2);
	TxPacket[12] = DXL_HIBYTE(CRC_2);

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 13, HAL_MAX_DELAY);
}

void XL320_set_pos(UART_HandleTypeDef *m_huart, uint16_t pos){
	const uint16_t POS_MAX = 120; //CHANGE THIS CONSTANT TO AVOID OVERCURRENT IN THE SERVO
	if(pos>POS_MAX) pos = POS_MAX;
	uint16_t pos_mapped = map(pos, 0, 300, 0, 1023);
	uint8_t TxPacket[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							 DXL2_0_PACKET_IDX_HEADER_2,
							 DXL2_0_PACKET_IDX_HEADER_3,
							 DXL2_0_PACKET_IDX_RESERVED,
							 DXL_BROADCAST_ID,
							 0x07,
							 0x00,
							 DXL_INST_WRITE,
							 DXL_LOBYTE(XL_GOAL_POSITION_L),
							 DXL_HIBYTE(XL_GOAL_POSITION_L),
							 DXL_LOBYTE(pos_mapped),
							 DXL_HIBYTE(pos_mapped),
							 0,
							 0};
	uint16_t CRC_2 = update_crc(0, TxPacket, 12);
	TxPacket[12] = DXL_LOBYTE(CRC_2);
	TxPacket[13] = DXL_HIBYTE(CRC_2);

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 14, HAL_MAX_DELAY);
}

void XL320_read_moving(UART_HandleTypeDef *m_huart){
	uint8_t RxPacket[13] = {0};

	uint8_t TxPacket[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_DEFAULT_ID,
							0x07,
							0x00,
							DXL_INST_READ,
							0x2E,
							0x00,
							0x01,
							0x00,
							0,
							0};
	uint16_t CRC_2 = update_crc(0, TxPacket, 12);
	TxPacket[12] = DXL_LOBYTE(CRC_2);
	TxPacket[13] = DXL_HIBYTE(CRC_2);

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 14, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 13);
	DMA_receive_delay();
}

uint16_t XL320_read_load(UART_HandleTypeDef *m_huart){
	uint8_t RxPacket[14] = {0};

	uint8_t TxPacket[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_DEFAULT_ID,
							0x07,
							0x00,
							DXL_INST_READ,
							0x29,
							0x00,
							0x02,
							0x00,
							0,
							0};
	uint16_t CRC_2 = update_crc(0, TxPacket, 12);
	TxPacket[12] = DXL_LOBYTE(CRC_2);
	TxPacket[13] = DXL_HIBYTE(CRC_2);

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 14, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 14);
	DMA_receive_delay();

	uint16_t present_load = (uint16_t) (RxPacket[11] << 8) | (RxPacket[10]);
	uint16_t load_ratio = present_load & 0b0000001111111111;

	return load_ratio;
}

void XL320_ping(UART_HandleTypeDef *m_huart){
	uint8_t RxPacket[100] = {0};

	uint8_t TxPacket3[10] = {DXL2_0_PACKET_IDX_HEADER_1,
							 DXL2_0_PACKET_IDX_HEADER_2,
							 DXL2_0_PACKET_IDX_HEADER_3,
							 DXL2_0_PACKET_IDX_RESERVED,
							 DXL_BROADCAST_ID,
							 0x03,
							 0x00,
							 DXL_INST_PING,
							 0x31,
							 0x42};

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket3, 10, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 100);
	DMA_receive_delay();
}