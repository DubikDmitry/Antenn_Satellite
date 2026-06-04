#ifndef ERROR_H
#define ERROR_H

#include <string>

enum ErrorCode {
    GENERATION_FAILED,
    QUEUE_ERROR,
    SOLVER_FAILED,
    FILE_ERROR,
    THREAD_POOL_ERROR
};

class SimulationError {
private:
    std::string message;
    ErrorCode code;

public:
    SimulationError(std::string msg, ErrorCode c);
    std::string what();
    ErrorCode getCode();
};

#endif