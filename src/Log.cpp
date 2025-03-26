#include "include/log.h"

LOG::LOG(std::string log_type, bool log_enable) : log(log_type), enable(log_enable)
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
    if(enable)
        if (log.is_open())
        {
            log << message << std::endl;
        }
    else return;
}