#include "platform/linux/linux-platform.h"

int main(int argc, char **argv)
{
    return (new LinuxPlatform())->Run();
}
