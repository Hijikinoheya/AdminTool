#include "pch.h"
#include "system_info.h"

void ShowSystemInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "Processor Architecture: " << sysInfo.wProcessorArchitecture << std::endl;
    std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    std::cout << "Total Physical Memory: " << memInfo.ullTotalPhys / 1024 / 1024 << " MB" << std::endl;
    std::cout << "Available Physical Memory: " << memInfo.ullAvailPhys / 1024 / 1024 << " MB" << std::endl;
}
