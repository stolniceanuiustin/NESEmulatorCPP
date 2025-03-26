#ifndef LOG_HEADERGUARD
#define LOG_HEADERGUARD

#include <iostream>
#include <fstream>

class LOG
{
public:
    LOG(std::string log_type, bool log_enable);
    ~LOG(); 

    void write(const std::string &message); 
private:
    std::ofstream log; 
    bool enable;
};

#endif