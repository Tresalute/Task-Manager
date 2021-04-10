#pragma once
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")


//发送包头的类型
enum DataPackType {
	login = 1,				// 登录类型
	registe = 2,			// 注册
	sendmsg = 3,			// 私聊消息
	addFriend = 4,			// 好友请求
	getFriend = 5,			// 获取好友
	CreateRoom = 6,			// 创建聊天室
	JoRoom = 7,				// 加入聊天室
	getRoom = 8,			// 获取聊天室
	roomMsg = 9,			// 聊天室消息
	msged = 10,				// 双人 消息
	roomHisMsg = 11,		// 聊天室 消息 
	filehide = 12,			// 文件
	roomFriend = 13,		// 更新显示聊天室成员
	GetVirusMD5Base = 14	// 获取病毒库
};

#pragma pack(push,1)
//发送的包头
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};
//接受的包头
struct DataPackResult {
	uint32_t type;
	uint32_t status;
	uint32_t size;
	char	 data[1];
};

class ServerPort
{
public:
	SOCKET m_hSocket;

	ServerPort();
	~ServerPort();
	//初始化套接字
	static void initSocket();
	//初始化客户端服务
	bool InitClient(const char* serverIp, short port);
	//发送信息
	void send(DataPackType _type, const char* data, int size = -1);
	//接受信息
	DataPackResult* recv();
	//释放
	void freeResult(DataPackResult* p);
};

