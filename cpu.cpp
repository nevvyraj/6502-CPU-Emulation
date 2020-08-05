#include <iostream>
#include "cpu.h"

#define STACK_OFFSET 0x0100

cpu::cpu() 
{
    for (int i = 0; i < RAMSIZE; i++)
    {
        ram[i] = 0x00;
    }

    status = 0x34; 
    stackPtr = 0xfd; //base of stack in memory is 0x100, stackptr represents the offset and grows downwards
    A = 0x0;
    X = 0x0;
    Y = 0x0;

    instrCycles = 0;
    systemCycles = 0;
    fetchedData = 0x00;
    absoluteAddr = 0x000;


    opcodeLookup = {
            {"BRK", &cpu::BRK, &cpu::IMPL, 7, false},  {"ORA", &cpu::ORA, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ORA", &cpu::ORA, &cpu::ZPG, 3, false},  {"ASL", &cpu::ASL, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"PHP", &cpu::PHP, &cpu::IMPL, 3, false}, {"ORA", &cpu::ORA, &cpu::IMM, 2, false},  {"ASL", &cpu::ASL, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ORA", &cpu::ORA, &cpu::ABS, 4, false}, {"ASL", &cpu::ASL, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BPL", &cpu::BPL, &cpu::REL, 2, false},   {"ORA", &cpu::ORA, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ORA", &cpu::ORA, &cpu::ZPGX, 4, false}, {"ASL", &cpu::ASL, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CLC", &cpu::CLC, &cpu::IMPL, 2, false}, {"ORA", &cpu::ORA, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ORA", &cpu::ORA, &cpu::ABSX, 4, true}, {"ASL", &cpu::ASL, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"JSR", &cpu::JSR, &cpu::ABS, 6, false},   {"AND", &cpu::AND, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"BIT", &cpu::BIT, &cpu::ZPG, 3, false},  {"AND", &cpu::AND, &cpu::ZPG, 3, false},  {"ROL", &cpu::ROL, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"PLP", &cpu::PLP, &cpu::IMPL, 4, false}, {"AND", &cpu::AND, &cpu::IMM, 2, false},  {"ROL", &cpu::ROL, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"BIT", &cpu::BIT, &cpu::ABS, 4, false},  {"AND", &cpu::AND, &cpu::ABS, 4, false}, {"ROL", &cpu::ROL, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BMI", &cpu::BMI, &cpu::REL, 2, false},   {"AND", &cpu::AND, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"AND", &cpu::AND, &cpu::ZPGX, 4, false}, {"ROL", &cpu::ROL, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"SEC", &cpu::SEC, &cpu::IMPL, 2, false}, {"AND", &cpu::AND, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"AND", &cpu::AND, &cpu::ABSX, 4, true}, {"ROL", &cpu::ROL, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"RTI", &cpu::RTI, &cpu::IMPL, 6, false},  {"EOR", &cpu::EOR, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"EOR", &cpu::EOR, &cpu::ZPG, 3, false},  {"LSR", &cpu::LSR, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"PHA", &cpu::PHA, &cpu::IMPL, 3, false}, {"EOR", &cpu::EOR, &cpu::IMM, 2, false},  {"LSR", &cpu::LSR, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"JMP", &cpu::JMP, &cpu::ABS, 3, false},  {"EOR", &cpu::EOR, &cpu::ABS, 4, false}, {"LSR", &cpu::LSR, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BVC", &cpu::BVC, &cpu::REL, 2, false},   {"EOR", &cpu::EOR, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"EOR", &cpu::EOR, &cpu::ZPGX, 4, false}, {"LSR", &cpu::LSR, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CLI", &cpu::CLI, &cpu::IMPL, 2, false}, {"EOR", &cpu::EOR, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"EOR", &cpu::EOR, &cpu::ABSX, 4, true}, {"LSR", &cpu::LSR, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"RTS", &cpu::RTS, &cpu::IMPL, 6, false},  {"ADC", &cpu::ADC, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ADC", &cpu::ADC, &cpu::ZPG, 3, false},  {"ROR", &cpu::ROR, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"PLA", &cpu::PLA, &cpu::IMPL, 4, false}, {"ADC", &cpu::ADC, &cpu::IMM, 2, false},  {"ROR", &cpu::ROR, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"JMP", &cpu::JMP, &cpu::IND, 5, false},  {"ADC", &cpu::ADC, &cpu::ABS, 4, false}, {"ROR", &cpu::ROR, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BVS", &cpu::BVS, &cpu::REL, 2, false},   {"ADC", &cpu::ADC, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ADC", &cpu::ADC, &cpu::ZPGX, 4, false}, {"ROR", &cpu::ROR, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"SEI", &cpu::SEI, &cpu::IMPL, 2, false}, {"ADC", &cpu::ADC, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"ADC", &cpu::ADC, &cpu::ABSX, 4, true}, {"ROR", &cpu::ROR, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"STA", &cpu::STA, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"STY", &cpu::STY, &cpu::ZPG, 3, false},  {"STA", &cpu::STA, &cpu::ZPG, 3, false},  {"STX", &cpu::STX, &cpu::ZPG, 3, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"DEY", &cpu::DEY, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"TXA", &cpu::TXA, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"STY", &cpu::STY, &cpu::ABS, 4, false},  {"STA", &cpu::STA, &cpu::ABS, 4, false}, {"STX", &cpu::STX, &cpu::ABS, 4, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BCC", &cpu::BCC, &cpu::REL, 2, false},   {"STA", &cpu::STA, &cpu::INDY, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"STY", &cpu::STY, &cpu::ZPGX, 4, false}, {"STA", &cpu::STA, &cpu::ZPGX, 4, false}, {"STX", &cpu::STX, &cpu::ZPGY, 4, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"TYA", &cpu::TYA, &cpu::IMPL, 2, false}, {"STA", &cpu::STA, &cpu::ABSY, 5, false}, {"TXS", &cpu::TXS, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"STA", &cpu::STA, &cpu::ABSX, 5, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"LDY", &cpu::LDY, &cpu::IMM, 2, false},   {"LDA", &cpu::LDA, &cpu::INDX, 6, false},  {"LDX", &cpu::LDX, &cpu::IMM, 2, false},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"LDY", &cpu::LDY, &cpu::ZPG, 3, false},  {"LDA", &cpu::LDA, &cpu::ZPG, 3, false},  {"LDX", &cpu::LDX, &cpu::ZPG, 3, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"TAY", &cpu::TAY, &cpu::IMPL, 2, false}, {"LDA", &cpu::LDA, &cpu::IMM, 2, false},  {"TAX", &cpu::TAX, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"LDY", &cpu::LDY, &cpu::ABS, 4, false},  {"LDA", &cpu::LDA, &cpu::ABS, 4, false}, {"LDX", &cpu::LDX, &cpu::ABS, 4, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BCS", &cpu::BCS, &cpu::REL, 2, false},   {"LDA", &cpu::LDA, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"LDY", &cpu::LDY, &cpu::ZPGX, 4, false}, {"LDA", &cpu::LDA, &cpu::ZPGX, 4, false}, {"LDX", &cpu::LDX, &cpu::ZPGY, 4, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CLV", &cpu::CLV, &cpu::IMPL, 2, false}, {"LDA", &cpu::LDA, &cpu::ABSY, 4, true},  {"TSX", &cpu::TSX, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"LDY", &cpu::LDY, &cpu::ABSX, 4, true},  {"LDA", &cpu::LDA, &cpu::ABSX, 4, true}, {"LDX", &cpu::LDX, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"CPY", &cpu::CPY, &cpu::IMM, 2, false},   {"CMP", &cpu::CMP, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CPY", &cpu::CPY, &cpu::ZPG, 3, false},  {"CMP", &cpu::CMP, &cpu::ZPG, 3, false},  {"DEC", &cpu::DEC, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"INY", &cpu::INY, &cpu::IMPL, 2, false}, {"CMP", &cpu::CMP, &cpu::IMM, 2, false},  {"DEX", &cpu::DEX, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CPY", &cpu::CPY, &cpu::ABS, 4, false},  {"CMP", &cpu::CMP, &cpu::ABS, 4, false}, {"DEC", &cpu::DEC, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BNE", &cpu::BNE, &cpu::REL, 2, false},   {"CMP", &cpu::CMP, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CMP", &cpu::CMP, &cpu::ZPGX, 4, false}, {"DEC", &cpu::DEC, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CLD", &cpu::CLD, &cpu::IMPL, 2, false}, {"CMP", &cpu::CMP, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CMP", &cpu::CMP, &cpu::ABSX, 4, true}, {"DEC", &cpu::DEC, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"CPX", &cpu::CPX, &cpu::IMM, 2, false},   {"SBC", &cpu::SBC, &cpu::INDX, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CPX", &cpu::CPX, &cpu::ZPG, 3, false},  {"SBC", &cpu::SBC, &cpu::ZPG, 3, false},  {"INC", &cpu::INC, &cpu::ZPG, 5, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"INX", &cpu::INX, &cpu::IMPL, 2, false}, {"SBC", &cpu::SBC, &cpu::IMM, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"CPX", &cpu::CPX, &cpu::ABS, 4, false},  {"SBC", &cpu::SBC, &cpu::ABS, 4, false}, {"INC", &cpu::INC, &cpu::ABS, 6, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, 
            {"BEQ", &cpu::BEQ, &cpu::REL, 2, false},   {"SBC", &cpu::SBC, &cpu::INDY, 5, true},   {"NOP", &cpu::NOP, &cpu::IMPL, 2, false},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"SBC", &cpu::SBC, &cpu::ZPGX, 4, false}, {"INC", &cpu::INC, &cpu::ZPGX, 6, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"SED", &cpu::SED, &cpu::IMPL, 2, false}, {"SBC", &cpu::SBC, &cpu::ABSY, 4, true},  {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}, {"SBC", &cpu::SBC, &cpu::ABSX, 4, true}, {"INC", &cpu::INC, &cpu::ABSX, 7, false}, {"NOP", &cpu::NOP, &cpu::IMPL, 2, false}
    };

}

