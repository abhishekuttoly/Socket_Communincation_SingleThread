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
// Purpose : To parse the recieved expresion and calculate
// Inputs  : ucBuffer - Recieved buffer from client
// Outputs : None
// Return  : unResult - Calculated result
//**********************************************************************************
eMathError ParseStringAndCalculate(uint8_t ucBuffer[], int32_t *lFinalResult) // Function used when expected a char string as received message
{
    int32_t lResult = 0;
    int32_t nNumber = 0;
    char operator = '+'; // Default operator value
    uint8_t ucIndex = 0;
    uint8_t ucCorrectExprIndex = 0;
    uint8_t ucCorrectExpr[20] = {0};
    uint8_t ucbufferIndex = 0;
    eMathError eError = MATH_OK;

    while (ucBuffer[ucbufferIndex] != '\0' && ucCorrectExprIndex < 20)
    {
        if ((ucBuffer[ucbufferIndex] >= '0' && ucBuffer[ucbufferIndex] <= '9') ||
            ucBuffer[ucbufferIndex] == '+' || ucBuffer[ucbufferIndex] == '-' ||
            ucBuffer[ucbufferIndex] == '*' || ucBuffer[ucbufferIndex] == '/' ||
            ucBuffer[ucbufferIndex] == '%' || ucBuffer[ucbufferIndex] == '>' ||
            ucBuffer[ucbufferIndex] == '<' || ucBuffer[ucbufferIndex] == '&' ||
            ucBuffer[ucbufferIndex] == '|' || ucBuffer[ucbufferIndex] == '^' ||
            ucBuffer[ucbufferIndex] == '~' || ucBuffer[ucbufferIndex] == '$' ||
            ucBuffer[ucbufferIndex] == '#')
        {
            ucCorrectExpr[ucCorrectExprIndex++] = ucBuffer[ucbufferIndex];
        }
        ucbufferIndex++;
    }
    ucCorrectExpr[ucCorrectExprIndex] = '\0';

    // Parsing first number
    while (ucCorrectExpr[ucIndex] >= '0' && ucCorrectExpr[ucIndex] <= '9')
    {
        if (nNumber > INT32_MAX / 10)
        {
            LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
            return MATH_OVERFLOW;
        }

        nNumber = nNumber * 10 + (ucCorrectExpr[ucIndex] - '0');
        ucIndex++;
    }
    lResult = nNumber;

    while (ucCorrectExpr[ucIndex] != '\0')
    {
        // Get operator
        operator = ucCorrectExpr[ucIndex];
        ucIndex++;

        // Get next number
        nNumber = 0;
        while (ucCorrectExpr[ucIndex] >= '0' && ucCorrectExpr[ucIndex] <= '9')
        {
            if (nNumber > INT32_MAX / 10)
            {
                LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                return MATH_OVERFLOW;
            }
            nNumber = nNumber * 10 + (ucCorrectExpr[ucIndex] - '0');
            ucIndex++;
        }

        switch (operator)
        {
        case '+':
            if (lResult > INT32_MAX - nNumber)
            {
                LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                eError = MATH_OVERFLOW;
            }
            else
            {
                lResult += nNumber;
            }
            break;
        case '-':
            lResult -= nNumber;
            break;
        case '*':
            if (nNumber != 0 && lResult > INT32_MAX / nNumber)
            {
                LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Overflow occured");
                eError = MATH_OVERFLOW;
            }
            lResult *= nNumber;
            break;
        case '/':
            if (nNumber != 0)
            {
                lResult /= nNumber;
            }
            else
            {
                eError = MATH_DIVISION_BY_ZERO;
            }
            break;
        case '%':
            if (nNumber != 0)
            {
                lResult %= nNumber;
            }
            else
            {
                eError = MATH_DIVISION_BY_ZERO;
            }
            break;
        case '<':
            printf("Left shift\n");
            lResult <<= nNumber;
            break;
        case '>':
            printf("right shift\n");
            lResult >>= nNumber;
            break;
        case '&':
            lResult &= nNumber;
            break;
        case '|':
            lResult |= nNumber;
            break;
        case '^':
            lResult ^= nNumber;
            break;
        case '~':
            lResult = ~lResult;
            break;
        case '$':
            printf("NAND\n");
            lResult = ~(lResult & nNumber);
            break;
        case '#':
            printf("NOR\n");
            lResult = ~(lResult | nNumber);
            break;
        default:
            return -1;
        }
    }
    *lFinalResult = lResult;

    return eError;
}

