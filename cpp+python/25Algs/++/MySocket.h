#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


#include <string.h>
#include <time.h>

int betriebssystem = 1;
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <ws2def.h>
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
int betriebssystem = 2;
#endif

#define SOCKET_ERROR -1
#define QUEUE_SIZE 1
#define SOCKET int

class MySocket {
public:
	MySocket(int port);
    ~MySocket();
    int setup_server(int port);
	void AcceptEm();

	int SendMessage(char *message, int len);
	int ReadMessage(char *message);
		
	SOCKET hSocket;
	SOCKET hServerSocket;
	
private:
	struct sockaddr_in Address;
    int puerta;
};

