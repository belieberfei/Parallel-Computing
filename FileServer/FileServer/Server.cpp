#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include "CBlockingSocket.h"


using namespace std;

DWORD WINAPI serverthread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	CBlockingSocket ClientSocket = *cs;                               //用局部变量保存线程传进来的地址传递的参数，防止主线程中socket被改写。

	BYTE filename[100] = { 0 };
	if (ClientSocket.Read(filename, 100) != -1)
	{
		cout << "  File requested from the client: " << filename << endl;
		ifstream infile((char *)filename, ios::in | ios::binary);

		infile.seekg(0, ios::end);
		int fileLength = infile.tellg();
		infile.seekg(0, ios::beg);
		bool flag = true;
		if (!infile)
		{
			cout << "Open file failed!" << endl;
		}
		else
		{
			BYTE file[101] = { 0 };
			while (fileLength >= 100)
			{
				infile.read((char *)file, 100);
				file[100] = '\0';
				flag = ClientSocket.Send(file, 100);
				fileLength -= 100;
			}
			infile.read((char *)file, fileLength);
			file[fileLength] = '\0';
			flag = ClientSocket.Send(file, fileLength);
		}
		if (flag)
		{
			cout << "  Send file back to the client: " << filename << endl;
		}
	}

	ClientSocket.Close();                   //关闭socket
	return 0;
}

int main(int argc, char *argv[])
{


	if (argc != 2)
	{
		cout << "FileServer 8888" << endl;
		return 1;
	}


	CBlockingSocket ListenSocket;                                  //定义一个ListenSocket对象
	CBlockingSocket::Initialize();                                 //将此对象初始化
	ListenSocket.HintsAndResult(NULL, argv[1]);
	ListenSocket.Open();                                           //open（）函数创建socket
	ListenSocket.Bind();                                           //bind（）函数绑定本机信息与端口号
	ListenSocket.Listen(argv[1]);                                  //参数进来，开始监听端口号
	cout << "FileServer is listening on port:" << argv[1] << endl;
	while (1)
	{
		cout << "Waiting for a connection…" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		CBlockingSocket ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		cout << "Accepted connection from " << inet_ntoa(their_addrin.sin_addr) << endl;

		HANDLE hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverthread, &ClientSocket, 0, NULL);

	}




	ListenSocket.Close();                                     //关闭ListenSocket
	CBlockingSocket::Cleanup();
	return 0;
}