cpu::~cpu()
{
    //nothing to do
}

void cpu::reset()
{
    status = 0x34; 
    stackPtr = 0xfd; //base of stack in memory is 0x100, stackptr represents the offset and grows downwards
  
    instrCycles = 0;

    //pc needs to jump to address at 0xfffc and 0xfffd (LITTLE ENDIAN)

}

void cpu::setStatus(Flag flag, bool value)
{
    if (value)
        status |= flag;
    else
        status &= ~flag;
    
}

uint8_t cpu::getStatus(Flag flag)
{
    return (status & flag) > 0 ? 1 : 0;
}

uint8_t cpu::memRead(uint16_t addr)
{
    if (addr >= 0x0000 && addr <= RAMSIZE - 1)
    {
        return ram[addr];
    }

    return 0x00;
}

void cpu::memWrite(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= RAMSIZE - 1)
    {
        ram[addr] = data;
    }

    //std::cout << "[RAM] $" << std::hex << addr << ": 0x" << (uint16_t)ram[addr] << "\n";
}

uint8_t cpu::fetchData(bool(cpu::*addrmode)())
{
    if (addrmode == &cpu::IMPL)
    {
        fetchedData = A;
    }
    else
    {
        fetchedData = memRead(absoluteAddr);   
    }
    
    return fetchedData;
}

