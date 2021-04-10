#include "stdafx.h"
#include "ServerPort.h"


ServerPort::ServerPort()
{
}


ServerPort::~ServerPort()
{
}

void ServerPort::initSocket()
{
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);
}

bool ServerPort::InitClient(const char * serverIp, short port)
{
	ServerPort::initSocket();
	// 1. 创建套接字
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 2. 配置服务端的地址和端口
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, serverIp, &addr.sin_addr);
	// 3. 连接到服务端
	int result = connect(m_hSocket, (sockaddr*)&addr, sizeof(addr));
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void ServerPort::send(DataPackType _type, const char * data, int size)
{
	if (size == -1) {
		size = strlen(data);
	}
	DataPack dp = { _type,size };
	::send(m_hSocket, (char*)&dp, sizeof(dp), 0);
	::send(m_hSocket, data, size, 0);
}

DataPackResult * ServerPort::recv()
{
	DataPackResult tmphead ;
	int x = sizeof(tmphead) - 1;
	if (::recv(m_hSocket, (char*)&tmphead, sizeof(tmphead) - 1, 0) != sizeof(tmphead) - 1){
		return NULL;
	}
	DataPackResult* pBuff = (DataPackResult*)malloc(sizeof(DataPackResult) + tmphead.size);
	memset(pBuff, 0, tmphead.size + sizeof(DataPackResult));
	memcpy(pBuff, &tmphead, sizeof(tmphead));
	if (::recv(m_hSocket, pBuff->data, pBuff->size, 0) != pBuff->size) {
		free(pBuff);
		return NULL;
	}
	return (DataPackResult*)pBuff;
}

void ServerPort::freeResult(DataPackResult * p)
{
	free(p);
}
