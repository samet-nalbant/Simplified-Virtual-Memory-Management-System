#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class VirtualMemory
{

private:
    vector<vector<int>> virtualMemory;
    int frameSize;
    int totalFrameCount;
public:
    VirtualMemory(int frameSize, int totalFrameCount);
    int *getValueAt(int frameNumber, int frameIndex);
    vector<int> getFrame(int frameNumber);
    bool setFrame(int frameNumber, vector<int> frame);

};





#endif