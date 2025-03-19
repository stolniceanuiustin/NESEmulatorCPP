#include "include/log.h"

LOG::LOG(std::string log_type) : log(log_type) 
{
    if (!log)
    {
        std::cerr << "Error opening " << log_type << std::endl;
    }
}

LOG::~LOG()
{
    if (log.is_open())
    {
        log.close();
    }
}

void LOG::write(const std::string &message)
{
    if (log.is_open())
    {
        log << message << std::endl;
    }
}