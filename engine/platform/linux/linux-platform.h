#include "../../platform.h"

class LinuxPlatform : Platform
{
public:
    int Run();
    std::string ReadFileToString (const std::string& path) override;
    virtual void Log(const char *fmt, ...) override;
};
