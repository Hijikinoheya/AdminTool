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

// プロセス管理を行う関数
void ManageServices() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        std::cerr << "サービスマネージャを開けません。エラーコード: " << GetLastError() << "\n";
        return;
    }

    DWORD bytesNeeded = 0, servicesReturned = 0, resumeHandle = 0;
    ENUM_SERVICE_STATUSW* serviceStatus = nullptr;

    // 必要なバッファサイズを取得
    EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, NULL, 0, &bytesNeeded, &servicesReturned, &resumeHandle);

    if (!EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, serviceStatus, bytesNeeded, &bytesNeeded, &servicesReturned, &resumeHandle)) {
        std::cerr << "EnumServicesStatusW に失敗しました。エラーコード: " << GetLastError() << "\n";
        free(serviceStatus);
        CloseServiceHandle(hSCManager);
        return;
    }


    // 必要なメモリを確保
    serviceStatus = (ENUM_SERVICE_STATUSW*)malloc(bytesNeeded);
    if (!serviceStatus) {
        std::cerr << "メモリ割り当てに失敗しました。\n";
        CloseServiceHandle(hSCManager);
        return;
    }

    // サービス情報を取得
    if (!EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_ACTIVE, serviceStatus, bytesNeeded, &bytesNeeded, &servicesReturned, &resumeHandle)) {
        std::cerr << "サービス情報の取得に失敗しました。エラーコード: " << GetLastError() << "\n";
        free(serviceStatus);
        CloseServiceHandle(hSCManager);
        return;
    }

    // サービス情報を表示
    std::cout << "=== サービス情報 ===\n";
    for (DWORD i = 0; i < servicesReturned; i++) {
        std::wcout << L"サービス名: " << serviceStatus[i].lpServiceName << L"\n";
    }

    // メモリ解放とハンドルのクローズ
    free(serviceStatus);
    CloseServiceHandle(hSCManager);
}

// ディスク使用状況を監視する関数
void MonitorDiskUsage() {
    std::cout << "ディスク使用状況を監視します...\n";

    // ドライブ情報を取得
    wchar_t drive[] = L"C:\\";
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

    if (GetDiskFreeSpaceEx(drive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        std::cout << "ディスクサイズ: " << totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024) << " GB\n";
        std::cout << "空き容量: " << totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024) << " GB\n";
    }
    else {
        std::cerr << "ディスクの情報取得に失敗しました。\n";
    }
}

// ネットワーク情報を表示する関数
void ShowNetworkInfo() {
    std::cout << "ネットワーク情報を表示します...\n";

    // ネットワークアダプタ情報を取得
    IP_ADAPTER_INFO adapterInfo[16];
    ULONG buflen = sizeof(adapterInfo);
    DWORD dwStatus = GetAdaptersInfo(adapterInfo, &buflen);

    if (dwStatus == NO_ERROR) {
        for (PIP_ADAPTER_INFO pAdapter = adapterInfo; pAdapter != nullptr; pAdapter = pAdapter->Next) {
            std::cout << "アダプタ名: " << pAdapter->AdapterName << "\n";
            std::cout << "IPアドレス: " << pAdapter->IpAddressList.IpAddress.String << "\n";
            std::cout << "サブネット: " << pAdapter->IpAddressList.IpMask.String << "\n";
            std::cout << "ゲートウェイ: " << pAdapter->GatewayList.IpAddress.String << "\n\n";
        }
    }
    else {
        std::cerr << "ネットワーク情報の取得に失敗しました。\n";
    }
}

// パフォーマンスをモニタリングする関数
void MonitorPerformance() {
    std::cout << "パフォーマンスモニタリングを開始します...\n";

    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;
    PDH_FMT_COUNTERVALUE counterVal;

    if (PdhOpenQuery(NULL, 0, &hQuery) == ERROR_SUCCESS) {
        if (PdhAddCounter(hQuery, L"\\Processor(_Total)\\% Processor Time", 0, &hCounter) == ERROR_SUCCESS) {
            PdhCollectQueryData(hQuery);
            PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
            std::cout << "現在のCPU使用率: " << counterVal.doubleValue << "%\n";
            PdhCloseQuery(hQuery);
        }
        else {
            std::cerr << "CPU使用率のカウンタ追加に失敗しました。\n";
        }
    }
    else {
        std::cerr << "パフォーマンスクエリの初期化に失敗しました。\n";
    }
}

// CPU温度を監視する関数
void MonitorCPUTemperature() {
    std::cout << "CPU温度監視を開始します...\n";

    // 実際のCPU温度を取得する方法は、Windows APIには直接的なサポートがないため、
    // サードパーティライブラリやWMIを利用する必要があります。
    // ここでは仮のメッセージを表示します。
    std::cout << "CPU温度: 55°C (仮の値)\n";
}

// システムを再起動する関数
void RestartSystem() {
    std::cout << "システムを再起動します...\n";
    system("shutdown /r /f");  // Windowsでシステムを再起動
}

// イベントログを表示する関数
void ShowEventLogs() {
    std::cout << "イベントログを表示します...\n";
    // シンプルなイベントログの表示（詳細なログ表示にはEventLog APIの使用が必要）
    std::cout << "イベントログ（仮の表示）: システムイベント\n";
}

// メモリ情報を表示する関数
void ShowMemoryInfo() {
    std::cout << "メモリ情報を表示します...\n";

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);

    std::cout << "メモリの使用量: " << memStatus.dwMemoryLoad << "%\n";
    std::cout << "利用可能なメモリ: " << memStatus.ullAvailPhys / 1024 / 1024 << " MB\n";
}

// システムをシャットダウンする関数
void ShutdownSystem() {
    std::cout << "システムをシャットダウンします...\n";
    system("shutdown /s /f");  // Windowsでシステムをシャットダウン
}


// メニューを表示する関数
void displayMenu() {
    std::cout << "=== メニュー ===\n";
    std::cout << "1. プロセス管理\n";
    std::cout << "2. ディスク使用状況\n";
    std::cout << "3. ネットワーク情報\n";
    std::cout << "4. パフォーマンスモニタリング\n";
    std::cout << "5. CPU温度監視\n";
    std::cout << "6. サービス管理\n";
    std::cout << "7. システム再起動\n";
    std::cout << "8. イベントログ\n";
    std::cout << "9. メモリ情報\n";
    std::cout << "10. システムシャットダウン\n";
    std::cout << "選択してください: ";
}

// メニュー選択に応じた処理を実行する関数
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
        std::cout << "無効な選択です。\n";
    }
}