#include "../include/MemoryManager.h"


MemoryManager::MemoryManager(int frameSize, int virtualFrameCount, int physicalFrameCount, string diskFileName, int pageReplacementAlgorithm, int pageTablePrintLimit){

    this->frameSize = pow(2, frameSize);
    this->virtualFrameCount = pow(2, virtualFrameCount);
    this->physicalFrameCount = pow(2, physicalFrameCount);
    this->pageReplacementAlgorithm = pageReplacementAlgorithm;
    this->diskFileName = diskFileName;
    this->pageTable = new PageTable(virtualFrameCount);
    this->physicalMemory = new PhysicalMemory(frameSize, physicalFrameCount);
    this->virtualMemory = new VirtualMemory(frameSize, virtualFrameCount);
    this->leastRecentlyUsed = new LeastRecentlyUsed(this->physicalFrameCount);
    this->wsClock = new WSClock(this->physicalFrameCount);
    this->wsClockIndex = 0;
    this->threshold = 100;
    this->numberOfPageFaults = 0;
    this->numberOfReads = 0;
    this->numberOfWrites = 0;
    this->numberOfDiskPageWrites = 0;
    this->numberOfDiskPageReads = 0;
    this->numberOfPageReplacements = 0;
    initializePageTable();
    intializePhysicalMemory();
    initializeDiskFile();
    initializeVirtualMemory();
    this->pageTablePrintLimit = pageTablePrintLimit;
    //printSecondChanceQueue();
}

MemoryManager::~MemoryManager(){
    delete this->pageTable;
    delete this->physicalMemory;
    delete this->virtualMemory;
}

void MemoryManager::initializePageTable(){
    vector<int> virtualFrames;
    for (int i = 0; i < this->virtualFrameCount; i++) {
        virtualFrames.push_back(i);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(virtualFrames.begin(), virtualFrames.end(), g);

    for (int i = 0; i < this->physicalFrameCount; i++) {
        int virtualFrameNumber = virtualFrames[i];
        this->pageTable->setPhysicalFrameNumber(virtualFrameNumber, i);
        this->pageTable->setPresentBit(virtualFrameNumber,true);
        secondChanceQueue.push(virtualFrameNumber);
        leastRecentlyUsed->setLRUEntry(i, virtualFrameNumber);
        wsClock->setWSClockEntry(i, virtualFrameNumber);
    }

    this->pageTable->printPageTable();
}

void MemoryManager::intializePhysicalMemory(){
    vector<int> frame;
    for (int j = 0; j < this->frameSize; j++) {
        frame.push_back(j);
        numberOfWrites++;
    }
    for (int i = 0; i < this->physicalFrameCount; i++) {
        this->physicalMemory->setFrame(i, frame);
    }
    //this->physicalMemory->printPhysicalMemory();
}

void MemoryManager::initializeDiskFile(){
    ofstream diskFile;
    diskFile.open(this->diskFileName);
    for(int i = 0; i < this->virtualFrameCount; i++){
        if(pageTable->getPhysicalFrameNumber(i) != -1)
            continue;
        else{
            diskFile << i << " ";
            for(int j = 0; j < this->frameSize; j++){
                diskFile << j << " ";
            }
            diskFile << endl;
            this->numberOfDiskPageWrites++;
        }
    }
    diskFile.close();
}

vector<int> MemoryManager::getFrameFromDiskFile(int virtualFrameNumber){
    ifstream inputFile(diskFileName);
    vector<int> integers;
    int frameNumber;
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            if (iss >> frameNumber) {
                if (frameNumber == virtualFrameNumber) {
                    int num;
                    while (iss >> num) {
                        integers.push_back(num);
                    }
                    break;
                }
            }
        }
        inputFile.close();
    }
    deleteFrameFromFile(virtualFrameNumber);
    numberOfDiskPageReads++;
    return integers;
}

void MemoryManager::deleteFrameFromFile(int virtualFrameNumber) {
    ifstream inputFile(diskFileName);
    ofstream tempFile("temp.txt");
    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            int frameNumber;
            if (iss >> frameNumber) {
                if (frameNumber != virtualFrameNumber) {
                    tempFile << line << endl;
                }
            }
        }
        inputFile.close();
        tempFile.close();

        remove(this->diskFileName.c_str());
        rename("temp.txt", diskFileName.c_str());
    }
}

bool MemoryManager::writeFrameToDiskFile(int virtualFrameNumber, vector<int> frame){
    numberOfDiskPageWrites++;
    ofstream outputFile(diskFileName, ios::app);
    if (outputFile.is_open()) {
        outputFile << virtualFrameNumber << " ";
        for (auto i : frame) {
            outputFile << i << " ";
        }
        outputFile << endl;
        outputFile.close();
        return true;
    }
    return false;
}

