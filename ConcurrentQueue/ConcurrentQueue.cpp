#include "ConcurrentQueue.h"
#include "Error.h"

void ConcurrentQueue::push(RowData item) {
    try {
        {
            std::lock_guard<std::mutex> lock(mtx);
            container.push(item);
        }
        cv.notify_one();
    } catch (const std::exception& e) {
        throw SimulationError("Ошибка при push в очередь: " + std::string(e.what()), QUEUE_ERROR);
    }
}

RowData ConcurrentQueue::pop() {
    try {
        std::unique_lock<std::mutex> lock(mtx);
        while (container.empty()) {
            cv.wait(lock);
        }
        RowData item = container.front();
        container.pop();
        return item;
    } catch (const std::exception& e) {
        throw SimulationError("Ошибка при pop из очереди: " + std::string(e.what()), QUEUE_ERROR);
    }
}