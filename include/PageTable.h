#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#include "../include/PageTableEntry.h"

using namespace std;

#define NOT_PRESENT -1
#define OUT_OF_BOUNDS -2


class PageTable{

private:
    int frameCount;
    //virtual frame number, physical frame number
    vector<PageTableEntry> pageTable;

public:

    int getPhysicalFrameNumber(int virtualFrameNumber);
    int getFrameCount();
    bool setPhysicalFrameNumber(int virtualFrameNumber, int physicalFrameNumber);
    PageTable(int frameCount);
    void printPageTable();
    bool isPresent(int virtualFrameNumber);
    bool setPresentBit(int virtualFrameNumber, bool present);
    bool isModified(int virtualFrameNumber);
    bool isReferenced(int virtualFrameNumber);
    void setModified(int virtualFrameNumber, bool modified);
    void setReferenced(int virtualFrameNumber, bool referenced);
    void clearPageTableEntry(int virtualFrameNumber);
    void initializePageTableEntry(int virtualFrameNumber, int physicalFrameNumber);
    int getVirtualFrameNumber(int physicalFrameNumber);

};






#endif