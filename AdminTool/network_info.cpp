#include "pch.h"
#include "network_info.h"

void ShowNetworkInfo() {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwBufLen = sizeof(adapterInfo);
    DWORD dwStatus = GetAdaptersInfo(adapterInfo, &dwBufLen);

    if (dwStatus == ERROR_SUCCESS) {
        for (PIP_ADAPTER_INFO pAdapterInfo = adapterInfo; pAdapterInfo != NULL; pAdapterInfo = pAdapterInfo->Next) {
            std::cout << "Adapter Name: " << pAdapterInfo->AdapterName << std::endl;
            std::cout << "IP Address: " << pAdapterInfo->IpAddressList.IpAddress.String << std::endl;
        }
    }
    else {
        std::cerr << "Error: Could not retrieve network information." << std::endl;
    }
}
