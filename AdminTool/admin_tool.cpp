#include "admin_tool.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <psapi.h>
#include <tchar.h>
#include <Pdh.h>
#include <IPTypes.h>
#include <iphlpapi.h>

// �v���Z�X�Ǘ����s���֐�
void ManageServices() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        std::cerr << "�T�[�r�X�}�l�[�W�����J���܂���B�G���[�R�[�h: " << GetLastError() << "\n";
        return;
    }

    DWORD bytesNeeded = 0, servicesReturned = 0, resumeHandle = 0;
    ENUM_SERVICE_STATUSW* serviceStatus = nullptr;

    // �K�v�ȃo�b�t�@�T�C�Y���擾
    EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, NULL, 0, &bytesNeeded, &servicesReturned, &resumeHandle);

    if (!EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, serviceStatus, bytesNeeded, &bytesNeeded, &servicesReturned, &resumeHandle)) {
        std::cerr << "EnumServicesStatusW �Ɏ��s���܂����B�G���[�R�[�h: " << GetLastError() << "\n";
        free(serviceStatus);
        CloseServiceHandle(hSCManager);
        return;
    }


    // �K�v�ȃ��������m��
    serviceStatus = (ENUM_SERVICE_STATUSW*)malloc(bytesNeeded);
    if (!serviceStatus) {
        std::cerr << "���������蓖�ĂɎ��s���܂����B\n";
        CloseServiceHandle(hSCManager);
        return;
    }

    // �T�[�r�X�����擾
    if (!EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, serviceStatus, bytesNeeded, &bytesNeeded, &servicesReturned, &resumeHandle)) {
        std::cerr << "�T�[�r�X���̎擾�Ɏ��s���܂����B�G���[�R�[�h: " << GetLastError() << "\n";
        free(serviceStatus);
        CloseServiceHandle(hSCManager);
        return;
    }

    // �T�[�r�X����\��
    std::cout << "=== �T�[�r�X��� ===\n";
    for (DWORD i = 0; i < servicesReturned; i++) {
        std::wcout << L"�T�[�r�X��: " << serviceStatus[i].lpServiceName << L"\n";
    }

    // ����������ƃn���h���̃N���[�Y
    free(serviceStatus);
    CloseServiceHandle(hSCManager);
}

// �f�B�X�N�g�p�󋵂��Ď�����֐�
void MonitorDiskUsage() {
    std::cout << "�f�B�X�N�g�p�󋵂��Ď����܂�...\n";

    // �h���C�u�����擾
    wchar_t drive[] = L"C:\\";
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    if (GetDiskFreeSpaceEx(drive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        std::cout << "�f�B�X�N�T�C�Y: " << totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024) << " GB\n";
        std::cout << "�󂫗e��: " << totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024) << " GB\n";
    }
    else {
        std::cerr << "�f�B�X�N�̏��擾�Ɏ��s���܂����B\n";
    }
}

// �l�b�g���[�N����\������֐�
void ShowNetworkInfo() {
    std::cout << "�l�b�g���[�N����\�����܂�...\n";

    // �l�b�g���[�N�A�_�v�^�����擾
    IP_ADAPTER_INFO adapterInfo[16];
    ULONG buflen = sizeof(adapterInfo);
    DWORD dwStatus = GetAdaptersInfo(adapterInfo, &buflen);

    if (dwStatus == NO_ERROR) {
        for (PIP_ADAPTER_INFO pAdapter = adapterInfo; pAdapter != nullptr; pAdapter = pAdapter->Next) {
            std::cout << "�A�_�v�^��: " << pAdapter->AdapterName << "\n";
            std::cout << "IP�A�h���X: " << pAdapter->IpAddressList.IpAddress.String << "\n";
            std::cout << "�T�u�l�b�g: " << pAdapter->IpAddressList.IpMask.String << "\n";
            std::cout << "�Q�[�g�E�F�C: " << pAdapter->GatewayList.IpAddress.String << "\n\n";
        }
    }
    else {
        std::cerr << "�l�b�g���[�N���̎擾�Ɏ��s���܂����B\n";
    }
}

