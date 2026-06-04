#include "ThreadPool.h"
#include "Error.h"
#include <iostream>
ThreadPool::ThreadPool(int numThreads) {
    stop = false;
    activeTasks = 0;
    for (int i = 0; i < numThreads; i++) {
        workers.push_back(std::thread(&ThreadPool::workerLoop, this));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all();
    for (int i = 0; i < (int)workers.size(); i++) {
        if (workers[i].joinable()) {
            workers[i].join();
        }
    }
}

void ThreadPool::add_task(std::unique_ptr<Task> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (stop) {
            throw SimulationError("Попытка добавить задачу в остановленный ThreadPool", THREAD_POOL_ERROR);
        }
        tasks.push(std::move(task));
        activeTasks++;
    }
    cv.notify_one();
}

void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(queueMutex);
    while (!(tasks.empty() && activeTasks == 0)) {
        waitCv.wait(lock);
    }
}

void ThreadPool::workerLoop() {
    bool should_run = true;
    while (should_run) {
        std::unique_ptr<Task> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            while (!stop && tasks.empty()) { cv.wait(lock); }
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
            } catch (const std::exception& e) {
                // Логируем ошибку в задаче, но не прерываем поток
                std::cerr << "Ошибка в задаче: " << e.what() << std::endl;
            }
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                activeTasks--;
                if (tasks.empty() && activeTasks == 0) { waitCv.notify_all(); }
            }
        }
    }
}

JacobiTask::JacobiTask(std::vector<std::vector<double> >& A,
                       std::vector<double>& b,
                       std::vector<double>& x,
                       std::vector<double>& x_new,
                       int idx) : A(A), b(b), x(x), x_new(x_new), idx(idx) {
}

void JacobiTask::execute() {
    double sum = 0.0;
    int n = (int)A.size();
    for (int j = 0; j < n; j++) {
        if (j != idx) {
            sum += A[idx][j] * x[j];
        }
    }
    double diag = A[idx][idx];
    if (std::abs(diag) < 1e-12) {
        throw SimulationError("Диагональный элемент слишком близок к нулю", SOLVER_FAILED);
    }
    x_new[idx] = (b[idx] - sum) / diag;
}