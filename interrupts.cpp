#include "interrupts.h"


void printf(char* str);

uint32_t InterruptManger::handleIntrupt(uint8_t interruptNumber, uint32_t esp){

    printf("Interrupts....");
    return esp;
}