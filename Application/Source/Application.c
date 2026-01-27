//******************************** APPLICATION ****************************************
// Copyright (c) 2025
// All Rights Reserved
//*********************************************************************************
//
// File		: Application.c
// Summary	: user functions and server functions
// Note		: None
// Author	: Abhishek
// Date		: 16-01-2026
//
//*********************************************************************************

//******************************* Include Files ***********************************
#include "Application.h"

//******************************* Local Types *************************************

//***************************** Local Constants ***********************************

//***************************** Extern Variables **********************************

//**************************** Global Variables ***********************************

//****************************** Local Functions **********************************

//***************************** Function Definitions ******************************
//******************************.FUNCTION_HEADER.******************************
//Purpose : Thread to listen cleints and calculate the result
//Inputs  : 
//Outputs : None
//Return  : void
//**********************************************************************************
void *ApplicationServerThread(void *arg)
{
    uint8_t ucSocketFD, ucNewSocket;
    struct sockaddr_in address;
    uint8_t ucOpt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    eReadError eReadErrorCode = 0;

    // Creating socket file descriptor
    if(!ServerCreateSocket(&ucSocketFD))
    {
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if(!ServerAttachSocket(ucSocketFD, &ucOpt))
    {
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if(!ServerBind(ucSocketFD))
    {
        exit(EXIT_FAILURE);
    }

    if(!ServerListen(ucSocketFD, 1))
    {
        exit(EXIT_FAILURE);
    }

    printf("waiting....\n");
    
    if(!ServerAccept(ucSocketFD, &ucNewSocket))
    {
        exit(EXIT_FAILURE);
    }
    
    while(1){
        eReadErrorCode = SocketRead(ucNewSocket);
        if(eReadErrorCode != READ_OK)
        {
            if(!ServerListen(ucSocketFD, 1))
            {
                exit(EXIT_FAILURE);
            }

            printf("Client disconnected, waiting for other client....\n");
            
            if(!ServerAccept(ucSocketFD, &ucNewSocket))
            {
                exit(EXIT_FAILURE);
            }
        }
    }

    // closing the connected socket
    SocketClose(ucNewSocket);
  
    // closing the listening socket
    SocketClose(ucSocketFD);
}