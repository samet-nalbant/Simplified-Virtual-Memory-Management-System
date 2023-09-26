#ifndef WSCLOCK_H
#define WSCLOCK_H


#include <iostream>
#include <ctime>

using namespace std;


class WSClockEntry
{

private:
    int virtualFrameNumber;
    time_t lastAccessTime;

public:
    WSClockEntry(int virtualFrameNumber){
        this->virtualFrameNumber = virtualFrameNumber;
        this->lastAccessTime = time(0);
    }
    int getVirtualFrameNumber(){
        return this->virtualFrameNumber;
    }
    time_t getLastAccessTime(){
        return this->lastAccessTime;
    }
    void setLastAccessTime(){
        this->lastAccessTime = time(0);
    }
};

class WSClock
{
public:
    WSClock(int physicalFrameCount){
        for(int i = 0; i < physicalFrameCount; i++){
            WSClockEntry wsClockEntry(-1);
            circularClockQueue.push_back(wsClockEntry);
        }
    }

    void setWSClockEntry(int physicalFrameNumber, int virtualFrameNumber){
        circularClockQueue[physicalFrameNumber] = WSClockEntry(virtualFrameNumber);
    }

    int getWSClockEntryVirtualFrameNumber(int physicalFrameNumber){
        return circularClockQueue[physicalFrameNumber].getVirtualFrameNumber();
    }

    time_t getWSClockEntryLastAccessTime(int physicalFrameNumber){
        return circularClockQueue[physicalFrameNumber].getLastAccessTime();
    }

    void updateLastAccessTime(int physicalFrameNumber){
        circularClockQueue[physicalFrameNumber].setLastAccessTime();
    }

    void printWSClock(){
        cout << "WSClock: ";
        for(int i = 0; i < physicalFrameCount; i++){
            cout << circularClockQueue[i].getVirtualFrameNumber() << " ";
        }
        cout << endl;
    }
    

private:

    int physicalFrameCount;
    vector<WSClockEntry> circularClockQueue;


};





#endif // WSCLOCK_H