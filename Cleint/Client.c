#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "8080"
#define DEFAULT_SERVER "127.0.0.1"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET clientSocket = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    char sendBuffer[BUFFER_SIZE];
    uint32_t recvBuffer[BUFFER_SIZE];
    int result;
    
    // Command line arguments
    char *serverIP = (argc > 1) ? argv[1] : DEFAULT_SERVER;
    char *port = (argc > 2) ? argv[2] : DEFAULT_PORT;
    
    printf("Connecting to %s:%s...\n", serverIP, port);
    
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("socket failed: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));
    
    // Convert IP address
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
    printf("Invalid IP address\n");
    WSACleanup();
    return 1;
}
    
    // Connect to server
    result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        printf("connect failed: %ld\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    printf("Connected to server successfully!\n");

    printf("Allowed operations\n1. Add(+)\n2. Subtract(-)\n3. Multiplication(*)\n");
    printf("Bitwise operation\n1. AND(&)\n2. OR(|)\n3. XOR(^)\n4. NOT(~)\n5.NAND($)\n6. NOR(#)\n");
    printf("FYI: the operation NAND and NOR are assigned with $ and #, since there is no other operators for that");
    // Send/Receive loop
    while (1) {
        printf("\nEnter expression (or 'quit' to exit): ");
        fgets(sendBuffer, sizeof(sendBuffer), stdin);
        
        // Remove newline
        sendBuffer[strcspn(sendBuffer, "\n")] = 0;
        
        if (strcmp(sendBuffer, "quit") == 0) {
            break;
        }

        // Send message
        result = send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
        if (result == SOCKET_ERROR) {
            printf("send failed: %ld\n", WSAGetLastError());
            break;
        }
        
        // printf("Sent: %s\n", sendBuffer);
        
        // Receive response
        result = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
        if (result > 0) {
            recvBuffer[result] = '\0';
            printf("Answer: %s\n", recvBuffer);
        } else if (result == 0) {
            printf("Server disconnected.\n");
            break;
        } else {
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
