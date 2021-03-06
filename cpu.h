#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

#define RAMSIZE 64*1024

class cpu
{

    //Emulator Variables
    private:
        uint8_t A;
        uint8_t X;
        uint8_t Y;

        uint8_t stackPtr;

        uint8_t status; //processor status flags
        enum Flag 
        {
            C = 1 << 0,
            Z = 1 << 1,
            I = 1 << 2,
            D = 1 << 3,
            B = 1 << 4,
            U = 1 << 5,
            V = 1 << 6,
            N = 1 << 7
        };

        uint16_t pc;

        uint8_t ram[RAMSIZE];

        uint8_t instrCycles; //number of cycles current instruction needs to execute
        uint64_t systemCycles; //total clock cycles

        uint16_t absoluteAddr;
        uint8_t fetchedData; //data fetched from addressing mode absolute address
        uint8_t currentOpcode; //opcode of currently executing instruction

        typedef struct instruction{
            instruction(std::string n, void(cpu::*operation)(void), bool(cpu::*addrmode)(void),  uint8_t c,
            bool affectedByPageBoundaryCrossing)
                :name(std::move(n)), cycles(c), addrmode(addrmode), operation(operation), 
                affectedByPageBoundaryCrossing(affectedByPageBoundaryCrossing)
            {}

            std::string name; 
            void (cpu::*operation)(void); //actual operation for the instruction
            bool (cpu::*addrmode)(void); //addressing mode function
            uint8_t cycles; 
            bool affectedByPageBoundaryCrossing;
        } instruction;

        std::vector<instruction> opcodeLookup;


    //Emulator Functions
    public:
        cpu();
        ~cpu();
        
        void reset();
        void step(); //step through one instruction 
        bool instrComplete();

        void loadProgram(std::istringstream& prog, uint16_t addr);

        bool emulationDone(); //temporary: end emulation if BRK instruction encountered


    private:
        bool endEmulation;

    private:
        void setStatus(Flag flag, bool value);
        uint8_t getStatus(Flag flag);

        uint8_t memRead(uint16_t addr);
        void memWrite(uint16_t addr, uint8_t data);

        uint8_t fetchData( bool(cpu::*addrmode)() );

        void pushStack(uint8_t data);
        uint8_t popStack(); //return top of stack

    
    //instruction related functions
    private:
        /* ADDRESSING MODES - return whether the addressing mode crosses a page boundary*/
        bool ABS(); //absolute
        bool ABSX(); //absolute x-indexed
        bool ABSY(); //absolute y-indexed
        bool IMM(); //immediate
        bool IMPL(); //implied and accumulator
        bool IND(); //indirect
        bool INDX(); //indirect x-indexed
        bool INDY(); //indirect y-indexed
        bool REL(); //relative
        bool ZPG(); //zero page
        bool ZPGX(); //zero page x-indexed
        bool ZPGY(); //zero page y-indexed

        /* INSTRCTIONS (as outlined in https://www.masswerk.at/6502/6502_instruction_set.html)*/
        void ADC();         void DEC();         void ROR();
        void AND();         void DEX();         void RTI();
        void ASL();         void DEY();         void RTS();
        void BCC();         void EOR();         void SBC();
        void BCS();         void INC();         void SEC();
        void BEQ();         void INX();         void SED();
        void BIT();         void INY();         void SEI();
        void BMI();         void JMP();         void STA();
        void BNE();         void JSR();         void STX();
        void BPL();         void LDA();         void STY();
        void BRK();         void LDX();         void TAX();
        void BVC();         void LDY();         void TAY();
        void BVS();         void LSR();         void TSX();
        void CLC();         void NOP();         void TXA();
        void CLD();         void ORA();         void TXS();
        void CLI();         void PHA();         void TYA();
        void CLV();         void PHP();
        void CMP();         void PLA();         
        void CPX();         void PLP();
        void CPY();         void ROL();
         

    //Debugging Functions
    public:
        void printCPU();
        uint8_t getStatusReg();
        uint8_t getA();
        uint8_t getX();
        uint8_t getY();
        uint16_t getPC();
        uint8_t getStackPtr();
        void initPC(uint16_t addr) { pc= addr;}
        uint8_t getMemVal(uint16_t addr) {return memRead(addr);}

        std::unordered_map<uint16_t, std::string> disassemble();

};