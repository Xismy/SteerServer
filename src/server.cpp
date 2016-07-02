#include "stdafx.h"
#include <Ws2bth.h>
#include "server.h"
#pragma comment (lib, "Ws2_32") 

SOCKET socket2 = INVALID_SOCKET;
DEFINE_GUID(g_guidServiceClass, 0xA16F2B14, 0xC236, 0xBB07, 0x70, 0x2A, 0x90, 0x4C, 0x27, 0x8F, 0xB8, 0x92);

void bt_server()
{
	WSADATA wsa;
	SOCKET sock;
	
	SOCKADDR_BTH addr = { 0 };
	int len = sizeof(SOCKADDR_BTH);
	WSAQUERYSET queryset;
	CSADDR_INFO addrInfo;

	WSAStartup(MAKEWORD(2, 2), &wsa);
	sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	addr.addressFamily = AF_BTH;
	addr.port = BT_PORT_ANY;
	bind(sock, (SOCKADDR*)&addr, len);
	getsockname(sock, (SOCKADDR*)&addr, &len);

	addrInfo.LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	addrInfo.LocalAddr.lpSockaddr = (LPSOCKADDR)&addr;
	addrInfo.RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	addrInfo.RemoteAddr.lpSockaddr = (LPSOCKADDR)&addr;
	addrInfo.iSocketType = SOCK_STREAM;
	addrInfo.iProtocol = BTHPROTO_RFCOMM;

	ZeroMemory(&queryset, sizeof(queryset));
	queryset.dwSize = sizeof(WSAQUERYSET);
	queryset.lpszServiceInstanceName = "Steer";
	queryset.lpServiceClassId = (LPGUID)&g_guidServiceClass;
	queryset.dwNameSpace = NS_BTH;
	queryset.dwNumberOfCsAddrs = 1;
	queryset.lpcsaBuffer = &addrInfo;

	WSASetService(&queryset, RNRSERVICE_REGISTER, 0);
	listen(sock, 4);
	socket2 = accept(sock, NULL, NULL);
	return;
}

void getInput(input* in) {
	char buff[9];
	if (socket2 == INVALID_SOCKET) {
		printf("ERROR: %d\n", WSAGetLastError());
		return;
	}
	recv(socket2, buff, 9, 0);
	in->y = (INT16)(buff[0]) << 8 | (0x00FF & (INT16)buff[1]);
	in->x = (INT16)(buff[2]) << 8 | (0x00FF & (INT16)buff[3]);
	in->padX = (INT16)(buff[4]) << 8 | (0x00FF & (INT16)buff[5]);
	in->padY = (INT16)(buff[6]) << 8 | (0x00FF & (INT16)buff[7]);
	in->buttons = (0x00FF & (INT16)buff[8]);
}