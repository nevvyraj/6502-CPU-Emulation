#include "cpu.h"
#include <iostream>

int main (void){

    cpu nes;

    

    nes.initPC(0x8000);

    std::istringstream sampleProgram("a9 7f aa e8 69 7f 00"); //overflow test
    //std::istringstream sampleProgram("a9 c0 aa e8 69 c4 00");
    nes.loadProgram(sampleProgram);

    /*
        From: https://skilldrick.github.io/easy6502/ (this emulator stops when a BRK is encountered)

        LDA #$c0  ;Load the hex value $c0 into the A register
        TAX       ;Transfer the value in the A register to X
        INX       ;Increment the value in the X register
        ADC #$c4  ;Add the hex value $c4 to the A register
        BRK       ;Break - we're done

        Output:
            A=$84 X=$c1 Y=$00
            SP=$ff PC=$0607 (pc started at $0600)

            NV-BDIZC
            10110001
    */

    nes.printCPU();
    while(!nes.emulationDone())
    {
        do{
            nes.step();
        }while(!nes.instrComplete());
        nes.printCPU(); 
    }

    

    return 0;
}