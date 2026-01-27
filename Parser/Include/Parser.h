//**************************** PARSER *****************************
//  Copyright (c) 2025
//  All Rights Reserved
//*****************************************************************************
//
// File		: Parser.h
// Summary	: This configurations header file contains structure and function declarations
//		       for user input and server thread
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*****************************************************************************

#ifndef INC_PARSER_H_
#define INC_PARSER_H_

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

//******************************* Global Types ********************************

//***************************** Global Constants ******************************

//***************************** Global Structure ******************************
typedef enum _eMathError{
    MATH_OK = 0,
    MATH_OVERFLOW,
    MATH_DIVISION_BY_ZERO
} eMathError;
//**************************** Forward Declarations ***************************

//***************************** Function Declaration **************************
eMathError ParseStringAndCalculate(uint8_t ucBuffer[], uint32_t *ulFinalResult);
//*********************** Inline Method Implementations ***********************

#endif /* //*********************** Inline Method Implementations ***********************

#endif /* INC_PARSER_H_ */