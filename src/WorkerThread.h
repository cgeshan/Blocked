#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class WorkerThread
{
private:
    enum
    {
        TASK_NONE,
        TASK_QUIT,
        TASK_RUN
    };

    int taskType=TASK_NONE;
    std::function <void()> task;
    std::thread thr;
    std::mutex mtx;
    std::condition_variable cond; // this takes care of the condition variable for us!
    void ThreadFunc();
public:
    WorkerThread();
    ~WorkerThread();
    void Run(std::function <void()>);
    void Wait(void);
    bool isRunning(void);
};