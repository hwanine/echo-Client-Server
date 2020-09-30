#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include<process.h>
#pragma comment(lib, "Ws2_32.lib")

unsigned __stdcall ServerThread(void* arg);
SOCKET servSocket, clntSocket;
int ck = 0;

void ErrorCK(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main() {
    WSADATA wsaData;
    
    char msg[1024];
    int strLength;
    int port = -1;
    

    SOCKADDR_IN servAdr, clntAdr;
    int clntAdrSize;
    
    while (port < 0 || port > 65535) {
        printf("Open the server.\nEnter port number: ");
        scanf("%d", &port);
    }


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리 초기화
        ErrorCK("Socket library initialization error!");

    // 소켓 생성
    servSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (servSocket == INVALID_SOCKET)
        ErrorCK("Socket creation error!");

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(port);

    // 소켓을 주소와 포트번호에 바인딩
    if (bind(servSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorCK("bind error");

    // listen 함수에 서버 소켓 등록
    if (listen(servSocket, 5) == SOCKET_ERROR)
        ErrorCK("listen error");

    clntAdrSize = sizeof(clntAdr);


    // 서버 컨트롤 쓰레드 호출, 클라이언트 접속 대기
    _beginthreadex(NULL, 0, ServerThread, 0, 0, NULL);
    printf("Waiting for client connection.. \n");
    while (1) {     
        clntSocket = accept(servSocket, (SOCKADDR*)&clntAdr, &clntAdrSize);    // 클라이언트 접속 승인
        if (clntSocket == INVALID_SOCKET && ck == 0)
            ErrorCK("Accept error");
        else if (clntSocket != INVALID_SOCKET)
            printf("Client connection! \n");
        else
            break;
        ck = 0;

        // 데이터 송수신
        while ((strLength = recv(clntSocket, msg, 1024, 0)) != 0) {
            msg[strLength] = 0;
            printf("Message from client: %s", msg);
            send(clntSocket, msg, strLength, 0);
        }
        printf("Client has closed the connection!\n");
        closesocket(clntSocket);
        printf("Waiting for client connection.. \n");
        ck = 1;
        printf("Shutdown or Wait?.. Default to wait (Shutdown to S): \n");
     
    }
    return 0;
}

unsigned __stdcall ServerThread(void* arg)  // 서버 컨트롤 쓰레드
{
    char c = 'n';
    while (1) {
        while (ck == 1) {
            getchar();
            scanf("%c", &c); 
            if (c == 's' || c == 'S') {
                if (ck == 0) {
                    printf("Server and client are communicating!\n");
                    break;
                }
                printf("Server shutdown!\n");
                closesocket(clntSocket);
                closesocket(servSocket);
                WSACleanup();
                exit(0);
            }
        }
    }
    return 0;
}