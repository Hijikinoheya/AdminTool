#include "pch.h"
#include "process_manager.h"
#include <tlhelp32.h>

void ManageProcesses() {
    std::cout << "Listing Processes..." << std::endl;

    PROCESSENTRY32 pe32;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Could not create snapshot of processes." << std::endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcessSnap, &pe32)) {
        do {
            std::wcout << L"PID: " << pe32.th32ProcessID << L" - " << pe32.szExeFile << std::endl;
        } while (Process32Next(hProcessSnap, &pe32));
    }
    else {
        std::cerr << "Error: Could not retrieve processes." << std::endl;
    }

    CloseHandle(hProcessSnap);
}
