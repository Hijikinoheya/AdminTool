#pragma once

void displayMenu();

// ���j���[�I������
void handleMenuSelection(int choice);

// �T�[�r�X�Ǘ�
void ManageServices();

// ���̊֐��錾�i�K�v�ɉ����Ēǉ��j
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
