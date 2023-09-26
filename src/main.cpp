#include <iostream>
#include <cstring>
#include <string>

#include "../include/PageTableEntry.h"
#include "../include/PageTable.h"
#include "../include/PhysicalMemory.h"
#include "../include/VirtualMemory.h"
#include "../include/MemoryManager.h"

using namespace std;


void matrixMultiplicationOperation(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount);

void linearSearchOperation(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount, int targetValue);

void binarySearchOperation(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount, int targetValue);

void arraySummationOpeartion(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount);

void testAlgorithms();


int main(int argc , char *argv[])
{


    /*
    testAlgorithms();
    return 0;

    */

    //./main 5 2 3 LRU inverted 10000 diskFileName.dat

    if(argc != 8){
        cout << "Invalid number of arguments" << endl;
        return 0;
    }

    //int frameSize = 5, virtualFrameCount = 3, physicalFrameCount = 2;
    int selectedPageReplacementAlgorithm = LEAST_RECENTLY_USED_PAGE_REPLACEMENT;
    int frameSize = stoi(argv[1]);
    int physicalFrameCount = stoi(argv[2]);
    int virtualFrameCount = stoi(argv[3]);
    char *pageReplacementAlgorithm = argv[4];
    char *pageTableType = argv[5];
    int pageTablePrintLimit = stoi(argv[6]);
    char *diskFileName = argv[7];

    if(strcmp(pageReplacementAlgorithm, "LRU") == 0){
        selectedPageReplacementAlgorithm = LEAST_RECENTLY_USED_PAGE_REPLACEMENT;
    }
    else if(strcmp(pageReplacementAlgorithm, "WSClock") == 0){
        selectedPageReplacementAlgorithm = WS_CLOCK_PAGE_REPLACEMENT;
    }
    else if(strcmp(pageReplacementAlgorithm, "SecondChance") == 0){
        selectedPageReplacementAlgorithm = SECOND_CHANCE_PAGE_REPLACEMENT;
    }
    else{
        cout << "Invalid page replacement algorithm" << endl;
        return 0;
    }

    if(virtualFrameCount < physicalFrameCount){
        cout << "Virtual Frame count should be greater than physical frame count!" << endl;
        return 0;
    }

    cout << "Frame Size: " << frameSize << endl;
    cout << "Virtual Frame Count: " << virtualFrameCount << endl;
    cout << "Physical Frame Count: " << physicalFrameCount << endl;
    switch (selectedPageReplacementAlgorithm)
    {
        case LEAST_RECENTLY_USED_PAGE_REPLACEMENT:
            cout << "pageReplacementAlgorithm: LRU" << endl;
            break;
        case WS_CLOCK_PAGE_REPLACEMENT:
            cout << "pageReplacementAlgorithm: WSClock" << endl;
            break;
        case SECOND_CHANCE_PAGE_REPLACEMENT:
            cout << "pageReplacementAlgorithm: SecondChance" << endl;
            break;
        default:
            break;
    }

    cout << "-----------------------------" << endl;
    MemoryManager *memoryManager = new MemoryManager(frameSize, virtualFrameCount, physicalFrameCount, 
        diskFileName, selectedPageReplacementAlgorithm, pageTablePrintLimit);

    frameSize = pow(2, frameSize);
    virtualFrameCount = pow(2, virtualFrameCount);
    physicalFrameCount = pow(2, physicalFrameCount);

    cout << "-----------------------------" << endl;

    linearSearchOperation(memoryManager, frameSize, virtualFrameCount, physicalFrameCount, -3);

    cout << "-----------------------------" << endl;

    arraySummationOpeartion(memoryManager, frameSize, virtualFrameCount, physicalFrameCount);

    cout << "-----------------------------" << endl;

    matrixMultiplicationOperation(memoryManager, frameSize, virtualFrameCount, physicalFrameCount);

    cout << "-----------------------------" << endl;

    memoryManager->printStatistics();



    //binarySearchOperation(memoryManager, frameSize, virtualFrameCount, physicalFrameCount, 31);



    return 0;
}


void matrixMultiplicationOperation(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount){

    /*
        3-    1 2 3 4 5 |
        2-    1 2 3 4 5 |   -> matrix
        1-    1 2 3 4 5 |

        0-    1 2 3 4 5 -> vector

    */

    int vectorSize = virtualFrameCount / 4;
    //matrix
    int vectorIndex = 0;
    for(int i=vectorSize; i< virtualFrameCount; i++){
        if(vectorIndex >= vectorSize)
            vectorIndex = 0;
        for(int j=0; j<frameSize; j++){
            int result = memoryManager->getValueFromPhysicalMemory(i, j) * memoryManager->getValueFromPhysicalMemory(vectorIndex, j);
            memoryManager->writeValueToPhysicalMemory(i, j, result);
        }
        vectorIndex++;
    }

    cout << "---------------------------" << endl;

    vector<int> resultVector;
    vectorIndex = 0;
    int sum = 0;
    for(int i=vectorSize; i< virtualFrameCount; i++){
        if(vectorIndex >= vectorSize){
            resultVector.push_back(sum);
            sum = 0;
            vectorIndex = 0;
        }
        for(int j=0; j<frameSize; j++){
            sum += memoryManager->getValueFromPhysicalMemory(i, j);
        }
        vectorIndex++;
    }
    resultVector.push_back(sum);
    cout  << "Result Matrix: " << endl;
    for(int i=0; i<3; i++){
        cout << resultVector[i] << endl;
    }

    cout << "---------------------------" << endl;
    memoryManager->printPageTable();
}

