// message_receiver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <string>
#include <iostream>
#include <memory>

#define INSTANCES	10
#define BUFFER_SIZE	250

HANDLE hPipes[INSTANCES];
HANDLE hEvents[INSTANCES];
OVERLAPPED oOverlap[INSTANCES];
wchar_t msg[INSTANCES][BUFFER_SIZE];
bool dataRead[INSTANCES];

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");

	std::wstring pipeName = L"\\\\.\\pipe\\smpipe";

	for (int i = 0; i < INSTANCES; i++)
	{
		ZeroMemory(&oOverlap[i], sizeof(oOverlap[i]));
		hEvents[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
		if (hEvents[i] == NULL)
		{
			printf("Не смогли создать ивент %d: ошибка %d.\n", i, GetLastError());
			return 0;
		}
		oOverlap[i].hEvent = hEvents[i];

		hPipes[i] = CreateNamedPipe(pipeName.c_str(), 
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, 
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			INSTANCES,
			260,
			0,
			0,
			NULL);
		if (hPipes[i] == INVALID_HANDLE_VALUE)
		{
			printf("Не смогли создать пайп %d: ошибка %d.\n", i, GetLastError());
			return 0;
		}

		dataRead[i] = false;

		if (ConnectNamedPipe(hPipes[i], &oOverlap[i]) == 0)
		{
			DWORD error = GetLastError();
			if (error != ERROR_IO_PENDING)
			{
				printf("ConnectNamedPipe() для пайпа %d выдала ошибку %d\n", i, GetLastError());
				printf("Закрываем пайп\n");
				CloseHandle(hPipes[i]);
				return 0;
			}
		}

		else
			printf("ConnectNamedPipe() выполнен для пайпа %d\n", i);
	}

	printf("Сервер готов к обработке сообщений\n");

//	HANDLE hPipe = CreateNamedPipe(L"\\\\.\\pipe\\smpipe",
//		PIPE_ACCESS_DUPLEX,
//		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
//		PIPE_UNLIMITED_INSTANCES,
//		260,
//		0,
//		0,
//		NULL);
	DWORD dwWait;
	while (true)
	{
		dwWait = WaitForMultipleObjects(INSTANCES, //сколько объектов
			hEvents, //массив из INSTANCES объектов
			FALSE, //ждём хотя бы один, а не все
			INFINITE); //ждём до посинения

		int i = dwWait - WAIT_OBJECT_0; //тут мы выясняем, какой из ивентов включился
		ResetEvent(hEvents[i]);
//		BOOL WINAPI GetOverlappedResult(
//			_In_  HANDLE       hFile,
//			_In_  LPOVERLAPPED lpOverlapped,
//			_Out_ LPDWORD      lpNumberOfBytesTransferred,
//			_In_  BOOL         bWait
//			);
		DWORD bytesTransferred;
		if (GetOverlappedResult(hPipes[i], &oOverlap[i], &bytesTransferred, TRUE) == 0)
		{
			printf("GetOverlappedResult() провалился с ошибкой %d", GetLastError());
		}
		else
		{
			if (dataRead[i] == false) //если мы ещё не прочитали данные
			{
				DWORD bytesRead;
				if (ReadFile(hPipes[i], msg[i], BUFFER_SIZE, &bytesRead, &oOverlap[i]) == 0)
				{
					if (GetLastError() != ERROR_IO_PENDING)
					{
						printf("ReadFile() провалился с ошибкой %d\n", GetLastError());
					}
				}

				dataRead[i] = true;
			}
			else //если же мы уже прочитали данные, то выводим на консоль
			{
				printf("Принято %d байт\n", bytesTransferred);
				std::wcout << msg[i] << std::endl;
				dataRead[i] = false;
				DisconnectNamedPipe(hPipes[i]);
			}
		}



		
//		ConnectNamedPipe(hPipe, NULL);
//		DWORD dwBytesRead;
//		ReadFile(hPipe, msg, 260, &dwBytesRead, NULL);
//		if (dwBytesRead > 0)
//			std::wcout << msg << std::endl;
//		ResetEvent(hEvents[i]);
//		DisconnectNamedPipe(hPipe);
	}
	return 0;
}

