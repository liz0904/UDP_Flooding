#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>   //윈도우 소켓(WinSock)을 위한 API 함수의 헤더
#pragma comment(lib,"ws2_32.lib")

#define PORT 10004   //서버의 포트 번호
#define BUFSIZE 1024

int main(void) {
    WSADATA winsockData;   //윈속 초기화 구조체
    SOCKET serverSocket;   //소켓 구조체
    SOCKADDR_IN serverAddr;   //소켓 주소 구조체
    SOCKADDR_IN clientAddr;

    char msg1[] = "IMHACKER_2019111313";//C&C 서버에게 보내는 메시지
    char buf[BUFSIZE];
    char msg2[] = "2019111313";//Victim에게 보내는 메시지
    int sendLen, recvLen, clientSize;

    //윈속 초기화 함수
    if (WSAStartup(0x202, &winsockData) == SOCKET_ERROR) {
        printf("[-] Winsocket Initialzation falied.\n");
        WSACleanup();
        return 0;
    }
    printf("[+] Winsock Init.\n");
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));
    memset(buf, 0, BUFSIZE);

    //소켓 주소 구조체 초기화
    serverAddr.sin_family = AF_INET;   //IPv4를 사용
    serverAddr.sin_addr.s_addr = inet_addr("114.70.37.17");   //C&C 서버 통신 IP
    serverAddr.sin_port = htons(PORT);//C&C 서버 통신 PORT 번호

    //소켓을 열고, 열린 소켓에 해당하는 파일 디스크립터 리턴
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        printf("[-] Socket creation failed.\n");
        WSACleanup();
        return 0;
    }
    printf("[+] Socket create.\n");

    //C&C Server에게 IMHACKER_2019111313 보내기
    sendLen = sendto(serverSocket, msg1, sizeof(msg1), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sendLen != sizeof(msg1)) {
        printf("[-] sendto() error.\n");
        return 0;
    }

    //C&C Server에게 IP, PORT 값을 받음
    clientSize = sizeof(clientAddr);
    recvLen = recvfrom(serverSocket, buf, BUFSIZE, 0, (struct sockaddr*)&clientAddr, &clientSize);
    if (recvLen < 0) {
        printf("[-] recvform() error.\n");
        return 0;
    }
    printf("[+]Recevied: %s\n", buf);

    //buf에 저장된 Victim의 IP와 PORT 잘라내기
    int i = 0;
    char* sArr[10] = { NULL, };
    char* ptr1 = strtok(buf, "_");
    while (ptr1 != NULL) {//자른 문자열이 나오지 않을 때까지 반복
        sArr[i] = ptr1;//문자열을 자른 뒤 메모리 주소를 문자열 포인터 배열에 저장
        i++;//인덱스 증가
        ptr1 = strtok(NULL, ":");//다음 문자열을 잘라서 포인터를 반환
    }
    char* ptr2 = strstr(buf + 18, "7");
    int num = atoi(ptr2);//Victim의 PORT 번호를 INT형 변환

    //socket 사용을 위한 소켓 주소 구조체 초기화
    clientAddr.sin_family = AF_INET;//IPv4
    clientAddr.sin_addr.s_addr = inet_addr(sArr[1]);//Victim 통신 IP
    clientAddr.sin_port = htons(num);//Victim 통신 PORT

    //10번 반복하여 Victim 에게 msg2를 보냄.
    for (int count = 0; count < 10; count++) {
        sendLen = sendto(serverSocket, msg2, sizeof(msg2), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        if (sendLen != sizeof(msg2)) {
            printf("[-] sendto() error.\n");
            return 0;
        }
    }
    closesocket(serverSocket);
    return 0;
}