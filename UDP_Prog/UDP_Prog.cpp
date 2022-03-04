// UDP_Prog.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
using namespace std;

int main()
{
    int opt;
    cout << "Choose option (0 - send, 1 - receive): ";
    cin >> opt;
    if (opt == 0) {
        u_short Port = 2000;
        const char IPAddr[] = "192.168.10.2";

        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        auto SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        sockaddr_in RecvAddr;
        RecvAddr.sin_family = AF_INET;
        RecvAddr.sin_port = htons(Port);
        RecvAddr.sin_addr.s_addr = inet_addr(IPAddr);

        const int BufLen = 8;
        char SendBuf[BufLen];

        for (int i = 0; i < 600; ++i) {
            sprintf(SendBuf, "%d", i);
            sendto(SendSocket, SendBuf, BufLen, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
            Sleep(10000);
        }

        closesocket(SendSocket);
        WSACleanup();
    }
    else {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        auto RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        u_short Port = 2000;

        sockaddr_in SenderAddr;
        SenderAddr.sin_family = AF_INET;
        SenderAddr.sin_port = htons(Port);
        SenderAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(RecvSocket, (SOCKADDR*)&SenderAddr, sizeof(SenderAddr));

        const int BufLen = 8;
        char RecvBuf[BufLen];
        int SenderAddrSize = 0;;

        for (int i = 0; i < 600; ++i) {
            recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
            cout << RecvBuf << endl;
        }
        closesocket(RecvSocket);
        WSACleanup();
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