void linearSearchOperation(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount, int targetValue){
    int count = -1;
    bool isFound = false;
    for(int i=0; i < virtualFrameCount; i++){
        if(isFound)
            break;
        for(int j=0; j < frameSize; j++){
            int readedValue = memoryManager->getValueFromPhysicalMemory(i, j);
            if(readedValue == targetValue){
                count++;
                isFound = true;
                break;
            }
            count++;
        }
    }
    if(!isFound)
        cout << "Target value not found!" << endl;
    else
        cout << "Target value found at index: " << count << endl;
}


void binarySearchOperation(MemoryManager* memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount, int targetValue) {

    for(int i=0; i < virtualFrameCount; i++){
        for(int j=0; j < frameSize; j++){
            for(int k=0; k < virtualFrameCount; k++){
                for(int l=0; l < frameSize; l++){
                    if(memoryManager->getValueFromPhysicalMemory(i, j) < memoryManager->getValueFromPhysicalMemory(k, l)){
                        int temp = memoryManager->getValueFromPhysicalMemory(i, j);
                        memoryManager->writeValueToPhysicalMemory(i, j, memoryManager->getValueFromPhysicalMemory(k, l));
                        memoryManager->writeValueToPhysicalMemory(k, l, temp);
                    }
                }
            }
        }
    }

    int left = 0;
    int right = virtualFrameCount * frameSize - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int row = mid / frameSize;
        int column = mid % frameSize;

        int readedValue = memoryManager->getValueFromPhysicalMemory(row, column);

        if (readedValue == targetValue) {
            cout << "Target value found at index: " << mid << endl;
            return;
        } else if (readedValue < targetValue) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    cout << "Target value not found!" << endl;
}

void arraySummationOpeartion(MemoryManager *memoryManager, int frameSize, int virtualFrameCount, int physicalFrameCount){
    int sum = 0;
    for(int i=0; i < virtualFrameCount; i++){
        for(int j=0; j < frameSize; j++){
            sum += memoryManager->getValueFromPhysicalMemory(i, j);
        }
    }
    cout << "Sum: " << sum << endl;
}

void testAlgorithms(){

    int frameSize = 5, virtualFrameCount = 3, physicalFrameCount = 2;

    MemoryManager *memoryManagerLRU = new MemoryManager(frameSize, virtualFrameCount, physicalFrameCount, 
        "test1.dat", LEAST_RECENTLY_USED_PAGE_REPLACEMENT, 1000000);
    
    MemoryManager *memoryManagerSecondChance = new MemoryManager(frameSize, virtualFrameCount, physicalFrameCount, 
        "test2.dat", SECOND_CHANCE_PAGE_REPLACEMENT, 1000000);
    MemoryManager *memoryManagerWSClock = new MemoryManager(frameSize, virtualFrameCount, physicalFrameCount, 
        "test3.dat", WS_CLOCK_PAGE_REPLACEMENT, 1000000);

    frameSize = pow(2, frameSize);
    virtualFrameCount = pow(2, virtualFrameCount);
    physicalFrameCount = pow(2, physicalFrameCount);

    //linearSearchOperation(memoryManagerLRU, frameSize, virtualFrameCount, physicalFrameCount, -3);
    //linearSearchOperation(memoryManagerSecondChance, frameSize, virtualFrameCount, physicalFrameCount, -3);
    //linearSearchOperation(memoryManagerWSClock, frameSize, virtualFrameCount, physicalFrameCount, -3);

    //arraySummationOpeartion(memoryManagerLRU, frameSize, virtualFrameCount, physicalFrameCount);
    //arraySummationOpeartion(memoryManagerSecondChance, frameSize, virtualFrameCount, physicalFrameCount);
    //arraySummationOpeartion(memoryManagerWSClock, frameSize, virtualFrameCount, physicalFrameCount);


    matrixMultiplicationOperation(memoryManagerLRU, frameSize, virtualFrameCount, physicalFrameCount);
    matrixMultiplicationOperation(memoryManagerSecondChance, frameSize, virtualFrameCount, physicalFrameCount);

    //binarySearchOperation(memoryManagerLRU, frameSize, virtualFrameCount, physicalFrameCount, 18);
    //binarySearchOperation(memoryManagerSecondChance, frameSize, virtualFrameCount, physicalFrameCount, 18);

    cout << "Least Recently Used Statistics: " << endl;
    memoryManagerLRU->printStatistics();
    cout << "-----------------------------------" << endl;
    cout << "Second Chance Statistics: " << endl;
    memoryManagerSecondChance->printStatistics();
    cout << "-----------------------------------" << endl;
    cout << "WS Clock Statistics: " << endl;
    memoryManagerWSClock->printStatistics();
    cout << "-----------------------------------" << endl;

}
