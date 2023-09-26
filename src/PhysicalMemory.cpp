#include "../include/PhysicalMemory.h"

PhysicalMemory::PhysicalMemory(int frameSize, int totalFrameCount){
    this->frameSize = pow(2,frameSize);
    this->totalFrameCount = pow(2,totalFrameCount);
    for(int i=0;i<this->totalFrameCount;i++){
        vector<int> frame;
        physicalMemory.push_back(frame);
    }
}

int PhysicalMemory::getFrameSize(){
    return frameSize;
}

int PhysicalMemory::getTotalFrameCount(){
    return totalFrameCount;
}

int *PhysicalMemory::getValueAt(int frameNumber, int frameIndex){
    return &physicalMemory[frameNumber][frameIndex];
}

bool PhysicalMemory::setFrame(int frameNumber, vector<int> frame){
    if(frameNumber >= totalFrameCount || frameNumber < 0)
        return false;
    physicalMemory[frameNumber] = frame;
    return true;
}

vector<int> PhysicalMemory::getFrame(int frameNumber){
    if(frameNumber >= totalFrameCount || frameNumber < 0)
        return vector<int>();
    return physicalMemory[frameNumber];
}

void PhysicalMemory::printPhysicalMemory(){
    for(int i=0;i<totalFrameCount;i++){
        cout << "Physical Frame " << i << ": ";
        for(int j=0;j<frameSize;j++){
            cout << physicalMemory[i][j] << " ";
        }
        cout << endl;
    }
}

void PhysicalMemory::setValue(int frameNumber, int frameIndex, int value){
    physicalMemory[frameNumber][frameIndex] = value;
}