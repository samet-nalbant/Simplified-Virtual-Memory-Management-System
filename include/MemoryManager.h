#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>


#include "../include/PageTable.h"
#include "../include/PhysicalMemory.h"
#include "../include/VirtualMemory.h"
#include "../include/LeastRecentlyUsed.h"
#include "../include/WSClock.h"
#include "../include/Constants.h"


using namespace std;


class MemoryManager{

public:
    MemoryManager(int frameSize, int virtualFrameCount, int physicalFrameCount, string diskFileName, int pageReplacementAlgorithm, int pageTablePrintLimit);
    int getValueFromPhysicalMemory(int virtualFrameNumber, int offset);
    void writeValueToPhysicalMemory(int virtualFrameNumber, int offset, int value);
    void printPhysicalMemory();
    void printPageTable();
    void printStatistics();
    ~MemoryManager();
private:
    queue<int> secondChanceQueue;
    LeastRecentlyUsed* leastRecentlyUsed;
    WSClock* wsClock;
    PageTable* pageTable;
    PhysicalMemory* physicalMemory;
    VirtualMemory* virtualMemory;
    int frameSize;
    int virtualFrameCount;
    int physicalFrameCount;
    string diskFileName;
    void initializePageTable();
    void intializePhysicalMemory();
    void initializeDiskFile();
    void initializeVirtualMemory();
    vector<int> getFrameFromDiskFile(int virtualFrameNumber);
    bool writeFrameToDiskFile(int virtualFrameNumber, vector<int> frame);
    void deleteFrameFromFile(int virtualFrameNumber);
    void secondChancePageReplacement(int virtualFrameNumber);
    int deleteFrameFromPhysicalMemory(int virtualFrameNumber); //return empty frame number
    void printSecondChanceQueue();
    void leastRecentlyUsedPageReplacement(int virtualFrameNumber);
    void wsClockPageReplacement(int virtualFrameNumber);
    int wsClockIndex;
    int threshold;
    int pageReplacementAlgorithm;
    int numberOfPageFaults;
    int numberOfReads;
    int numberOfWrites;
    int numberOfDiskPageWrites;
    int numberOfDiskPageReads;
    int numberOfPageReplacements;
    int pageTablePrintLimit;
};






#endif