void MemoryManager::initializeVirtualMemory(){
    for(int i = 0; i < this->virtualFrameCount; i++){
        vector<int> frame;
        for(int j = 0; j < this->frameSize; j++){
            frame.push_back(j);
        }
        this->virtualMemory->setFrame(i, frame);
    }
}

void MemoryManager::secondChancePageReplacement(int virtualFrameNumber){
    int frontPage = secondChanceQueue.front();
    while(pageTable->isReferenced(frontPage)){
        pageTable->setReferenced(frontPage, false);
        secondChanceQueue.pop();
        secondChanceQueue.push(frontPage);
        frontPage = secondChanceQueue.front();
    }

    secondChanceQueue.pop();
    secondChanceQueue.push(virtualFrameNumber);
    
    int physicalFrameNumberWillBeReplaced = deleteFrameFromPhysicalMemory(frontPage);
    vector<int> frameFromFile = getFrameFromDiskFile(virtualFrameNumber);
    physicalMemory->setFrame(physicalFrameNumberWillBeReplaced, frameFromFile);
    pageTable->initializePageTableEntry(virtualFrameNumber, physicalFrameNumberWillBeReplaced);

}
int MemoryManager::deleteFrameFromPhysicalMemory(int virtualFrameNumber){
    int physicalFrameNumberWillBeDeleted = pageTable->getPhysicalFrameNumber(virtualFrameNumber);
    vector<int> physicalFrameWillBeDeleted = physicalMemory->getFrame(physicalFrameNumberWillBeDeleted);
    pageTable->clearPageTableEntry(virtualFrameNumber);
    writeFrameToDiskFile(virtualFrameNumber, physicalFrameWillBeDeleted);
    return physicalFrameNumberWillBeDeleted;
}

void MemoryManager::printSecondChanceQueue(){
    queue<int> temp = secondChanceQueue;
    cout << "Second Chance Queue: ";
    while(!temp.empty()){
        cout << temp.front() << " ";
        temp.pop();
    }
    cout << endl;
}

int MemoryManager::getValueFromPhysicalMemory(int virtualFrameNumber, int offset){
    int physicalFrameNumber = pageTable->getPhysicalFrameNumber(virtualFrameNumber);
    if(physicalFrameNumber == -1){ // page fault is occured
        numberOfPageFaults++;
        numberOfPageReplacements++;
        switch (pageReplacementAlgorithm)
        {
        case WS_CLOCK_PAGE_REPLACEMENT:
            wsClockPageReplacement(virtualFrameNumber);
            break;
        case SECOND_CHANCE_PAGE_REPLACEMENT:
            secondChancePageReplacement(virtualFrameNumber);
            break;
        case LEAST_RECENTLY_USED_PAGE_REPLACEMENT:
            leastRecentlyUsedPageReplacement(virtualFrameNumber);
            break;
        default:
            break;
        }
        physicalFrameNumber = pageTable->getPhysicalFrameNumber(virtualFrameNumber);
    }
    vector<int> frame = physicalMemory->getFrame(physicalFrameNumber);
    pageTable->setReferenced(virtualFrameNumber, true);
    if(pageReplacementAlgorithm == WS_CLOCK_PAGE_REPLACEMENT){
        wsClock->updateLastAccessTime(physicalFrameNumber);  
    }

    if(pageReplacementAlgorithm == LEAST_RECENTLY_USED_PAGE_REPLACEMENT)
        leastRecentlyUsed->incrementLRUEntryCounter(physicalFrameNumber);
    numberOfReads++;

    if(numberOfReads % pageTablePrintLimit == 0){
        cout << "Page Table: " << endl;
        pageTable->printPageTable();
        cout << endl;
    }

    return frame[offset];
}

void MemoryManager::leastRecentlyUsedPageReplacement(int virtualFrameNumber){
    int virtualFrameNumberWillBeReplaced = leastRecentlyUsed->getLeastRecentlyUsedVirtualFrameNumber();
    leastRecentlyUsed->resetLRUEntryCounter(leastRecentlyUsed->getPhysicalFrameNumber(virtualFrameNumberWillBeReplaced));
    leastRecentlyUsed->setLRUEntryVirtualFrameNumber(leastRecentlyUsed->getPhysicalFrameNumber(virtualFrameNumberWillBeReplaced), virtualFrameNumber);
    int physicalFrameNumberWillBeReplaced = deleteFrameFromPhysicalMemory(virtualFrameNumberWillBeReplaced);
    vector<int> frameFromFile = getFrameFromDiskFile(virtualFrameNumber);
    physicalMemory->setFrame(physicalFrameNumberWillBeReplaced, frameFromFile);
    pageTable->initializePageTableEntry(virtualFrameNumber, physicalFrameNumberWillBeReplaced);
}

