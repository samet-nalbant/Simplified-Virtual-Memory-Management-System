#ifndef PHYSICALMEMORY_H
#define PHYSICALMEMORY_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class PhysicalMemory
{
public:

    PhysicalMemory(int frameSize, int totalFrameCount);
    int getFrameSize();
    int getTotalFrameCount();
    int *getValueAt(int frameNumber, int frameIndex);
    bool setFrame(int frameNumber, vector<int> frame);
    void printPhysicalMemory();
    vector<int> getFrame(int frameNumber);
    void setValue(int frameNumber, int frameIndex, int value);
private:

    int frameSize; // 2^12 = 4096 integer 
    int totalFrameCount; // 2^5 = 32 frame -> 32 * 4096 = 131072 integer
    vector<vector<int>> physicalMemory; // 2D vector -> 32 * 4096 = 131072 integer

};





#endif