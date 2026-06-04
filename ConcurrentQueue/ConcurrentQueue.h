#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "RowData.h"

class ConcurrentQueue {
private:
    std::queue<RowData> container;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(RowData item);
    RowData pop();
};

#endif