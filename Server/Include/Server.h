//**************************** SERVER *****************************
//  Copyright (c) 2025
//  All Rights Reserved
//*****************************************************************************
//
// File		: Server.h
// Summary	: This configurations header file contains structure and function declarations
//		       for socket communication
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*****************************************************************************

#ifndef INC_SERVER_H_
#define INC_SERVER_H_

//******************************* Include Files *******************************
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include "Log.h"
#include "Parser.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define PORT 8080

//***************************** Global Structure ******************************
typedef enum _eReadError{
    READ_OK = 0,
    READ_FAILED,
    READ_CLIENT_DISCONNECTED
} eReadError;
//**************************** Forward Declarations ***************************

//***************************** Function Declaration **************************
bool ServerCreateSocket(uint8_t *ucServerFD);
bool ServerAttachSocket(uint8_t ucServerFD, uint8_t *ucOpt);
bool ServerBind(uint8_t ucServerFD);
bool ServerListen(uint8_t ucServerFD, uint8_t ucNumberOfConnection);
bool ServerAccept(uint8_t ucServerFD, uint8_t *ucNewSocket);
eReadError SocketRead(uint8_t ucNewSocket);
void SocketWrite(uint8_t ucNewSocket, uint8_t ucTransmitBuffer[], uint8_t ucSizeOfBuffer);
void SocketClose(uint8_t ucSocketFD);
//*********************** Inline Method Implementations ***********************

#endif /* INC_SERVER_H_ */
