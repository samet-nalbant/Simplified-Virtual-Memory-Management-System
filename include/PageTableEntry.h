#ifndef PAGETABLEENTRY_H
#define PAGETABLEENTRY_H

#include <iostream>
using namespace std;

class PageTableEntry
{
private:
    int pageFrameNumber;
    bool present;
    bool modified;
    bool referenced;

public:
    PageTableEntry();
    PageTableEntry(int pageFrameNumber);
    int getPageFrameNumber();
    void setPageFrameNumber(int pageFrameNumber);
    bool isPresent();
    void setPresentBit(bool present);
    bool isModified();
    void setModified(bool modified);
    bool isReferenced();
    void setReferenced(bool referenced);
    string toString();
};

#endif // PAGETABLEENTRY_H
