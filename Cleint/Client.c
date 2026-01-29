#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "8080"
#define DEFAULT_SERVER "127.0.0.1"
#define BUFFER_SIZE 1024

#pragma pack(push, 1)

typedef struct __sExpression
{
    int32_t lOperandOne;
    uint8_t ucOperator;
    int32_t lOperandTwo;
} _sExpression;


int main(int argc, char *argv[])
{
    WSADATA wsaData;
    _sExpression sExpression = {0};
    SOCKET clientSocket = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    char sendBuffer[BUFFER_SIZE];
    uint32_t recvBuffer[20];
    int result;
    int32_t lFirstOperand, lSecondOperand = 0;
    int64_t llTempFirst, llTempSecond = 0;
    char operator = '+';

    // Command line arguments
    char *serverIP = (argc > 1) ? argv[1] : DEFAULT_SERVER;
    char *port = (argc > 2) ? argv[2] : DEFAULT_PORT;

    printf("Connecting to %s:%s...\n", serverIP, port);

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("socket failed: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));

    // Convert IP address
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    if (serverAddr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Invalid IP address\n");
        WSACleanup();
        return 1;
    }

    // Connect to server
    result = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        printf("connect failed: %ld\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server successfully!\n");

    sExpression.ucOperator = '-';
    sExpression.lOperandOne = htonl(1);
    sExpression.lOperandTwo = htonl(10);

    printf("Allowed operations\n1. Add(+)\n2. Subtract(-)\n3. Multiplication(*)\n");
    printf("Bitwise operation\n1. AND(&)\n2. OR(|)\n3. XOR(^)\n4. NOT(~)\n5.NAND($)\n6. NOR(#)\n");
    printf("FYI: the operation NAND and NOR are assigned with $ and #, since there is no other operators for that");
    // Send/Receive loop
    while (1)
    {
        printf("\nEnter the Operator:");
        scanf(" %c", &operator);
        if (operator == '~' || operator == 'x' || operator == 'b' )
        {
            printf("\nEnter the number:");
            scanf("%ld", &llTempFirst);
        }
        else
        {
            printf("\nEnter the first number:");
            scanf("%lld", &llTempFirst);
            printf("\nEnter the second number:");
            scanf("%lld", &llTempSecond);
        }

        if (llTempFirst > INT32_MAX || llTempSecond > INT32_MAX)
        {
            printf("\nOverflow\n");
            continue;
        }
        else
        {
            lFirstOperand = llTempFirst;
            lSecondOperand = llTempSecond;
        }
        printf("1st = %d\n", lFirstOperand);

        sExpression.ucOperator = operator;
        sExpression.lOperandOne = htonl(lFirstOperand);
        sExpression.lOperandTwo = htonl(lSecondOperand);

        // Remove newline
        sendBuffer[strcspn(sendBuffer, "\n")] = 0;

        result = send(clientSocket, &sExpression, sizeof(sExpression), 0);
        if (result == SOCKET_ERROR)
        {
            printf("send failed: %ld\n", WSAGetLastError());
            break;
        }

        // Receive response
        result = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
        if (result > 0)
        {
            recvBuffer[result] = '\0';
            if(operator == 'x' )
            {
                printf("Answer: 0X%s\n", recvBuffer);
            }
            else
            {
                printf("Answer: %s\n", recvBuffer);
            }
            
        }
        else if (result == 0)
        {
            printf("Server disconnected.\n");
            break;
        }
        else
        {
            printf("recv failed: %ld\n", WSAGetLastError());
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    printf("Connection closed.\n");

    return 0;
}
