#pragma once
#ifndef _BASE_TASK_H
#define _BASE_TASK_H


//任务抽象基类
class BaseTask
{
public:
	virtual void worker() = 0;
};

#endif // !_BASE_TASK_H


