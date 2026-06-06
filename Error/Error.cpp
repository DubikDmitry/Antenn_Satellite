#include "Error.h"

SimulationError::SimulationError(const std::string& msg, ErrorCode c) {
    message = msg;
    code = c;
}

std::string SimulationError::what() {
    return message;
}

ErrorCode SimulationError::getCode() {
    return code;
}