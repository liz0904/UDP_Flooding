#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>   //������ ����(WinSock)�� ���� API �Լ��� ���
#pragma comment(lib,"ws2_32.lib")

#define PORT 10004   //������ ��Ʈ ��ȣ
#define BUFSIZE 1024

int main(void) {
    WSADATA winsockData;   //���� �ʱ�ȭ ����ü
    SOCKET serverSocket;   //���� ����ü
    SOCKADDR_IN serverAddr;   //���� �ּ� ����ü
    SOCKADDR_IN clientAddr;

    char msg1[] = "IMHACKER_2019111313";//C&C �������� ������ �޽���
    char buf[BUFSIZE];
    char msg2[] = "2019111313";//Victim���� ������ �޽���
    int sendLen, recvLen, clientSize;

    //���� �ʱ�ȭ �Լ�
    if (WSAStartup(0x202, &winsockData) == SOCKET_ERROR) {
        printf("[-] Winsocket Initialzation falied.\n");
        WSACleanup();
        return 0;
    }
    printf("[+] Winsock Init.\n");
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));
    memset(buf, 0, BUFSIZE);

    //���� �ּ� ����ü �ʱ�ȭ
    serverAddr.sin_family = AF_INET;   //IPv4�� ���
    serverAddr.sin_addr.s_addr = inet_addr("114.70.37.17");   //C&C ���� ��� IP
    serverAddr.sin_port = htons(PORT);//C&C ���� ��� PORT ��ȣ

    //������ ����, ���� ���Ͽ� �ش��ϴ� ���� ��ũ���� ����
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        printf("[-] Socket creation failed.\n");
        WSACleanup();
        return 0;
    }
    printf("[+] Socket create.\n");

    //C&C Server���� IMHACKER_2019111313 ������
    sendLen = sendto(serverSocket, msg1, sizeof(msg1), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sendLen != sizeof(msg1)) {
        printf("[-] sendto() error.\n");
        return 0;
    }

    //C&C Server���� IP, PORT ���� ����
    clientSize = sizeof(clientAddr);
    recvLen = recvfrom(serverSocket, buf, BUFSIZE, 0, (struct sockaddr*)&clientAddr, &clientSize);
    if (recvLen < 0) {
        printf("[-] recvform() error.\n");
        return 0;
    }
    printf("[+]Recevied: %s\n", buf);

    //buf�� ����� Victim�� IP�� PORT �߶󳻱�
    int i = 0;
    char* sArr[10] = { NULL, };
    char* ptr1 = strtok(buf, "_");
    while (ptr1 != NULL) {//�ڸ� ���ڿ��� ������ ���� ������ �ݺ�
        sArr[i] = ptr1;//���ڿ��� �ڸ� �� �޸� �ּҸ� ���ڿ� ������ �迭�� ����
        i++;//�ε��� ����
        ptr1 = strtok(NULL, ":");//���� ���ڿ��� �߶� �����͸� ��ȯ
    }
    char* ptr2 = strstr(buf + 18, "7");
    int num = atoi(ptr2);//Victim�� PORT ��ȣ�� INT�� ��ȯ

    //socket ����� ���� ���� �ּ� ����ü �ʱ�ȭ
    clientAddr.sin_family = AF_INET;//IPv4
    clientAddr.sin_addr.s_addr = inet_addr(sArr[1]);//Victim ��� IP
    clientAddr.sin_port = htons(num);//Victim ��� PORT

    //10�� �ݺ��Ͽ� Victim ���� msg2�� ����.
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