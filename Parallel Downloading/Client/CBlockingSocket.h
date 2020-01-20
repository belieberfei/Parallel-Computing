#pragma once
#include <WinSock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

class CBlockingSocket
{
private:
	SOCKET m_socket;
public:
	CBlockingSocket();                                   // 创建 socket
	virtual		~CBlockingSocket();
	BOOL		Open(const char *ip, const char *port);  // 连接 socket
	BOOL		Close();                                 // 关闭 socket
	BOOL		Listen(const char *port);                // 监听 socket
	BOOL		Accept();                               // 接受  client socket
	BOOL		Send(char *s, UINT count);              // 发送数据包
	INT			Read(BYTE *s, UINT count);              // 接收数据包
	INT         Recv(char *recvbuf, int recvbuflen);
	static void Cleanup();
};


CBlockingSocket::CBlockingSocket()
{
	WORD wVersionRequestd = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequestd, &wsaData);
	if (err != 0)
	{
		cout << "load Winsocket failed!" << endl;
		this->m_socket = NULL;
		return;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		cout << "create socket failed!" << endl;
		this->m_socket = NULL;
		return;
	}
}

CBlockingSocket::~CBlockingSocket()
{
	if (WSACleanup() == SOCKET_ERROR)
	{
		cout << "WSACleanup failed!" << endl;
	}
}

BOOL CBlockingSocket::Open(const char *ip, const char *port)
{
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(port));
	if (connect(m_socket, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		return TRUE;
	}
	else
	{
		cout << "Open socket Failed!" << endl;
		return FALSE;
	}
}

BOOL CBlockingSocket::Close()
{
	if (closesocket(m_socket) == 0)
	{
		cout << "Connection is down" << endl;
		return TRUE;
	}
	else
	{
		cout << "Connection failed!" << endl;
		return FALSE;
	}
}

BOOL CBlockingSocket::Listen(const char *port)
{
	return TRUE;
}

BOOL CBlockingSocket::Accept()
{
	return TRUE;
}

BOOL CBlockingSocket::Send(char *s, UINT count)
{
	int sendLen = send(m_socket, (char *)s, count, 0);
	if (sendLen > 0)
	{
		return TRUE;
	}
	else
	{
		cout << "Send Failed!" << endl;
		return FALSE;
	}
}

INT CBlockingSocket::Read(BYTE *s, UINT count)
{
	int recvLen = recv(m_socket, (char *)s, count, 0);
	if (recvLen == SOCKET_ERROR)
	{
		cout << "Receive failed!" << endl;
		return -1;
	}
	return recvLen;
}

INT CBlockingSocket::Recv(char *recvbuf, int recvbuflen)
{
	int iResult;
	iResult = recv(m_socket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		//printf("Bytes received: %d\n", iResult);
		return iResult;
	}
	else if (iResult == 0)
	{
		printf("one of three part receiving over...\n");
		return -1;
	}
	else
	{
		printf("receive failed: %d\n", WSAGetLastError());
		return -1;
	}
}



void CBlockingSocket::Cleanup()
{
	WSACleanup();
}