#include <iostream>
#include "cpu.h"

cpu::cpu()
{
    
}

cpu::~cpu()
{
    //nothing to do
}

void cpu::reset()
{
    status = 0x0; 
    stackPtr = 0xff; //base of stack in memory is 0x100, stackptr represents the offset and grows downwards
    A = 0x0;
    X = 0x0;
    Y = 0x0;

    //pc needs to jump to address at 0xfffc and 0xfffd (LITTLE ENDIAN)

}

void cpu::print()
{ 
    std::cout << "6502 cpu emulation!\n";
}

void cpu::setStatus(Flag flag, bool value)
{
    if (value)
        status |= flag;
    else
        status &= ~flag;
    
}

uint8_t cpu::getStatus(Flag flag){
    return (status & flag) > 0 ? 1 : 0;
}



/* DEBUGGING FUNCTIONS */
uint8_t cpu::getStatusReg() 
{
    return status;
}
uint8_t cpu::getA()
{
    return A;
}
uint8_t cpu::getX(){
    return X;
}
uint8_t cpu::getY()
{
    return Y;
}
uint8_t cpu::getPC()
{
    return pc;
}
uint8_t cpu::getStackPtr()
{
    return stackPtr;
}