// �p�t�H�[�}���X�����j�^�����O����֐�
void MonitorPerformance() {
    std::cout << "�p�t�H�[�}���X���j�^�����O���J�n���܂�...\n";

    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;
    PDH_FMT_COUNTERVALUE counterVal;

    if (PdhOpenQuery(NULL, 0, &hQuery) == ERROR_SUCCESS) {
        if (PdhAddCounter(hQuery, L"\\Processor(_Total)\\% Processor Time", 0, &hCounter) == ERROR_SUCCESS) {
            PdhCollectQueryData(hQuery);
            PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
            std::cout << "���݂�CPU�g�p��: " << counterVal.doubleValue << "%\n";
            PdhCloseQuery(hQuery);
        }
        else {
            std::cerr << "CPU�g�p���̃J�E���^�ǉ��Ɏ��s���܂����B\n";
        }
    }
    else {
        std::cerr << "�p�t�H�[�}���X�N�G���̏������Ɏ��s���܂����B\n";
    }
}

// CPU���x���Ď�����֐�
void MonitorCPUTemperature() {
    std::cout << "CPU���x�Ď����J�n���܂�...\n";

    // ���ۂ�CPU���x���擾������@�́AWindows API�ɂ͒��ړI�ȃT�|�[�g���Ȃ����߁A
    // �T�[�h�p�[�e�B���C�u������WMI�𗘗p����K�v������܂��B
    // �����ł͉��̃��b�Z�[�W��\�����܂��B
    std::cout << "CPU���x: 55��C (���̒l)\n";
}

// �V�X�e�����ċN������֐�
void RestartSystem() {
    std::cout << "�V�X�e�����ċN�����܂�...\n";
    system("shutdown /r /f");  // Windows�ŃV�X�e�����ċN��
}

// �C�x���g���O��\������֐�
void ShowEventLogs() {
    std::cout << "�C�x���g���O��\�����܂�...\n";
    // �V���v���ȃC�x���g���O�̕\���i�ڍׂȃ��O�\���ɂ�EventLog API�̎g�p���K�v�j
    std::cout << "�C�x���g���O�i���̕\���j: �V�X�e���C�x���g\n";
}

// ����������\������֐�
void ShowMemoryInfo() {
    std::cout << "����������\�����܂�...\n";

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);

    std::cout << "�������̎g�p��: " << memStatus.dwMemoryLoad << "%\n";
    std::cout << "���p�\�ȃ�����: " << memStatus.ullAvailPhys / 1024 / 1024 << " MB\n";
}

// �V�X�e�����V���b�g�_�E������֐�
void ShutdownSystem() {
    std::cout << "�V�X�e�����V���b�g�_�E�����܂�...\n";
    system("shutdown /s /f");  // Windows�ŃV�X�e�����V���b�g�_�E��
}


// ���j���[��\������֐�
void displayMenu() {
    std::cout << "=== ���j���[ ===\n";
    std::cout << "1. �v���Z�X�Ǘ�\n";
    std::cout << "2. �f�B�X�N�g�p��\n";
    std::cout << "3. �l�b�g���[�N���\n";
    std::cout << "4. �p�t�H�[�}���X���j�^�����O\n";
    std::cout << "5. CPU���x�Ď�\n";
    std::cout << "6. �T�[�r�X�Ǘ�\n";
    std::cout << "7. �V�X�e���ċN��\n";
    std::cout << "8. �C�x���g���O\n";
    std::cout << "9. ���������\n";
    std::cout << "10. �V�X�e���V���b�g�_�E��\n";
    std::cout << "�I�����Ă�������: ";
}

// ���j���[�I���ɉ��������������s����֐�
void handleMenuSelection(int choice) {
    switch (choice) {
    case 1:
        ManageProcesses();
        break;
    case 2:
        MonitorDiskUsage();
        break;
    case 3:
        ShowNetworkInfo();
        break;
    case 4:
        MonitorPerformance();
        break;
    case 5:
        MonitorCPUTemperature();
        break;
    case 6:
        ManageServices();
        break;
    case 7:
        RestartSystem();
        break;
    case 8:
        ShowEventLogs();
        break;
    case 9:
        ShowMemoryInfo();
        break;
    case 10:
        ShutdownSystem();
        break;
    default:
        std::cout << "�����ȑI���ł��B\n";
    }
}