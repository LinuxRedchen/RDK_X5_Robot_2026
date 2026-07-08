#ifndef _ONENET_H_
#define _ONENET_H_


#include "stdint.h"


_Bool OneNET_RegisterDevice(void);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

void UART2_Send_String(uint8_t *str);
void Onenet_Date_trans(
    unsigned char fan,    // 风扇状态（0/1）
    int HotCold,              // 光照强度（整数）
    unsigned char status,  // 灯光状态（0/1）
    unsigned char temp,   // 温度（整数）
    unsigned char humi    // 湿度（整数）
);

#endif
