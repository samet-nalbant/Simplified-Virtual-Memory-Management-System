#include "../include/PageTable.h"

PageTable::PageTable(int frameCount){
    this->frameCount = pow(2,frameCount);
    for(int i = 0; i < pow(2,frameCount); i++){
        pageTable.push_back(PageTableEntry());
    }
}

int PageTable::getPhysicalFrameNumber(int virtualFrameNumber){
    if(virtualFrameNumber >= frameCount || virtualFrameNumber < 0)
        return OUT_OF_BOUNDS;
    return pageTable[virtualFrameNumber].getPageFrameNumber();
}

int PageTable::getFrameCount(){
    return frameCount;
}

bool PageTable::setPhysicalFrameNumber(int virtualFrameNumber, int physicalFrameNumber){
    if(virtualFrameNumber >= frameCount || virtualFrameNumber < 0)
        return false;
    pageTable[virtualFrameNumber].setPageFrameNumber(physicalFrameNumber);
    return true;
}

void PageTable::printPageTable(){
    cout << "Virtual Frame Number\tPhysical Frame Number\tPresent Bit\t Modified Bit\t\tReferenced Bit" << endl;
    for(int i = 0; i < frameCount; i++){
        pageTable[i].getPageFrameNumber() == NOT_PRESENT ? cout << i << "\t\t\t" << "-" 
        << "\t\t\t" << pageTable[i].isPresent() << "\t\t\t" << pageTable[i].isModified() << "\t\t\t" << pageTable[i].isReferenced()
        << endl : cout << i << "\t\t\t" << pageTable[i].getPageFrameNumber() << 
        "\t\t\t" << pageTable[i].isPresent() << "\t\t\t" << pageTable[i].isModified() << "\t\t\t" << pageTable[i].isReferenced() << endl;
    }
}

bool PageTable::isPresent(int virtualFrameNumber){
    if(virtualFrameNumber >= frameCount || virtualFrameNumber < 0)
        return false;
    return pageTable[virtualFrameNumber].isPresent();
}

bool PageTable::setPresentBit(int virtualFrameNumber, bool present){
    if(virtualFrameNumber >= frameCount || virtualFrameNumber < 0)
        return false;
    pageTable[virtualFrameNumber].setPresentBit(present);
    return true;
}

bool PageTable::isModified(int virtualFrameNumber){
    return pageTable[virtualFrameNumber].isModified();
}
bool PageTable::isReferenced(int virtualFrameNumber){
    return pageTable[virtualFrameNumber].isReferenced();
}
void PageTable::setModified(int virtualFrameNumber, bool modified){
    pageTable[virtualFrameNumber].setModified(modified);
}
void PageTable::setReferenced(int virtualFrameNumber, bool referenced){
    pageTable[virtualFrameNumber].setReferenced(referenced);
}
void PageTable::clearPageTableEntry(int virtualFrameNumber){
    pageTable[virtualFrameNumber].setPageFrameNumber(NOT_PRESENT);
    pageTable[virtualFrameNumber].setPresentBit(false);
    pageTable[virtualFrameNumber].setModified(false);
    pageTable[virtualFrameNumber].setReferenced(false);
}
void PageTable::initializePageTableEntry(int virtualFrameNumber, int physicalFrameNumber){
    pageTable[virtualFrameNumber].setPageFrameNumber(physicalFrameNumber);
    pageTable[virtualFrameNumber].setPresentBit(true);
    pageTable[virtualFrameNumber].setModified(false);
    pageTable[virtualFrameNumber].setReferenced(false);
}
int PageTable::getVirtualFrameNumber(int physicalFrameNumber){
    for(int i = 0; i < frameCount; i++){
        if(pageTable[i].getPageFrameNumber() == physicalFrameNumber)
            return i;
    }
    return NOT_PRESENT;
}