void MemoryManager::wsClockPageReplacement(int virtualFrameNumber){
    for (int i = 0; i < physicalFrameCount; i++) {
        int startingIndex = (i + wsClockIndex) % physicalFrameCount;
        int virtualFrameIndex = pageTable->getVirtualFrameNumber(startingIndex);
        if (!pageTable->isReferenced(virtualFrameIndex)) { // R bit 0 ise ne yapılcak
            time_t currentTime = time(0);
            if (difftime(wsClock->getWSClockEntryLastAccessTime(startingIndex), currentTime) > threshold) {
                int virtualFrameNumberWillBeReplaced = wsClock->getWSClockEntryVirtualFrameNumber(startingIndex);
                int physicalFrameNumberWillBeReplaced = deleteFrameFromPhysicalMemory(virtualFrameNumberWillBeReplaced);
                vector<int> frameFromFile = getFrameFromDiskFile(virtualFrameNumber);
                physicalMemory->setFrame(physicalFrameNumberWillBeReplaced, frameFromFile);
                pageTable->initializePageTableEntry(virtualFrameNumber, physicalFrameNumberWillBeReplaced);
                wsClockIndex = startingIndex+1;
                return;
            } 
            else{
                continue;
            }
        }
        else{
            pageTable->setReferenced(virtualFrameIndex, false);
            wsClock->updateLastAccessTime(virtualFrameIndex);
        }
    }

    // if all bits are 1, then we have to do some replacement operations
    int virtualFrameNumberWillBeReplaced = wsClock->getWSClockEntryVirtualFrameNumber(wsClockIndex);
    wsClock->setWSClockEntry(wsClockIndex, virtualFrameNumber);
    int physicalFrameNumberWillBeReplaced = deleteFrameFromPhysicalMemory(virtualFrameNumberWillBeReplaced);
    vector<int> frameFromFile = getFrameFromDiskFile(virtualFrameNumber);
    physicalMemory->setFrame(physicalFrameNumberWillBeReplaced, frameFromFile);
    pageTable->initializePageTableEntry(virtualFrameNumber, physicalFrameNumberWillBeReplaced);
    wsClockIndex = (wsClockIndex + 1) % physicalFrameCount;
}

void MemoryManager::writeValueToPhysicalMemory(int virtualFrameNumber, int offset, int value){
    int physicalFrameNumber = pageTable->getPhysicalFrameNumber(virtualFrameNumber);
    if(physicalFrameNumber == -1){ // page fault is occured
        numberOfPageFaults++;
        numberOfPageReplacements++;
        switch (pageReplacementAlgorithm)
        {
        case WS_CLOCK_PAGE_REPLACEMENT:
            wsClockPageReplacement(virtualFrameNumber);
            break;
        case SECOND_CHANCE_PAGE_REPLACEMENT:
            secondChancePageReplacement(virtualFrameNumber);
            break;
        case LEAST_RECENTLY_USED_PAGE_REPLACEMENT:
            leastRecentlyUsedPageReplacement(virtualFrameNumber);
            break;
        default:
            break;
        }
        physicalFrameNumber = pageTable->getPhysicalFrameNumber(virtualFrameNumber);
    }

    pageTable->setReferenced(virtualFrameNumber, false);
    physicalMemory->setValue(physicalFrameNumber, offset, value);
    pageTable->setModified(virtualFrameNumber, true);
    if(pageReplacementAlgorithm == WS_CLOCK_PAGE_REPLACEMENT)
        wsClock->updateLastAccessTime(physicalFrameNumber);  
    numberOfWrites++;
}

void MemoryManager::printPhysicalMemory(){
    physicalMemory->printPhysicalMemory();
}

void MemoryManager::printPageTable(){
    pageTable->printPageTable();
}

void MemoryManager::printStatistics(){
    cout << "Number of page faults: " << this->numberOfPageFaults << endl;
    cout << "Number of reads: " << this->numberOfReads << endl;
    cout << "Number of writes: " << this->numberOfWrites << endl;
    cout << "Number of disk page writes: " << this->numberOfDiskPageWrites << endl;
    cout << "Number of disk page reads: " << this->numberOfDiskPageReads << endl;
    cout << "Number of page replacements: " << this->numberOfPageReplacements << endl;
}