void cpu::pushStack(uint8_t data)
{
    stackPtr = (stackPtr - 1) & 0xFF;
    memWrite(STACK_OFFSET | stackPtr, data);
}
uint8_t cpu::popStack()
{
    uint8_t data = memRead(STACK_OFFSET | stackPtr);
    stackPtr = (stackPtr + 1) & 0xFF;
    return data;
}

bool cpu::instrComplete()
{
    return instrCycles == 0;
}

bool cpu::emulationDone()
{
    return endEmulation;
}

void cpu::step()
{
   if (instrCycles == 0)
   {
        currentOpcode = memRead(pc++);

        instrCycles = opcodeLookup[currentOpcode].cycles;

        bool crossedPageBoundary = (this->*opcodeLookup[currentOpcode].addrmode)();
        if (crossedPageBoundary && opcodeLookup[currentOpcode].affectedByPageBoundaryCrossing) 
        {
            instrCycles++;

            //TODO: test this case
            //std::cout << std::hex << (uint16_t)opcode << ": " << opcodeLookup[opcode].name << " REQUIRES AN EXTRA CYCLE\n";
        }

        fetchData((opcodeLookup[currentOpcode].addrmode));
        (this->*opcodeLookup[currentOpcode].operation)();

        //end emulation when BRK is encountered for now
        endEmulation = !opcodeLookup[currentOpcode].name.compare("BRK"); 
        
   }

   instrCycles--;
   systemCycles++;
}

