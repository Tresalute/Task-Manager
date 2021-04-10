#pragma once
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")


//���Ͱ�ͷ������
enum DataPackType {
	login = 1,				// ��¼����
	registe = 2,			// ע��
	sendmsg = 3,			// ˽����Ϣ
	addFriend = 4,			// ��������
	getFriend = 5,			// ��ȡ����
	CreateRoom = 6,			// ����������
	JoRoom = 7,				// ����������
	getRoom = 8,			// ��ȡ������
	roomMsg = 9,			// ��������Ϣ
	msged = 10,				// ˫�� ��Ϣ
	roomHisMsg = 11,		// ������ ��Ϣ 
	filehide = 12,			// �ļ�
	roomFriend = 13,		// ������ʾ�����ҳ�Ա
	GetVirusMD5Base = 14	// ��ȡ������
};

#pragma pack(push,1)
//���͵İ�ͷ
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};
//���ܵİ�ͷ
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
	//��ʼ���׽���
	static void initSocket();
	//��ʼ���ͻ��˷���
	bool InitClient(const char* serverIp, short port);
	//������Ϣ
	void send(DataPackType _type, const char* data, int size = -1);
	//������Ϣ
	DataPackResult* recv();
	//�ͷ�
	void freeResult(DataPackResult* p);
};