eMathError ParseCalculateData(struct _sExpression sExpressionData, int32_t *lCalculatedResult)
{
    int32_t lResult = 0;
    eMathError eError = MATH_OK;

    if ((sExpressionData.ucOperator == '+' || sExpressionData.ucOperator == '-' ||
         sExpressionData.ucOperator == '*' || sExpressionData.ucOperator == '/' ||
         sExpressionData.ucOperator == '%' || sExpressionData.ucOperator == '>' ||
         sExpressionData.ucOperator == '<' || sExpressionData.ucOperator == '&' ||
         sExpressionData.ucOperator == '|' || sExpressionData.ucOperator == '^' ||
         sExpressionData.ucOperator == '~' || sExpressionData.ucOperator == '$' ||
         sExpressionData.ucOperator == '#' || sExpressionData.ucOperator == 'r' ||
         sExpressionData.ucOperator == 'B' || sExpressionData.ucOperator == 'x' ||
         sExpressionData.ucOperator == 'b'))
    {
        switch (sExpressionData.ucOperator)
        {
        case '+':
            lResult = sExpressionData.lOperandOne + sExpressionData.lOperandTwo;
            break;
        case '-':
            lResult = sExpressionData.lOperandOne - sExpressionData.lOperandTwo;
            break;
        case '*':
            lResult = sExpressionData.lOperandOne * sExpressionData.lOperandTwo;
            break;
        case '/':
            if (sExpressionData.lOperandTwo != 0)
            {
                lResult = sExpressionData.lOperandOne / sExpressionData.lOperandTwo;
            }
            else
            {
                eError = MATH_DIVISION_BY_ZERO;
            }
            break;
        case '%':
            if (sExpressionData.lOperandTwo != 0)
            {
                lResult = sExpressionData.lOperandOne % sExpressionData.lOperandTwo;
            }
            else
            {
                eError = MATH_DIVISION_BY_ZERO;
            }
            break;
        case '<':
            printf("Left shift\n");
            lResult = sExpressionData.lOperandOne << sExpressionData.lOperandTwo;
            break;
        case '>':
            printf("right shift\n");
            lResult = sExpressionData.lOperandOne >> sExpressionData.lOperandTwo;
            break;
        case '&':
            lResult = sExpressionData.lOperandOne & sExpressionData.lOperandTwo;
            break;
        case '|':
            lResult = sExpressionData.lOperandOne | sExpressionData.lOperandTwo;
            break;
        case '^':
            lResult = sExpressionData.lOperandOne ^ sExpressionData.lOperandTwo;
            break;
        case '~':
            lResult = ~sExpressionData.lOperandOne;
            break;
        case '$':
            printf("NAND\n");
            lResult = ~(sExpressionData.lOperandOne & sExpressionData.lOperandTwo);
            break;
        case '#':
            printf("NOR\n");
            lResult = ~(sExpressionData.lOperandOne | sExpressionData.lOperandTwo);
            break;
        case 'B':
        case 'b':
            uint8_t ucBinaryNumber[20];
            uint8_t ucArrayIndex= 0;
            int ucLoopIndex = 0;
            while(sExpressionData.lOperandOne > 0)
            {
                ucBinaryNumber[ucArrayIndex] = sExpressionData.lOperandOne % 2;
                sExpressionData.lOperandOne /= 2;
                ucArrayIndex++;
            }
            for(ucLoopIndex = ucArrayIndex - 1; ucLoopIndex >=0; ucLoopIndex--)
            {
                lResult = (lResult * 10) + ucBinaryNumber[ucLoopIndex];
            }
            break;
        case 'r':
            uint8_t ucIter = 0;
            uint32_t ulr= 0;
            for(ucIter = 0; ucIter < 32; ucIter++)
            {
                lResult <<=1;
                lResult |= (sExpressionData.lOperandOne & 1);
                sExpressionData.lOperandOne >>= 1;
            }
            break;
        default:
            return -1;
        }
    }
    else
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Invalid operator");
        eError = MATH_INVALID_OPERATOR;
    }

    *lCalculatedResult = lResult;

    return eError;
}

eMathError ParseConvertToHex(struct _sExpression sExpressionData, uint8_t ucBuffer[], uint32_t *ulSizeOfBuffer)
{
    uint8_t ucHexNumber[10];
    uint8_t ucArrayIndex = 0;
    uint8_t ucTemp = 0;
    uint8_t ucBufferIndex = 0;
    int ucLoopIndex = 0;
    while (sExpressionData.lOperandOne != 0)
    {
        ucTemp = sExpressionData.lOperandOne % 16;
        if (ucTemp < 10)
        {
            ucHexNumber[ucArrayIndex] = ucTemp + 48;
        }
        else
        {
            ucHexNumber[ucArrayIndex] = ucTemp + 55;
        }
        ucArrayIndex++;
        sExpressionData.lOperandOne /= 16;
    }

    *ulSizeOfBuffer = ucArrayIndex;
    printf("Hexadecimal value is: 0X");
    for (ucLoopIndex = ucArrayIndex - 1; ucLoopIndex >= 0; ucLoopIndex--)
    {
        printf("%c", ucHexNumber[ucLoopIndex]);
        ucBuffer[ucBufferIndex] = ucHexNumber[ucLoopIndex];
        ucBufferIndex++;
    }

    return MATH_OK;
}
