#include "include/debugging.h"

#include <iostream>

void logInfo(const std::string &_info)
{
#ifdef DEBUG
    std::cout << "[INFO] " << _info << std::endl;
#endif
}

void logWarning(const std::string &_warning)
{
#ifdef DEBUG
    std::cout << "[WARNING] " << _warning << std::endl;
#endif
}

void logError(const std::string &_error)
{
#ifdef DEBUG
    std::cout << "[ERROR] " << _error << std::endl;
#endif
}