void cpu::loadProgram(std::istringstream& prog, uint16_t addr)
{
    uint16_t location = addr;

    std::string byte;
    while(prog >> byte)
    {
        uint32_t num = std::stoul(byte, nullptr, 16);
        ram[location++] = (uint8_t)num;
    }
    
}

//whether two addresses are on the same page or not
static bool samePage(uint16_t addrA, uint16_t addrB)
{
    return (addrA & 0xFF00) == (addrB & 0xFF00);
}

/* ADDRESSING MODES */
bool cpu::ABS()
{
    uint8_t lo = memRead(pc++);
    uint8_t hi = memRead(pc++);
    absoluteAddr = (hi << 8) | lo;

    return false;
}
bool cpu::ABSX()
{
    uint8_t lo = memRead(pc++);
    uint8_t hi = memRead(pc++);

    uint16_t relativeAddr = (hi << 8) | lo;

    absoluteAddr = relativeAddr + X;

    return !samePage(absoluteAddr, relativeAddr);
}
bool cpu::ABSY()
{
    uint8_t lo = memRead(pc++);
    uint8_t hi = memRead(pc++);

    uint16_t relativeAddr = (hi << 8) | lo;

    absoluteAddr = relativeAddr + Y;

    return !samePage(absoluteAddr, relativeAddr);
}
bool cpu::IMM()
{
    absoluteAddr = pc++;
    return false;
}
bool cpu::IMPL()
{
    return false;
}
bool cpu::IND()
{
    uint8_t lo = memRead(pc++);
    uint8_t hi = memRead(pc++);

    uint16_t indirectAddr = (hi << 8) | lo;

    uint8_t absAddrLo = memRead(indirectAddr);
    uint8_t absAddrHi = memRead(indirectAddr + 1);

    absoluteAddr = (absAddrHi << 8) | absAddrLo;
    
    return false;
}
bool cpu::INDX()
{  
    uint8_t zeroPageAddr = (memRead(pc++) + X) & 0x00FF;

    uint8_t relativeLo = memRead(zeroPageAddr);
    uint8_t relativeHi = memRead(zeroPageAddr + 1);
    uint16_t relativeAddr = (relativeHi << 8) | relativeLo;

    absoluteAddr = relativeAddr;

    return false;
}
bool cpu::INDY()
{
    uint8_t zeroPageAddr = memRead(pc++) & 0x00FF;
    uint8_t relativeLo = memRead(zeroPageAddr);
    uint8_t relativeHi = memRead(zeroPageAddr + 1);

    uint16_t relativeAddr = (relativeHi << 8) | relativeLo;

    absoluteAddr = relativeAddr + Y;
    
    return !samePage(relativeAddr,absoluteAddr); 
} 
bool cpu::REL()
{
    uint8_t relativeAddr8bit = memRead(pc++);
    uint16_t relativeAddr = relativeAddr8bit;

    //sign extended the 8bit offset if its negative
    if (relativeAddr8bit & 0x80)
    {
        relativeAddr = relativeAddr8bit | 0xFF00;
    }

    absoluteAddr = pc + relativeAddr;

    instrCycles++;

    if (!samePage(pc,absoluteAddr))
    {
        instrCycles++;
    }

    return false;
} 
bool cpu::ZPG()
{
    absoluteAddr = memRead(pc++) & 0x00FF;
    return false;
} 
bool cpu::ZPGX()
{
    absoluteAddr = (memRead(pc++) + X) & 0x00FF;
    return false;
} 
bool cpu::ZPGY()
{
    absoluteAddr = (memRead(pc++) + Y) & 0x00FF;
    return false;
} 

