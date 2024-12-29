#include "port.h"

Port::Port(uint16_t portnumber){
    this->portnumber = portnumber;
}

Port::~Port(){

}


//Port 8 Bit
Port8Bit::Port8Bit(uint16_t portnumber) : Port(portnumber){

}

Port8Bit::~Port8Bit(){

}

void Port8Bit::Write(uint8_t data){
    // writes a byte (b) to an I/O port.
    /*
    The "a" constraint specifies that the data value should be placed in the AL register (part of the EAX register) because outb expects its first operand in the AL register.
    The "Nd" constraint specifies that the portnumber should be an immediate value (constant) or fit in the DX register.
    */
    __asm__ volatile("outb %0, %1" :: "a"(data), "Nd"(portnumber));
}

uint8_t Port8Bit::Read(){
    /*
    "=a" specifies that the result will be stored in the AL register (lower 8 bits of the EAX register).
    The = indicates that this is an output operand.
    "Nd" specifies that the port number can either be an immediate value (constant) or fit in the DX register.
    The inb instruction expects the port number in the DX register if it's not a constant.
    */
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}

//Port 8 Bit Slow
//Purpose
/*
Some hardware devices require a small delay after receiving data to process it correctly. Writing data too quickly can lead to undefined behavior or missed operations.
Compatibility:
This is especially useful when dealing with older hardware or systems where precise timing is critical.
*/
Port8BitSlow::Port8BitSlow(uint16_t portnumber) : Port8Bit(portnumber){

}

Port8BitSlow::~Port8BitSlow(){

}

void Port8BitSlow::Write(uint8_t data){
    /*
    jmp 1f and 1: Labels:
    The jmp instruction creates a delay by introducing a couple of "no-operation" jumps.
    1f refers to the label 1: (a local label) that appears after the jump.
    */
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" :: "a"(data), "Nd"(portnumber));
}


//Port 16 Bit
Port16Bit::Port16Bit(uint16_t portnumber) : Port(portnumber){

}

Port16Bit::~Port16Bit(){

}

void Port16Bit::Write(uint16_t data){
    __asm__ volatile("outw %0, %1" :: "a"(data), "Nd"(portnumber));
}

uint16_t Port16Bit::Read(){
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}


//Port 32 Bit
Port32Bit::Port32Bit(uint16_t portnumber) : Port(portnumber){

}

Port32Bit::~Port32Bit(){

}

void Port32Bit::Write(uint32_t data){
    __asm__ volatile("outl %0, %1" :: "a"(data), "Nd"(portnumber));
}

uint32_t Port32Bit::Read(){
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(portnumber));
    return result;
}