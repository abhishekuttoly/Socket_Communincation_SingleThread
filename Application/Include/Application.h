//**************************** APPLICATION *****************************
//  Copyright (c) 2025
//  All Rights Reserved
//*****************************************************************************
//
// File		: Application.h
// Summary	: This configurations header file contains structure and function declarations
//		       for user input and server thread
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*****************************************************************************

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

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
#include "Server.h"
#include "Parser.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************

//***************************** Global Structure ******************************


//**************************** Forward Declarations ***************************

//***************************** Function Declaration **************************
void *ApplicationServerThread(void *arg);
//*********************** Inline Method Implementations ***********************

//*********************** Inline Method Implementations ***********************

#endif /* INC_APPLICATION_H_ */