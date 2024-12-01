#include "pch.h"
#include "disk_manager.h"

void MonitorDiskUsage() {
    LPCWSTR drive = L"C:\\";
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    if (GetDiskFreeSpaceEx(drive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        std::cout << "Total Space: " << totalNumberOfBytes.QuadPart / 1024 / 1024 / 1024 << " GB" << std::endl;
        std::cout << "Free Space: " << totalNumberOfFreeBytes.QuadPart / 1024 / 1024 / 1024 << " GB" << std::endl;
    }
    else {
        std::cerr << "Error: Could not retrieve disk information." << std::endl;
    }
}
