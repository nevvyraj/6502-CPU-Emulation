#include "cpu.h"
#include <iostream>

int main (void){

    cpu nes;
    
    //std::string program = "a2 08 ca 8e 00 02 e0 03 d0 f8 8e 01 02 00 "; 
    //std::string program = "a9 01 85 f0 a9 cc 85 f1 6c f0 00"; //indirect JMP
    //std::string program = "a2 01 a9 05 85 01 a9 07 85 02 a0 0a 8c 05 07 a1 00"; //INDX
    //std::string program = "a0 01 a9 03 85 01 a9 07 85 02 a2 0a 8e 04 07 b1 01"; //INDY
    //std::string program = "a9 0a aa 9d 10 00 a2 00 a8 be 10 00"; //ABSX
    //std::string program = "a2 01 a0 aa 84 a0 c8 a6 a0 e8 96 a0";

    //std::string program = "a2 01 a9 05 85 01 a9 07 85 02 a0 0a 8c 05 07 a1 00";
    
    //from https://github.com/cminter/hmc-6502/blob/master/emu/testvectors/TestAllInstructions/test00-loadstore.6502
    //expected: $022A = 0x55
    //std::string program = "a9 55 a2 2a a0 73 85 81 a9 01 85 61 a9 7e a5 81 8d 10 09 a9 7e ad 10 09 95 56 a9 7e b5 56 84 60 91 60 a9 7e b1 60 9d ff 07 a9 7e bd ff 07 99 ff 07 a9 7e b9 ff 07 81 36 a9 7e a1 36 86 50 a6 60 a4 50 8e 13 09 a2 22 ae 13 09 8c 14 09 a0 99 ac 14 09 94 2d 96 77 a0 99 b4 2d a2 22 b6 77 a0 99 bc a0 08 a2 22 be a1 08 9d 00 02";

    //test1 and,or,xor
    //EXPECTED RESULTS: $A9 = 0xAA
    //std::string program = "a9 55 29 53 09 38 49 11 85 99 a9 b9 85 10 a9 e7 85 11 a9 39 85 12 a5 99 25 10 05 11 45 12 a2 10 85 99 a9 bc 85 20 a9 31 85 21 a9 17 85 22 a5 99 35 10 15 11 55 12 85 99 a9 6f 8d 10 01 a9 3c 8d 11 01 a9 27 8d 12 01 a5 99 2d 10 01 0d 11 01 4d 12 01 85 99 a9 8a 8d 20 01 a9 47 8d 21 01 a9 8f 8d 22 01 a5 99 3d 10 01 1d 11 01 5d 12 01 a0 20 85 99 a9 73 8d 30 01 a9 2a 8d 31 01 a9 f1 8d 32 01 a5 99 39 10 01 19 11 01 59 12 01 85 99 a9 70 85 30 a9 01 85 31 a9 71 85 32 a9 01 85 33 a9 72 85 34 a9 01 85 35 a9 c5 8d 70 01 a9 7c 8d 71 01 a9 a1 8d 72 01 a5 99 21 20 01 22 41 24 85 99 a9 60 85 40 a9 01 85 41 a9 61 85 42 a9 01 85 43 a9 62 85 44 a9 01 85 45 a9 37 8d 50 02 a9 23 8d 51 02 a9 9d 8d 52 02 a5 99 a0 f0 31 40 11 42 51 44 85 a9";

    //test2 inc,dec
    //EXPECTED RESULTS: $71=0xFF
    //std::string program = "a9 ff a2 00 85 90 e6 90 e6 90 a5 90 a6 90 95 90 f6 90 b5 90 a6 91 9d 90 01 ee 92 01 bd 90 01 ae 92 01 9d 90 01 fe 90 01 bd 90 01 ae 93 01 9d 70 01 de 70 01 bd 70 01 ae 74 01 9d 70 01 ce 73 01 bd 70 01 ae 73 01 95 70 d6 70 b5 70 a6 72 95 70 c6 71 c6 71";

    //test3 bitshift
    //EXPECTED RESULTS: $01DD = 0x6E
    //std::string program = "a9 4b 4a 0a 85 50 06 50 06 50 46 50 a5 50 a6 50 09 c9 85 60 16 4c 56 4c 56 4c b5 4c a6 60 09 41 8d 2e 01 5e 00 01 5e 00 01 1e 00 01 bd 00 01 ae 2e 01 09 81 9d 00 01 4e 36 01 4e 36 01 0e 36 01 bd 00 01 2a 2a 6a 85 70 a6 70 09 03 95 0c 26 c0 66 c0 66 c0 b5 0c a6 c0 85 d0 36 75 36 75 76 75 a5 d0 a6 d0 9d 00 01 2e b7 01 2e b7 01 2e b7 01 6e b7 01 bd 00 01 ae b7 01 8d dd 01 3e 00 01 7e 00 01 7e 00 01";

    //test4 jmp - final address at $0624 -- need to load program at $0600
    //nes.initPC(0x0600);
    //EXPECTED RESULTS: $40=0x42
    //std::string program = "a9 24 85 20 a9 06 85 21 a9 00 09 03 4c 11 06 09 ff 09 30 20 1d 06 09 42 6c 20 00 09 ff 85 30 a6 30 a9 00 60 95 0d ";

    //test5 register function tax,txa,....
    //EXPECTED RESULTS: $40 = 0x33
    //std::string program = "a9 35 aa ca ca e8 8a a8 88 88 c8 98 aa a9 20 9a a2 10 ba 8a 85 40";

    //test6 ADC,SBC
    //EXPECTED RESULTS: $30=0x9D
    //std::string program = "a9 6a 85 50 a9 6b 85 51 a9 a1 85 60 a9 a2 85 61 a9 ff 69 ff 69 ff e9 ae 85 40 a6 40 75 00 f5 01 65 60 e5 61 8d 20 01 a9 4d 8d 21 01 a9 23 6d 20 01 ed 21 01 85 f0 a6 f0 a9 64 8d 24 01 a9 62 8d 25 01 a9 26 7d 00 01 fd 01 01 85 f1 a4 f1 a9 e5 8d 28 01 a9 e9 8d 29 01 a9 34 79 00 01 f9 01 01 85 f2 a6 f2 a9 20 85 70 a9 01 85 71 a9 24 85 72 a9 01 85 73 61 41 e1 3f 85 f3 a4 f3 a9 da 85 80 a9 00 85 81 a9 dc 85 82 a9 00 85 83 a9 aa 71 80 f1 82 85 30 ";

    //test7 cmp,beq,bne
    //EXPECTED RESULTS: $15 = 0x7F
    //pc starts at 0600
    //std::string program = "a9 00 85 34 a9 ff 8d 30 01 a9 99 8d 9d 01 a9 db 8d 99 01 a9 2f 85 32 a9 32 85 4f a9 30 85 33 a9 70 85 af a9 18 85 30 c9 18 f0 02 29 00 09 01 c5 30 d0 02 29 00 a2 00 cd 30 01 f0 04 85 40 a6 40 d5 27 d0 06 09 84 85 41 a6 41 29 db dd 00 01 f0 02 29 00 85 42 a4 42 29 00 d9 00 01 d0 02 09 0f 85 43 a6 43 09 24 c1 40 f0 02 09 7f 85 44 a4 44 49 0f d1 33 d0 04 a5 44 85 15";

    //test8 cpx,cpy,bit
    //EXPECTED RESULTS $42 = 0xA5
    //pc starts at 0600
    //std::string program = "a9 a5 85 20 8d 20 01 a9 5a 85 21 a2 a5 e0 a5 f0 02 a2 01 e4 20 f0 02 a2 02 ec 20 01 f0 02 a2 03 86 30 a4 30 c0 a5 f0 02 a0 04 c4 20 f0 02 a0 05 cc 20 01 f0 02 a0 06 84 31 a5 31 24 20 d0 02 a9 07 2c 20 01 d0 02 a9 08 24 21 d0 02 85 42";

    //test9 all other branches
    //EXPECTED RESULTS: $80 = 0x1F
    //pc starts at 0600
    //std::string program = "a9 54 85 32 a9 b3 85 a1 a9 87 85 43 a2 a1 10 02 a2 32 b4 00 10 04 a9 05 a6 a1 30 02 e9 03 30 02 a9 41 49 30 85 32 75 00 50 02 a9 03 85 54 b6 00 75 51 50 02 a9 e5 75 40 70 05 99 01 00 65 55 70 02 a9 00 69 f0 90 04 85 60 65 43 90 02 a9 ff 65 54 b0 04 69 87 a6 60 b0 02 a9 00 95 73";

    //test10 flag instructions
    //EXPECTED RESULTS: $30 = 0xCE
    //pc starts at 0600
    std::string program = "a9 99 69 87 18 ea 90 04 69 60 69 93 38 ea 90 01 b8 50 02 a9 00 69 ad ea 85 30";

    std::istringstream sampleProgram(program);
    
    uint16_t pcStart = 0x0600;
    nes.initPC(pcStart);
    nes.loadProgram(sampleProgram, pcStart);

    uint16_t resultLocation = 0x0030;
    uint8_t result = 0xCE;

    std::unordered_map<uint16_t,std::string> code;
    code = nes.disassemble();

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

    
    std::cout << "---------------------------------------------------\n";
    while(!nes.emulationDone())
    {
        std::cout << std::hex << nes.getPC() << ": " << code[nes.getPC()] << "\t";
        do{
            nes.step();
        }while(!nes.instrComplete());
        nes.printCPU();
    
    }


    std::cout << std::hex << "ram[$" << resultLocation << "]: 0x" << (uint16_t)nes.getMemVal(resultLocation) << "\n";
    if (result == nes.getMemVal(resultLocation))
    {
        std::cout << "\033[1;32mTEST PASSED!\033[0m\n";
    } else
    {
        std::cout << "\033[1;31mTEST FAILED\033[0m\n";
    }
    
    return 0;
}