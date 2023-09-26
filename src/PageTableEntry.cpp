#include "../include/PageTableEntry.h"

PageTableEntry::PageTableEntry()
{
    this->pageFrameNumber = -1;
    this->present = false;
    this->modified = false;
    this->referenced = false;
}

PageTableEntry::PageTableEntry(int pageFrameNumber){
    this->pageFrameNumber = pageFrameNumber;
    this->present = false;
    this->modified = false;
    this->referenced = false;
}

int PageTableEntry::getPageFrameNumber(){
    return pageFrameNumber;
}

void PageTableEntry::setPageFrameNumber(int pageFrameNumber){
    this->pageFrameNumber = pageFrameNumber;
}
bool PageTableEntry::isPresent(){
    return present;
}
void PageTableEntry::setPresentBit(bool present){
    this->present = present;
}
bool PageTableEntry::isModified(){
    return modified;
}
void PageTableEntry::setModified(bool modified){
    this->modified = modified;
}
bool PageTableEntry::isReferenced(){
    return referenced;
}
void PageTableEntry::setReferenced(bool referenced){
    this->referenced = referenced;
}
string PageTableEntry::toString(){
    std::string result = "Frame Number: " + std::to_string(pageFrameNumber);
    result += "\nPresent: " + std::string(present ? "true" : "false");
    result += "\nModified: " + std::string(modified ? "true" : "false");
    result += "\nReferenced: " + std::string(referenced ? "true" : "false");
    result += "\n";
    return result;
}