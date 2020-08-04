#include "cpu.h"
#include <iostream>

int main (void){

    cpu nes;
    
    nes.initPC(0x8000);

    //std::istringstream sampleProgram("a2 08 ca 8e 00 02 e0 03 d0 f8 8e 01 02 00 "); 
    //std::string program = "a9 01 85 f0 a9 cc 85 f1 6c f0 00"; //indirect JMP
    //std::string program = "a2 01 a9 05 85 01 a9 07 85 02 a0 0a 8c 05 07 a1 00"; //INDX
    //std::string program = "a0 01 a9 03 85 01 a9 07 85 02 a2 0a 8e 04 07 b1 01"; //INDY
    //std::string program = "a9 0a aa 9d 10 00 a2 00 a8 be 10 00"; //ABSX
    std::string program = "a2 01 a0 aa 84 a0 c8 a6 a0 e8 96 a0";
    std::istringstream sampleProgram(program);
    
    nes.loadProgram(sampleProgram);
    sampleProgram.seekg(0);

    /*
        From: https://skilldrick.github.io/easy6502/ (this emulator stops when a BRK is encountered)

                LDX #$08
            decrement:
                DEX
                STX $0200
                CPX #$03
                BNE decrement
                STX $0201
                BRK
    */

    nes.printCPU();
    std::cout << "---------------------------------------------------\n";
    while(!nes.emulationDone())
    {
        do{
            nes.step();
        }while(!nes.instrComplete());
        nes.printCPU(); 
    }

    

    return 0;
}