#include "pch.h"
#include "performance_monitor.h"

void MonitorPerformance() {
    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;

    PdhOpenQuery(NULL, NULL, &hQuery);
    PdhAddCounter(hQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &hCounter);

    PdhCollectQueryData(hQuery);
    PDH_FMT_COUNTERVALUE counterValue;
    PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &counterValue);

    std::cout << "CPU Usage: " << counterValue.longValue << "%" << std::endl;

    PdhCloseQuery(hQuery);
}
