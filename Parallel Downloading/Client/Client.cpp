#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "CBlockingSocket.h"
#include <windows.h>
#include <stdio.h>

using namespace std;


char savefile[100] = { "C:\\Users\\yifei zhou\\Desktop\\小说2.txt" };
char serverIP[100] = { "192.168.43.237" };
char serverPORT[100] = { 0 };
int serverPORT2 = 8000;
char conNum[100] = { 0 };
int m = 0;
int i;
HANDLE ghMutex;
fstream fs;
int port = 8000;
DWORD WINAPI clientthread(LPVOID);
DWORD WINAPI clientthreadNoMutex(LPVOID);

#define ThreadFunction clientthread

//使用互斥信号量
DWORD WINAPI clientthread(LPVOID lpParameter) {

	_itoa(serverPORT2, serverPORT, 10);
	cout << "Connection established to remote Server at " << serverIP << ":" << serverPORT << endl;



	CBlockingSocket BlockingSocket = CBlockingSocket();
	BlockingSocket.Open(serverIP, serverPORT);
	cout << "Receiving file... " << endl;
	_itoa(i, conNum, 10);


	BlockingSocket.Send(conNum, 100);

	WaitForSingleObject(
		ghMutex,   
		INFINITE);

	char recvBuf[101] = { 0 };
	int filerecvd = 0;
	while (filerecvd != -1)
	{
		filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
		fs.write(recvBuf, filerecvd);

	}
	return 0;

}

//未使用互斥信号量
DWORD WINAPI clientthreadNoMutex(LPVOID lpParameter) {

	_itoa(serverPORT2, serverPORT, 10);
	cout << "Connection established to remote Server at " << serverIP << ":" << serverPORT << endl;



	CBlockingSocket BlockingSocket = CBlockingSocket();
	BlockingSocket.Open(serverIP, serverPORT);
	cout << "Receiving file... " << endl;
	_itoa(i, conNum, 10);


	BlockingSocket.Send(conNum, 100);



	char recvBuf[101] = { 0 };
	int filerecvd = 0;
	while (filerecvd != -1)
	{
		filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
		fs.write(recvBuf, filerecvd);

	}
	return 0;

}



int main(void) {
	HANDLE aThread[3];
	DWORD ThreadID;

	ghMutex = CreateMutex(
		NULL,              
		FALSE,             
		NULL);             

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}
	fs.open(savefile, ios::out | ios::binary);

	for (i = 0; i < 3; i++)
	{
		aThread[i] = CreateThread(
			NULL,       
			0,         
			(LPTHREAD_START_ROUTINE)ThreadFunction,
			NULL,       
			0,          
			&ThreadID); 

		if (aThread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}

		Sleep(200);
		serverPORT2++;
	}
	WaitForMultipleObjects(3, aThread, TRUE, INFINITE);
	printf("文件下载完成！");
	Sleep(5000);
	for (i = 0; i < 3; i++)
		CloseHandle(aThread[i]);
	CloseHandle(ghMutex);

}