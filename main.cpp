#include <cstdio>

#include "Common/Global.h"
#include "Worker/ThreadPool.h"
#include "Worker/BaseTask.h"
#include "Worker/EpollServer.h"

#define SIP "0.0.0.0"
#define SPORT 8000	//服务端端口

#define threadnum 10	//线程池数
#define maxtasknum 40  //最大任务数

int main()
{
	//实例化服务器socket
	TcpSocket* tSocket = new TcpSocket(SPORT);
	tSocket->init();
	tSocket->Bind();
	tSocket->Listen();

	//将指针指向的socket值存入全局类
	Global::getInstance()->SetSocket(tSocket);


	//初始化线程池
	ThreadPool<BaseTask>* pool = new ThreadPool<BaseTask>(threadnum, maxtasknum);
	//初始化线程池
	pool->init();

	//初始化管道服务
	EpollServer* epoll = new EpollServer(pool);
	epoll->init();
	epoll->run(tSocket->GetSockFd());
	epoll->Close();

	//程序退出清理内存
	Global::getInstance()->release();
    return 0;
}