//******************************** SERVER ****************************************
// Copyright (c) 2025
// All Rights Reserved
//*********************************************************************************
//
// File		: Server.c
// Summary	: Socket communication functions
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "Server.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************

//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************

//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.******************************
//Purpose : To create socket
//Inputs  : ucServerFD - server file descriptor
//Outputs : None
//Return  : true if success, else false
//**********************************************************************************
bool ServerCreateSocket(uint8_t *ucServerFD)
{
    if ((*ucServerFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to create socket");
        return false;
    }
    LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Socket created succesfull");

    return true;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To attach socket
//Inputs  : ucServerFD - server file descriptor
//Outputs : ucOpt -  type of data pointed to by optval
//Return  : true if success, else false
//**********************************************************************************
bool ServerAttachSocket(uint8_t ucServerFD, uint8_t *ucOpt)
{
    if (setsockopt(ucServerFD, SOL_SOCKET,
                   SO_REUSEADDR , &ucOpt,
                   sizeof(ucOpt))) {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to attach socket");
        return false;
    }
    LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Socket attached succesfully");

    return true;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To Bind socket to ip and port
//Inputs  : ucServerFD - server file descriptor
//Outputs : None
//Return  : true if success, else false
//**********************************************************************************
bool ServerBind(uint8_t ucServerFD)
{
    struct sockaddr_in sAddress;
    sAddress.sin_family = AF_INET;
    sAddress.sin_addr.s_addr = INADDR_ANY;
    sAddress.sin_port = htons(PORT);

    if (bind(ucServerFD, (struct sockaddr*)&sAddress,
             sizeof(sAddress))
        < 0) {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to Bind socket");
        return false;
    }
    LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Bind succesfully");

    return true;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To listen to cleint
//Inputs  : ucServerFD - server file descriptor
//Inputs  : ucNumberOfConnection - Number of connection allowed
//Outputs : None
//Return  : True if success, else false
//**********************************************************************************
bool ServerListen(uint8_t ucServerFD, uint8_t ucNumberOfConnection)
{
    if (listen(ucServerFD, ucNumberOfConnection) < 0) {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to listen");
        return false;
    }
    LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Listening....");

    return true;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To accept client connection
//Inputs  : ucServerFD - server file descriptor
//Outputs : ucNewSocket - New socket file descriptor
//Return  : True if success, else false
//**********************************************************************************
bool ServerAccept(uint8_t ucServerFD,uint8_t *ucNewSocket)
{
    struct sockaddr_in sAddress;
    socklen_t addrlen = sizeof(sAddress);
    int new_socket = 0;
    if ((*ucNewSocket = accept(ucServerFD, (struct sockaddr*)&sAddress, &addrlen)) < 0)
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to Accept client");
        return false;
    }
    LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Client accepted");

    socklen_t addr_len = sizeof(sAddress);
    getpeername(ucServerFD, (struct sockaddr*)&sAddress, &addr_len);

    char *ip = inet_ntoa(sAddress.sin_addr);
    int port = ntohs(sAddress.sin_port);

    printf("Client from %s:%d\n", ip, port);

    return true;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To read data from socket
//Inputs  : ucNewSocket - socket file descriptor
//Outputs : None
//Return  : None
//**********************************************************************************
eReadError SocketRead(uint8_t ucNewSocket)
{
    ssize_t valread;
    uint8_t ucBuffer[1024] = { 0 };
    struct _sExpression sExpressionData = {0};
    int32_t lResult = 0;
    uint8_t ucTransmit[20];
    uint32_t ulSizeOfBuffer = 0;
    eMathError eError = MATH_OK;
    uint8_t ucOverFlowErrorMessage[] = "Overflow error";
    uint8_t ucDivideByZeroErrorMessage[] = "Division by zero";
    uint8_t ucInvalidOperatorErrorMessage[] = "Invalid operator";

    valread = read(ucNewSocket, &sExpressionData, sizeof(sExpressionData));
    sExpressionData.lOperandOne = ntohl(sExpressionData.lOperandOne);
    sExpressionData.lOperandTwo = ntohl(sExpressionData.lOperandTwo);
    
    if(valread > 0)
    {
        LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Data received");
        if(sExpressionData.ucOperator == 'x' || sExpressionData.ucOperator == 'X')
        {
            eError = ParseConvertToHex(sExpressionData,&ucTransmit, &ulSizeOfBuffer);
            SocketWrite(ucNewSocket, ucTransmit, sizeof(ucTransmit));
        }
        else
        {
            eError = ParseCalculateData(sExpressionData,&lResult);
        }

        if(eError == MATH_OVERFLOW)
        {
            valread = send(ucNewSocket, ucOverFlowErrorMessage, sizeof(ucOverFlowErrorMessage), 0);
            if(valread > 0)
            {
                LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Data transmitted");
            }
        }
        else if(eError == MATH_DIVISION_BY_ZERO)
        {
            valread = send(ucNewSocket, ucDivideByZeroErrorMessage, sizeof(ucDivideByZeroErrorMessage), 0);
            if(valread > 0)
            {
                LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Data transmitted");
            }
        }
        else if(eError == MATH_INVALID_OPERATOR)
        {
            valread = send(ucNewSocket, ucInvalidOperatorErrorMessage, sizeof(ucInvalidOperatorErrorMessage), 0);
            if(valread > 0)
            {
                LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Data transmitted");
            }
        }
        else
        {
            snprintf(ucTransmit, sizeof(ucTransmit), "%u", lResult);
            SocketWrite(ucNewSocket, ucTransmit, sizeof(ucTransmit));
        }
    }
    else if(valread == 0)
    {
        LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Client disconnected");
        return READ_CLIENT_DISCONNECTED;
    }
    else
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to read data from client");
        return READ_FAILED;
    }
    return READ_OK;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To write/transmit data
//Inputs  : ucNewSocket - new socket file descriptor
//Inputs  : ucTransmitBuffer - Buffer to transmit
//Inputs  : ucSizeOfBuffer - Size of buffer to transmit
//Outputs : None
//Return  : Void
//**********************************************************************************
void SocketWrite(uint8_t ucNewSocket, uint8_t ucTransmitBuffer[], uint8_t ucSizeOfBuffer)
{
    ssize_t valread;
    valread = send(ucNewSocket, ucTransmitBuffer, ucSizeOfBuffer, 0);
    if(valread > 0)
    {
        LOGGER(LOG_LEVEL_INFO, __FILE_NAME__, __LINE__, "Data transmitted");
    }
    else if(valread == 0)
    {
        LOGGER(LOG_LEVEL_WARNING, __FILE_NAME__, __LINE__, "Client disconnected");
    }
    else
    {
        LOGGER(LOG_LEVEL_ERROR, __FILE_NAME__, __LINE__, "Failed to send data ");
    }
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : To close socket connection
//Inputs  : ucSocketFD - File descriptor
//Outputs : None
//Return  : Void 
//**********************************************************************************
void SocketClose(uint8_t ucSocketFD)
{
    close(ucSocketFD);
}