#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

class SolverMethod {
public:
    virtual ~SolverMethod() {}
    virtual void execute() = 0;
};

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::unique_ptr<SolverMethod> > tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::condition_variable waitCv;
    bool stop;
    size_t activeTasks;

    void workerLoop();

public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void add_task(std::unique_ptr<SolverMethod> task);
    void wait();
};

class JacobiMethod : public SolverMethod {
private:
    std::vector<std::vector<double> >& A;
    std::vector<double>& b;
    std::vector<double>& x;
    std::vector<double>& x_new;
    size_t idx;

public:
    JacobiMethod(std::vector<std::vector<double> >& A,
                 std::vector<double>& b,
                 std::vector<double>& x,
                 std::vector<double>& x_new,
                 size_t idx);
    void execute();
};

#endif