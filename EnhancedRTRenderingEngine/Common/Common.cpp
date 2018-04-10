#include "stdafx.h"
#include <cstdlib>
#include <string>
#include <stdexcept>

std::string getEnvironmentVariable(const char* environment_name) {
    size_t buf;
    if (getenv_s(&buf, NULL, 0, environment_name)) throw std::runtime_error("read error");
    if (buf == 0) throw std::runtime_error("not such environment variable");
    std::string buffer;
    buffer.resize(buf + 1);//reserve
    getenv_s(&buf, &buffer[0], buf, environment_name);
    buffer.resize(std::strlen(buffer.c_str()));//resize
    return buffer;
}