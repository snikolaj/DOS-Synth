#pragma once
#include "eth_driver.h"
#include "string.h"
#include "debug.h"
#include "WCHNET.h"

#define KEEPLIVE_ENABLE         1                           //Enable keeplive function

extern u8 MACAddr[6];                                              //MAC address
extern u8 IPAddr[4];                         //IP address
extern u8 GWIPAddr[4];                        //Gateway IP address
extern u8 IPMask[4];                        //subnet mask
extern u8 DESIP[4];                         //destination IP address
extern u16 desport;                                         //destination port
extern u16 srcport;                                         //source port

extern u8 SocketId;
extern u8 socket[WCHNET_MAX_SOCKET_NUM];                           //Save the currently connected socket
extern u8 SocketRecvBuf[WCHNET_MAX_SOCKET_NUM][RECE_BUF_LEN];      //socket receive buffer

extern void (*ETHReceiveHandler)(uint8_t);

void WCHNET_CreateTcpSocket(void);
void WCHNET_DataLoopback(u8 id);
void WCHNET_HandleSockInt(u8 socketid, u8 intstat);
void WCHNET_HandleGlobalInt(void);
void mStopIfError(u8 iError);
void Set_Receive_Handler(void (* InterruptHandler)(uint8_t));