/* INSTRUCTION OPERATIONS */
void cpu::ADC()
{
    uint16_t result = 0;

    result = (uint16_t)A + (uint16_t)fetchedData + (uint16_t)getStatus(C);
    uint8_t overflow = ~(((A^fetchedData) & 0x80) >> 7) & (((A^result) & 0x80) >> 7);

    A = (result & 0xff);


    setStatus(N, (A & 0x80) >> 7);
    setStatus(V, overflow);
    setStatus(Z, A == 0x00);
    setStatus(C, result > 255);

}
void cpu::AND()
{
    A = A & fetchedData;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z , A == 0x00);
}
void cpu::ASL()
{
    setStatus(C, (fetchedData & 0x80) >> 7);
    fetchedData = fetchedData << 1;
    setStatus(Z, fetchedData == 0x00);
    setStatus(N, (fetchedData & 0x80) >> 7);
    if (currentOpcode == 0x0A)
    {
        A = fetchedData;
    }
    else{
        memWrite(absoluteAddr,fetchedData);
    }
}
void cpu::BCC()
{
    if (getStatus(C) == 0)
    {
        pc = absoluteAddr;
    }
}
void cpu::BCS()
{
    if (getStatus(C) == 1)
    {
        pc = absoluteAddr;
    }
}
void cpu::BEQ()
{
    if (getStatus(Z) == 1)
    {
        pc = absoluteAddr;
    }
}
void cpu::BIT()
{
    setStatus(N, (fetchedData & 0x80) >> 7);
    setStatus(V, (fetchedData & 0x40) >> 6);
    setStatus(Z, (A & fetchedData) == 0x00);
}
void cpu::BMI()
{
     if (getStatus(N) == 1)
    {
        pc = absoluteAddr;
    }
}
void cpu::BNE()
{
    if (getStatus(Z) == 0)
    {
        pc = absoluteAddr;
    }

}
void cpu::BPL()
{
    if (getStatus(N) == 0)
    {
        pc = absoluteAddr;
    }
}
void cpu::BRK()
{
    //does nothing for now
}
void cpu::BVC()
{
    if (getStatus(V) == 0)
    {
        pc = absoluteAddr;
    }
}
void cpu::BVS()
{
    if (getStatus(V) == 1)
    {
        pc = absoluteAddr;
    }
}
void cpu::CLC(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::CLD(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::CLI(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::CLV(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::CMP()
{
    //doing a 2's complement addtion: A + (-fetchedData)
    uint8_t negData = ~(fetchedData) + 1;
   
    uint16_t result = A + (negData);
    
    setStatus(N, (result & 0x0080) >> 7);
    setStatus(Z, (result & 0x00FF) == 0x0000);
    setStatus(C, result & 0x0100);
}
void cpu::CPX()
{
    uint8_t negData = ~(fetchedData) + 1;
   
    uint16_t result = X + (negData);
    
    setStatus(N, (result & 0x0080) >> 7);
    setStatus(Z, (result & 0x00FF) == 0x0000);
    setStatus(C, result & 0x0100);

}
void cpu::CPY()
{
    uint8_t negData = ~(fetchedData) + 1;
   
    uint16_t result = Y + (negData);
    
    setStatus(N, (result & 0x0080) >> 7);
    setStatus(Z, (result & 0x00FF) == 0x0000);
    setStatus(C, result & 0x0100);
}
void cpu::DEC()
{
    fetchedData = fetchedData - 1;
    setStatus(N, (fetchedData & 0x80) >> 7);
    setStatus(Z, fetchedData == 0x00);
    memWrite(absoluteAddr, fetchedData);
}
void cpu::DEX()
{
    X = (X - 1) & 0xFF;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::DEY()
{
    Y = (Y - 1) & 0xFF;
    setStatus(N, (Y & 0x80) >> 7);
    setStatus(Z, Y == 0x00);   
}
void cpu::EOR()
{
    A = A ^ fetchedData;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);
}
void cpu::INC()
{
    fetchedData = fetchedData + 1;
    setStatus(N, (fetchedData & 0x80) >> 7);
    setStatus(Z, fetchedData == 0x00);
    memWrite(absoluteAddr, fetchedData);
}
void cpu::INX()
{
    X = (X + 1) & 0xFF;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::INY()
{
    Y = (Y + 1) & 0xFF;
    setStatus(N, (Y & 0x80) >> 7);
    setStatus(Z, Y == 0x00);
}

void cpu::JMP()
{
    pc = absoluteAddr;
}
void cpu::JSR()
{
    //since addressing mode bumps PC to the next instruction just save that on the stack
    uint8_t pcLo = pc & 0x00FF;
    uint8_t pcHi = (pc & 0xFF00) >> 8;
    pushStack(pcLo);
    pushStack(pcHi);    

    pc = absoluteAddr;
}
void cpu::LDA()
{
    A = fetchedData;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);

}
void cpu::LDX()
{
    X = fetchedData;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::LDY()
{
    Y = fetchedData;
    setStatus(N, (Y & 0x80) >> 7);
    setStatus(Z, Y == 0x00);
}
void cpu::LSR()
{
    setStatus(C, fetchedData & 0x01);
    fetchedData = fetchedData >> 1;
    setStatus(Z, fetchedData == 0x00);

    if (currentOpcode == 0x4A) //accumulator addressing mode
    {
        A = fetchedData;
    }
    else
    {
        memWrite(absoluteAddr,fetchedData);
    }
    
}
void cpu::NOP()
{
    //does nothing
}
void cpu::ORA()
{
    A = A | fetchedData;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);
}
void cpu::PHA(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::PHP(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::PLA(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::PLP(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::ROL()
{
    uint8_t carry = getStatus(C);
    setStatus(C, (fetchedData & 0x80) >> 7);
    fetchedData = fetchedData << 1;
    fetchedData = fetchedData | carry;

    setStatus(N, (fetchedData & 0x80) >> 7);
    setStatus(Z, fetchedData == 0x00);

    if (currentOpcode == 0x2A)
    {
        A = fetchedData;
    }
    else
    {
        memWrite(absoluteAddr, fetchedData);
    }
    
}
void cpu::ROR()
{
    uint8_t carry = getStatus(C);
    setStatus(C, (fetchedData & 0x01));
    fetchedData = fetchedData >> 1;
    fetchedData = fetchedData | (carry << 7);

    setStatus(N, (fetchedData & 0x80) >> 7);
    setStatus(Z, fetchedData == 0x00);

    if (currentOpcode == 0x6A)
    {
        A = fetchedData;
    }
    else
    {
        memWrite(absoluteAddr, fetchedData);
    }
}
void cpu::RTI(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::RTS()
{
    uint8_t pcHi = popStack();
    uint8_t pcLo = popStack();
    
    pc = (pcHi << 8) | pcLo;
}
void cpu::SBC()
{
    uint16_t result = 0;

    uint8_t onesComplementData = ~fetchedData;
    result = (uint16_t)A + (uint16_t)onesComplementData + (uint16_t)getStatus(C);
    uint8_t overflow = ~(((A^onesComplementData) & 0x80) >> 7) & (((A^result) & 0x80) >> 7);

    A = (result & 0xff);

    setStatus(N, (A & 0x80) >> 7);
    setStatus(V, overflow);
    setStatus(Z, A == 0x00);
    setStatus(C, result > 255);
}
void cpu::SEC(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::SED(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::SEI(){std::cout << __func__ << "\n"; exit(-1);}
void cpu::STA()
{
    memWrite(absoluteAddr, A);
}
void cpu::STX()
{
    memWrite(absoluteAddr, X);
}
void cpu::STY()
{
    memWrite(absoluteAddr, Y);
}
void cpu::TAX()
{
    X = A;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::TAY()
{
    Y = A;
    setStatus(N, (Y & 0x80) >> 7);
    setStatus(Z, Y == 0x00);
}
void cpu::TSX()
{
    X = stackPtr;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::TXA()
{
    A = X;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);
}
void cpu::TXS()
{
    stackPtr = X;
    setStatus(N, (stackPtr & 0x80) >> 7);
    setStatus(Z, stackPtr == 0x00);
}
void cpu::TYA()
{
    A = Y;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);
}


/* DEBUGGING FUNCTIONS */
void cpu::printCPU()
{ 
/*
    std::cout << "A: 0x" << std::hex << (uint16_t)A << " X: 0x" << std::hex << (uint16_t)X << " Y: 0x" << std::hex << (uint16_t)Y << "\n";
    std::cout << "N V - B D I Z C\n";
    std::cout << (uint16_t)getStatus(N) << " " << (uint16_t)getStatus(V) << " " << (uint16_t)getStatus(U) << " " << (uint16_t)getStatus(B) << " " << (uint16_t)getStatus(D) << " " << (uint16_t)getStatus(I) << " " << (uint16_t)getStatus(Z) << " " << (uint16_t)getStatus(C) << "\n";
    std::cout << "Next PC: 0x" << std::hex << pc << " StackPtr: 0x" << std::hex << (uint16_t)stackPtr << "\n";
    std::cout << "total clock cycles: " << std::dec << systemCycles << "\n";
    std::cout << "---------------------------------------------------\n";
*/

    std::cout << "A:" <<std::hex << (uint16_t)A << " X:" << (uint16_t)X << " Y:" << (uint16_t)Y;// << " P:" << printP(status) << "\n";
   // std::cout << " P(NV-BDIZC):" << (uint16_t)getStatus(N) << (uint16_t)getStatus(V) << (uint16_t)getStatus(U) << (uint16_t)getStatus(B) << (uint16_t)getStatus(D) << (uint16_t)getStatus(I) << (uint16_t)getStatus(Z) << (uint16_t)getStatus(C);
    std::cout << " P:" << (uint16_t)status;
    std::cout << " SP:" << std::hex << (uint16_t)stackPtr;
    std::cout << " CYC:" << std::dec << systemCycles << "\n";
}

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
uint16_t cpu::getPC()
{
    return pc;
}
uint8_t cpu::getStackPtr()
{
    return stackPtr;
}

std::unordered_map<uint16_t, std::string> cpu::disassemble()
{
    std::unordered_map<uint16_t, std::string> map;

    uint16_t startAddr = 0x0000;
    uint16_t endAddr = 0xFFFF;
    uint32_t addr = startAddr; //pc
    uint16_t instrAddr;

    std::string instr;


    auto hex = [](uint16_t data, uint8_t numDigits)
    {
        std::string s('0', numDigits);
        std::string hexAlphabet = "0123456789ABCDEF";

        for (int i = numDigits - 1; i >= 0; i--)
        {
            s[i] = hexAlphabet[data & 0x000F];
            data >>= 4;
        }

        return s;
    }; 

    uint8_t loData = 0;
    uint8_t hiData = 0;
    while(addr <= (uint32_t)endAddr)
    {
        instrAddr = addr;
        uint8_t opcode = memRead(addr++);
        

        instr += opcodeLookup[opcode].name + " ";
        if (opcodeLookup[opcode].addrmode == &cpu::IMPL)
        {
            instr += " {IMPL}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::IMM)
        {
            loData = memRead(addr++);
            instr += "#$" + hex(loData,2) + " {IMM}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::ABS)
        {
            loData = memRead(addr++);
            hiData = memRead(addr++);

            instr += "$" + hex((hiData << 8) | loData,4) + " {ABS}";
        }        
        else if (opcodeLookup[opcode].addrmode == &cpu::ABSX)
        {
            loData = memRead(addr++);
            hiData = memRead(addr++);

            uint16_t relativeAddr = (hiData << 8) | loData;

            instr += "$" + hex(relativeAddr,4) + ",X {ABSX}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::ABSY)
        {
            loData = memRead(addr++);
            hiData = memRead(addr++);

            uint16_t relativeAddr = (hiData << 8) | loData;

            instr += "$" + hex(relativeAddr,4) + ",Y {ABSY}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::IND)
        {
            loData = memRead(addr++);
            hiData = memRead(addr++);

            uint16_t indirectAddr = (hiData << 8) | loData;

            instr += "($" + hex(indirectAddr,4) + ") {IND}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::INDX)
        {
            loData = memRead(addr++);
            instr += "($" + hex(loData,2) + ",X) {INDX}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::INDY)
        {
            loData = memRead(addr++);
            instr += "($" + hex(loData,2) + "),Y {INDY}";
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::ZPG)
        {
            loData = memRead(addr++) & 0xFF;
            instr += "$" + hex(loData,2) + " {ZPG}";   
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::ZPGX)
        {
            loData = memRead(addr++) & 0xFF;
            instr += "$" + hex(loData,2) + ",X {ZPGX}";       
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::ZPGY)
        {
            loData = memRead(addr++) & 0xFF;
            instr += "$" + hex(loData,2) + ",Y {ZPGY}";  
        }
        else if (opcodeLookup[opcode].addrmode == &cpu::REL)
        {
            loData = memRead(addr++);
            uint16_t relativeAddr = loData;
            if (loData & 0x80)
            {
                relativeAddr = loData | 0xFF00;
            }

            instr += "$" + hex(relativeAddr + addr, 4) + " {REL}";
        }
                   
        map[instrAddr] = instr;
        instr = "";
    }

    return map;
}