//******************************** PARSER ****************************************
// Copyright (c) 2025
// All Rights Reserved
//*********************************************************************************
//
// File		: Parser.c
// Summary	: Parsing functions
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "Parser.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************

//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************

//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.******************************
//Purpose : To parse the recieved expresion and calculate 
//Inputs  : ucBuffer - Recieved buffer from client
//Outputs : None
//Return  : unResult - Calculated result
//**********************************************************************************
eMathError ParseStringAndCalculate(uint8_t ucBuffer[], uint32_t *ulFinalResult)
{
    uint32_t ulResult = 0;
    uint32_t unNumber = 0;
    char operator = '+';  // Default operator value
    uint8_t ucIndex = 0;
    uint8_t ucCorrectExprIndex = 0;
    uint8_t ucCorrectExpr[20] = {0};
    uint8_t ucbufferIndex = 0;
    eMathError eError = MATH_OK;


    while (ucBuffer[ucbufferIndex] != '\0' && ucCorrectExprIndex < 20) {
        if ((ucBuffer[ucbufferIndex] >= '0' && ucBuffer[ucbufferIndex] <= '9') ||  
            ucBuffer[ucbufferIndex] == '+' || ucBuffer[ucbufferIndex] == '-' || 
            ucBuffer[ucbufferIndex] == '*' || ucBuffer[ucbufferIndex] == '/' ||
            ucBuffer[ucbufferIndex] == '%' || ucBuffer[ucbufferIndex] == '>' ||
            ucBuffer[ucbufferIndex] == '<' || ucBuffer[ucbufferIndex] == '&' ||
            ucBuffer[ucbufferIndex] == '|' || ucBuffer[ucbufferIndex] == '^' ||
            ucBuffer[ucbufferIndex] == '~' || ucBuffer[ucbufferIndex] == '$' ||
            ucBuffer[ucbufferIndex] == '#') {
            ucCorrectExpr[ucCorrectExprIndex ++] = ucBuffer[ucbufferIndex];
        }
        ucbufferIndex ++;
    }
    ucCorrectExpr[ucCorrectExprIndex] = '\0';
    
    // Parsing first number
    while (ucCorrectExpr[ucIndex] >= '0' && ucCorrectExpr[ucIndex] <= '9') {
        if(unNumber > UINT32_MAX/10)
        {
            LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
            return MATH_OVERFLOW;
        }

        unNumber = unNumber * 10 + (ucCorrectExpr[ucIndex] - '0');
        ucIndex++;
    }
    ulResult = unNumber;
    
    while (ucCorrectExpr[ucIndex] != '\0') {
        // Get operator
        operator = ucCorrectExpr[ucIndex];
        ucIndex++;
        
        // Get next number
        unNumber = 0;
        while (ucCorrectExpr[ucIndex] >= '0' && ucCorrectExpr[ucIndex] <= '9') {
            if(unNumber > UINT32_MAX/10)
            {
                LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                return MATH_OVERFLOW;
            }
            unNumber = unNumber * 10 + (ucCorrectExpr[ucIndex] - '0');
            ucIndex++;
        }

        switch (operator) {
            case '+':
                if(ulResult > UINT32_MAX - unNumber )
                {
                    LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                    eError = MATH_OVERFLOW;
                }
                else
                {
                    ulResult += unNumber;
                }
                break;
            case '-': 
                ulResult -= unNumber; 
                break;
            case '*': 
                if(unNumber != 0 && ulResult > UINT32_MAX / unNumber)
                {
                    LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                    eError = MATH_OVERFLOW;
                }
                ulResult *= unNumber; 
                break;
            case '/': 
                if (unNumber != 0) 
                {
                    ulResult /= unNumber;  
                }
                else
                {
                    eError = MATH_DIVISION_BY_ZERO;
                }
                break;
            case '%':
                if (unNumber != 0) 
                {
                    ulResult %= unNumber;
                }
                else
                {
                    eError = MATH_DIVISION_BY_ZERO;
                }
                break;
            case '<':
                printf("Left shift\n");
                ulResult <<= unNumber;
                break;
            case '>':
                printf("right shift\n");
                ulResult >>= unNumber;
                break;
            case '&':
                ulResult &= unNumber;
                break;
            case '|':
                ulResult |= unNumber;
                break;
            case '^':
                ulResult ^= unNumber;
                break;
            case '~':
                ulResult = ~ulResult;
                break;
            case '$':
                printf("NAND\n");
                ulResult = ~(ulResult & unNumber);
                break;
            case '#':
                printf("NOR\n");
                ulResult = ~(ulResult | unNumber);
                break;
            default: 
                return -1;  
        }
    }
    *ulFinalResult = ulResult;
    return eError; 
}



