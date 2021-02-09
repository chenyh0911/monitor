#include "TcpSocket.h"

//��������
TcpSocket::~TcpSocket()
{
	//�رշ�����׽���
	Close();
};

//�����׽���
bool TcpSocket::init() 
{
	_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sockfd < 0)
	{
		perror("socket error\n");
		return false;
	}

	//int opt = 1;
	//setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//����keepalive
	return true;
}

//���׽��ֽṹ��
bool TcpSocket::Bind()
{
	struct sockaddr_in addr;
	//memset(&addr, 0, sizeof(addr)); //���ݳ�ʼ��--����
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET; //����IPͨѶ
	addr.sin_addr.s_addr = INADDR_ANY; //������IP��ַ-�������ӵ����б��ص�ַ��"0.0.0.0"
	//addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port); //�������˿ں�

	socklen_t len = sizeof(struct sockaddr_in);
	int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
	if (ret < 0)
	{
		perror("bind error");
		return false;
	}
	return true;
};

//�����׽���
bool TcpSocket::Listen(int backlog)
{
	//int listen(int sockfd, int backlog);
	//backlog:��󲢷�������--�ں�����������Ӷ��е����ڵ���
	int ret = listen(_sockfd, backlog);
	if (ret < 0)
	{
		perror("listen error");
		return false;
	}
	return  true;
};

bool TcpSocket::ConvertToNet(int _sockfd, std::string &ip, int &port)
{
	struct sockaddr_in cAddr;//�ֱ��ʾ���ӵĶԶ˵�ַ���ͻ��˵�ַ
	char ipAddr[INET_ADDRSTRLEN];//������ʮ���Ƶĵ�ַ
	socklen_t clen;
	//��ȡconnfd��ʾ�������ϵĶԶ˵�ַ
	clen = sizeof(cAddr);
	if (getpeername(_sockfd, (struct sockaddr *)&cAddr, &clen) == -1) 
	{
		printf("fd [%d] getpeername error\n", _sockfd);
		return false;
	}
	ip = inet_ntop(AF_INET, &cAddr.sin_addr, ipAddr, sizeof(ipAddr));
	port = ntohs(cAddr.sin_port);
	//printf("connected peer address = %s:%d\n", ip, ntohs(cAddr.sin_port));
	return true;
}