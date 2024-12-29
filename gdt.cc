#include "gdt.h"

struct GDTPointer {
    public:
        uint32_t limit;   
        uint32_t base;    // Address of the GDT
} __attribute__((packed)); 

GlobalDescriptorTable::GlobalDescriptorTable(): 
nullSegmentSelector(0,0,0), //indicate that it is an empty or unused segment (no access rights, no base, no limit).
unusedSegmentSelector(0,0,0), 
codeSegmentSelector(0, 64*1024*1024, 0x9A), //(64 MB) is the segment limit, 0x9A code segment flag
dataSegementSelector(0, 64*1024*1024, 0x92) //(64 MB) is the segment limit, 0x92 data segment flag
{

    GDTPointer gdtp;
    gdtp.limit = sizeof(GlobalDescriptorTable) << 16;
    gdtp.base = reinterpret_cast<uint32_t>(this);  

    // Load the GDT
    asm volatile("lgdt %0" : : "m"(gdtp));

}

GlobalDescriptorTable::~GlobalDescriptorTable()
{}

uint16_t GlobalDescriptorTable::DataSegmentSelector(){
    return (uint8_t*)&dataSegementSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector(){
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags){

    uint8_t* target = (uint8_t*)this;

    // If the limit is less than 16bits, this tells the processor that this is 16bit entry
    if(limit <= 65536){
        /* granularity flag
         * The granularity flag in the segment descriptor needs to be set to 
         * indicate that the limit is in byte units.
        */
        target[6] = 0x40;
    }
    else{

        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) -1; //adjusted by shifting it to represent a value in pages
        else
            limit = limit >> 12; //shifted by 12 bits to represent the limit in terms of pages
        
        target[6] = 0xC0; // to indicate that the limit is in pages
    }

    //Setting the limit
    target[0] = limit & 0xFF; //first 8 bits
    target[1] = (limit >> 8) & 0xFF; // next 8 bits 
    target[6] |= (limit >> 16) & 0xF; // lower 4 bits

    //Setting the pointer(base)
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    //Setting Flags
    target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base(){

    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit(){
    
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xFFF;
    
    return result;
}