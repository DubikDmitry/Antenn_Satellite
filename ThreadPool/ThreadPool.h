#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

class Task {
public:
    virtual ~Task() {}
    virtual void execute() = 0;
};

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::unique_ptr<Task> > tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::condition_variable waitCv;
    bool stop;
    int activeTasks;

    void workerLoop();

public:
    ThreadPool(int numThreads);
    ~ThreadPool();
    void add_task(std::unique_ptr<Task> task);
    void wait();
};

class JacobiTask : public Task {
private:
    std::vector<std::vector<double> >& A;
    std::vector<double>& b;
    std::vector<double>& x;
    std::vector<double>& x_new;
    int idx;

public:
    JacobiTask(std::vector<std::vector<double> >& A,
               std::vector<double>& b,
               std::vector<double>& x,
               std::vector<double>& x_new,
               int idx);
    void execute();
};

#endif