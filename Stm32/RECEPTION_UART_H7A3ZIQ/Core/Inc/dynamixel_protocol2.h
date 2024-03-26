//***********************************************
// XL320_STM32
// Copyright (c) 2022 Ricardo SILVESTRE.
// Github @ricardossilvestre.
// All rights reserved.
//***********************************************

#ifndef STM32_SERVO_TEST_DYNAMIXEL_PROTOCOL2_H
#define STM32_SERVO_TEST_DYNAMIXEL_PROTOCOL2_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum DXL_ID{
	DXL_BROADCAST_ID =      0xFE,
	DXL_DEFAULT_ID =		0x01
};

enum DXLProtocol{
	DXL2_0_PACKET_IDX_HEADER_1 =        0xFF,	//0
	DXL2_0_PACKET_IDX_HEADER_2 =        0xFF,	//1
	DXL2_0_PACKET_IDX_HEADER_3 =        0xFD,	//2
	DXL2_0_PACKET_IDX_RESERVED =        0x00	//3
	//DXL2_0_PACKET_IDX_ID =                    //4
	//DXL2_0_PACKET_IDX_LENGTH_L =              //5
	//DXL2_0_PACKET_IDX_LENGTH_H =              //6
	//DXL2_0_PACKET_IDX_INST =                  //7
	//DXL2_0_PACKET_IDX_ERROR =                	//8
	//DXL2_0_PACKET_IDX_INST_PARAM =           	//8
	//DXL2_0_PACKET_IDX_STATUS_PARAM =          //9
};

enum DXLInstruction{
	DXL_INST_PING = 			0x01,
	DXL_INST_READ = 			0x02,
	DXL_INST_WRITE = 			0x03,
	DXL_INST_REG_WRITE = 		0x04,
	DXL_INST_ACTION = 			0x05,
	DXL_INST_FACTORY_RESET = 	0x06,
	DXL_INST_REBOOT = 			0x08,
	DXL_INST_CLEAR = 			0x10,
	DXL_INST_STATUS = 			0x55,
	DXL_INST_SYNC_READ = 		0x82,
	DXL_INST_FAST_SYNC_READ = 	0x8A,
	DXL_INST_SYNC_WRITE = 		0x83,
	DXL_INST_BULK_READ = 		0x92,
	DXL_INST_BULK_WRITE = 		0x93
};

#define MAXNUM_TXPACKET     (255)//(65535)
#define MAXNUM_RXPACKET     (255)//(65535)

///////////////// utility for value ///////////////////////////
#define DXL_MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned char)(((unsigned long)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b)     ((unsigned int)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define DXL_HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define DXL_HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))


#define RX_TIMEOUT_COUNT2		(1600L) //(1000L) //porting
#define NANO_TIME_DELAY			(12000) //ydh added 20111228 -> 20120210 edited ydh
#define RX_TIMEOUT_COUNT1  		(RX_TIMEOUT_COUNT2*128L)

uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, uint16_t data_blk_size);

#endif //STM32_SERVO_TEST_DYNAMIXEL_PROTOCOL2_H