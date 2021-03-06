#pragma once

#include <string>

class Platform
{
public:
    // Add some game-visible services here as vfuncs
    virtual std::string ReadFileToString (const std::string& path) = 0;
    virtual void Log(const char *fmt, ...) = 0;
};
