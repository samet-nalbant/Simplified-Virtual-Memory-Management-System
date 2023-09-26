#include "../include/VirtualMemory.h"

VirtualMemory::VirtualMemory(int frameSize, int totalFrameCount){
    this->frameSize = pow(2,frameSize);
    this->totalFrameCount = pow(2,totalFrameCount);
    for(int i=0;i<this->totalFrameCount;i++){
        vector<int> frame;
        virtualMemory.push_back(frame);
    }
}

int *VirtualMemory::getValueAt(int frameNumber, int frameIndex){
    return &virtualMemory[frameNumber][frameIndex];
}

vector<int> VirtualMemory::getFrame(int frameNumber){
    if(frameNumber >= totalFrameCount || frameNumber < 0)
        return vector<int>();
    return virtualMemory[frameNumber];
}

bool VirtualMemory::setFrame(int frameNumber, vector<int> frame){
    if(frameNumber >= totalFrameCount || frameNumber < 0)
        return false;
    virtualMemory[frameNumber] = frame;
    return true;
}