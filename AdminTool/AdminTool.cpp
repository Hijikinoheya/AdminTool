#include "pch.h"
#include "system_info.h"
#include "process_manager.h"
#include "disk_manager.h"
#include "network_info.h"
#include "log_manager.h"
#include "file_manager.h"
#include "performance_monitor.h"

void ShowMenu() {
    std::cout << "PC Management App" << std::endl;
    std::cout << "1. Show System Info" << std::endl;
    std::cout << "2. Manage Processes" << std::endl;
    std::cout << "3. Monitor Disk Usage" << std::endl;
    std::cout << "4. Show Network Info" << std::endl;
    std::cout << "5. Monitor Event Logs" << std::endl;
    std::cout << "6. Manage Files/Directories" << std::endl;
    std::cout << "7. Monitor Performance" << std::endl;
    std::cout << "8. Exit" << std::endl;
}

int main() {
    int choice;
    bool running = true;

    while (running) {
        ShowMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            ShowSystemInfo();
            break;
        case 2:
            ManageProcesses();
            break;
        case 3:
            MonitorDiskUsage();
            break;
        case 4:
            ShowNetworkInfo();
            break;
        case 5:
            MonitorEventLogs();
            break;
        case 6:
            ManageFiles();
            break;
        case 7:
            MonitorPerformance();
            break;
        case 8:
            running = false;
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }

    return 0;
}
