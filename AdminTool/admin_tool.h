#pragma once

void displayMenu();

// メニュー選択処理
void handleMenuSelection(int choice);

// サービス管理
void ManageServices();

// 他の関数宣言（必要に応じて追加）
void ManageProcesses();
void MonitorDiskUsage();
void ShowNetworkInfo();
void MonitorPerformance();
void MonitorCPUTemperature();
void RestartSystem();
void ShowEventLogs();
void ShowMemoryInfo();
void ShutdownSystem();

#endif // ADMIN_TOOL_H
