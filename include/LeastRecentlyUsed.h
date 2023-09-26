#ifndef LEASTRECENTLYUSED_H
#define LEASTRECENTLYUSED_H

#include <iostream>
#include <vector>

using namespace std;

class LRUEntry
{
public:
    LRUEntry(int virtualFrameNumber){
        this->virtualFrameNumber = virtualFrameNumber;
        this->counter = 0;
    }

    int getVirtualFrameNumber(){
        return this->virtualFrameNumber;
    }

    int getCounter(){
        return this->counter;
    }

    void incrementCounter(){
        this->counter++;
    }

    void resetCounter(){
        this->counter = 0;
    }

    void setVirtualFrameNumber(int virtualFrameNumber){
        this->virtualFrameNumber = virtualFrameNumber;
    }

private:
    int virtualFrameNumber;
    int counter;
};


class LeastRecentlyUsed
{

private:
    int physicalFrameCount;
    vector<LRUEntry> lruEntries;

public:

    LeastRecentlyUsed(int physicalFrameCount){
        this->physicalFrameCount = physicalFrameCount;
        for(int i = 0; i < physicalFrameCount; i++){
            LRUEntry lruEntry(-1);
            lruEntries.push_back(lruEntry);
        }
    }

    void setLRUEntry(int physicalFrameNumber, int virtualFrameNumber){
        lruEntries[physicalFrameNumber] = LRUEntry(virtualFrameNumber);
    }

    void setLRUEntryVirtualFrameNumber(int physicalFrameNumber, int virtualFrameNumber){
        lruEntries[physicalFrameNumber].setVirtualFrameNumber(virtualFrameNumber);
    }

    void incrementLRUEntryCounter(int physicalFrameNumber){
        lruEntries[physicalFrameNumber].incrementCounter();
    }

    void resetLRUEntryCounter(int physicalFrameNumber){
        lruEntries[physicalFrameNumber].resetCounter();
    }

    int getLeastRecentlyUsedVirtualFrameNumber(){
        int min = 0;
        for(int i = 0; i < physicalFrameCount; i++){
            if(lruEntries[i].getCounter() < lruEntries[min].getCounter()){
                min = i;
            }
        }
        return lruEntries[min].getVirtualFrameNumber();
    }

    int getPhysicalFrameNumber(int virtualFrameNumber){
        for(int i = 0; i < physicalFrameCount; i++){
            if(lruEntries[i].getVirtualFrameNumber() == virtualFrameNumber){
                return i;
            }
        }
        return -1;
    }

    void printLeastRecentlyUsedEntries(){
        cout << "Physical Frame Count: " << physicalFrameCount << endl;
        cout << "Least Recently Used Entries: " << endl;
        for(int i = 0; i < physicalFrameCount; i++){
            cout << "Virtual Frame Number: " << lruEntries[i].getVirtualFrameNumber() << endl;
        }
        cout << "-----------------------------" << endl;
    }

};


#endif