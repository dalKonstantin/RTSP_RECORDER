// utils.cpp
#include "utils.h"
#include <ctime>

std::string getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d_%m_%y___%H:%M:%S", ltm);
    return std::string(buffer);
}