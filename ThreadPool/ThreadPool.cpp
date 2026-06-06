#include "ThreadPool.h"
#include "Error.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads) {
    stop = false;
    activeTasks = 0;
    workers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all();
    for (std::thread& worker : workers) {
        if (worker.joinable()) worker.join();
    }
}

void ThreadPool::add_task(std::unique_ptr<SolverMethod> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (stop) {
            throw SimulationError("Attempt to add task to stopped ThreadPool", THREAD_POOL_ERROR);
        }
        tasks.push(std::move(task));
        ++activeTasks;
    }
    cv.notify_one();
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(queueMutex);
    waitCv.wait(lock, [this]() { return tasks.empty() && activeTasks == 0; });
}

void ThreadPool::workerLoop() {
    bool should_run = true;
    while (should_run) {
        std::unique_ptr<SolverMethod> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                should_run = false;
            } else {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }
        if (task) {
            try {
                task->execute();
            } catch (SimulationError& e) {
                std::cerr << "Simulation error in solver method: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Standard error in solver method: " << e.what() << std::endl;
            }
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                --activeTasks;
                if (tasks.empty() && activeTasks == 0) {
                    waitCv.notify_all();
                }
            }
        }
    }
}

JacobiMethod::JacobiMethod(std::vector<std::vector<double> >& A,
                           std::vector<double>& b,
                           std::vector<double>& x,
                           std::vector<double>& x_new,
                           size_t idx) : A(A), b(b), x(x), x_new(x_new), idx(idx) { }

void JacobiMethod::execute() {
    double sum = 0.0;
    size_t n = A.size();
    for (size_t j = 0; j < n; ++j) {
        if (j != idx) {
            sum += A[idx][j] * x[j];
        }
    }
    double diag = A[idx][idx];
    if (std::abs(diag) < 1e-12) {
        throw SimulationError("Diagonal element too close to zero", SOLVER_FAILED);
    }
    x_new[idx] = (b[idx] - sum) / diag;
}