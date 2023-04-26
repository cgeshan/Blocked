// For threading
#include "WorkerThread.h"

// Worker thread example from class example

WorkerThread::WorkerThread()
{
    std::thread t(&WorkerThread::ThreadFunc,this);
    thr.swap(t);
}
WorkerThread::~WorkerThread()
{
    taskType=TASK_QUIT;
    cond.notify_one();
    thr.join();
}
void WorkerThread::Run(std::function <void()> newTask)
{
    mtx.lock();
    taskType=TASK_RUN;
    task=newTask;
    mtx.unlock();
    cond.notify_one();
}
void WorkerThread::Wait(void)
{
  std::unique_lock <std::mutex> lock(mtx); 
  cond.wait(lock,[&]{return taskType==TASK_NONE;});
}

void WorkerThread::ThreadFunc()
{
    for(;;)
    {
        std::unique_lock <std::mutex> lock(mtx);
        cond.wait(lock,[&]{return taskType!=TASK_NONE;});
        if(TASK_QUIT==taskType)
        {
            break;
        }
        else if(TASK_RUN==taskType)
        {
            task();
            taskType=TASK_NONE;
            cond.notify_one();
        }
    }
}

bool  WorkerThread::isRunning()
{
    if (taskType==TASK_RUN)
    {
        return true;
    }
    else
    {
        return false;
    }
}