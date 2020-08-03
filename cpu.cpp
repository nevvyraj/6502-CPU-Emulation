#include <iostream>
#include "cpu.h"

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
        uint8_t opcode = memRead(pc++);

        instrCycles = opcodeLookup[opcode].cycles;

        bool crossedPageBoundary = (this->*opcodeLookup[opcode].addrmode)();
        if (crossedPageBoundary && opcodeLookup[opcode].affectedByPageBoundaryCrossing) {instrCycles++;}

        fetchData((opcodeLookup[opcode].addrmode));
        (this->*opcodeLookup[opcode].operation)();

        //end emulation when BRK is encountered for now
        endEmulation = !opcodeLookup[opcode].name.compare("BRK"); 
        
   }

   instrCycles--;
   systemCycles++;
}

void cpu::loadProgram(std::istringstream& prog)
{
    uint16_t location = 0x8000;

    std::string byte;
    while(prog >> byte)
    {
        uint32_t num = std::stoul(byte, nullptr, 16);
        ram[location++] = (uint8_t)num;
    }
    
}

/* ADDRESSING MODES */
bool cpu::ABS()
{
    std::cout << __func__ << "\n";
    return false;
}
bool cpu::ABSX()
{
    std::cout << __func__ << "\n";
    return true;
}
bool cpu::ABSY()
{
    std::cout << __func__ << "\n";
    return true;
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
    std::cout << __func__ << "\n";
    return false;
}
bool cpu::INDX()
{  
    std::cout << __func__ << "\n";
    return false;
}
bool cpu::INDY()
{
    std::cout << __func__ << "\n";
    return true;
} 
bool cpu::REL()
{
    std::cout << __func__ << "\n";
    return false;
} 
bool cpu::ZPG()
{
    std::cout << __func__ << "\n";
    return false;
} 
bool cpu::ZPGX()
{
    std::cout << __func__ << "\n";
    return false;
} 
bool cpu::ZPGY()
{
    std::cout << __func__ << "\n";
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
void cpu::AND(){std::cout << __func__ << "\n";}
void cpu::ASL(){std::cout << __func__ << "\n";}
void cpu::BCC(){std::cout << __func__ << "\n";}
void cpu::BCS(){std::cout << __func__ << "\n";}
void cpu::BEQ(){std::cout << __func__ << "\n";}
void cpu::BIT(){std::cout << __func__ << "\n";}
void cpu::BMI(){std::cout << __func__ << "\n";}
void cpu::BNE(){std::cout << __func__ << "\n";}
void cpu::BPL(){std::cout << __func__ << "\n";}
void cpu::BRK(){std::cout << __func__ << "\n";}
void cpu::BVC(){std::cout << __func__ << "\n";}
void cpu::BVS(){std::cout << __func__ << "\n";}
void cpu::CLC(){std::cout << __func__ << "\n";}
void cpu::CLD(){std::cout << __func__ << "\n";}
void cpu::CLI(){std::cout << __func__ << "\n";}
void cpu::CLV(){std::cout << __func__ << "\n";}
void cpu::CMP(){std::cout << __func__ << "\n";}
void cpu::CPX(){std::cout << __func__ << "\n";}
void cpu::CPY(){std::cout << __func__ << "\n";}
void cpu::DEC(){std::cout << __func__ << "\n";}
void cpu::DEX(){std::cout << __func__ << "\n";}
void cpu::DEY(){std::cout << __func__ << "\n";}
void cpu::EOR(){std::cout << __func__ << "\n";}
void cpu::INC(){std::cout << __func__ << "\n";}
void cpu::INX()
{
    X = X + 1;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::INY(){std::cout << __func__ << "\n";}
void cpu::JMP(){std::cout << __func__ << "\n";}
void cpu::JSR(){std::cout << __func__ << "\n";}
void cpu::LDA()
{
    A = fetchedData;
    setStatus(N, (A & 0x80) >> 7);
    setStatus(Z, A == 0x00);

}
void cpu::LDX(){std::cout << __func__ << "\n";}
void cpu::LDY(){std::cout << __func__ << "\n";}
void cpu::LSR(){std::cout << __func__ << "\n";}
void cpu::NOP(){std::cout << __func__ << "\n";}
void cpu::ORA(){std::cout << __func__ << "\n";}
void cpu::PHA(){std::cout << __func__ << "\n";}
void cpu::PHP(){std::cout << __func__ << "\n";}
void cpu::PLA(){std::cout << __func__ << "\n";}
void cpu::PLP(){std::cout << __func__ << "\n";}
void cpu::ROL(){std::cout << __func__ << "\n";}
void cpu::ROR(){std::cout << __func__ << "\n";}
void cpu::RTI(){std::cout << __func__ << "\n";}
void cpu::RTS(){std::cout << __func__ << "\n";}
void cpu::SBC(){std::cout << __func__ << "\n";}
void cpu::SEC(){std::cout << __func__ << "\n";}
void cpu::SED(){std::cout << __func__ << "\n";}
void cpu::SEI(){std::cout << __func__ << "\n";}
void cpu::STA(){std::cout << __func__ << "\n";}
void cpu::STX(){std::cout << __func__ << "\n";}
void cpu::STY(){std::cout << __func__ << "\n";}
void cpu::TAX()
{
    X = A;
    setStatus(N, (X & 0x80) >> 7);
    setStatus(Z, X == 0x00);
}
void cpu::TAY(){std::cout << __func__ << "\n";}
void cpu::TSX(){std::cout << __func__ << "\n";}
void cpu::TXA(){std::cout << __func__ << "\n";}
void cpu::TXS(){std::cout << __func__ << "\n";}
void cpu::TYA(){std::cout << __func__ << "\n";}


/* DEBUGGING FUNCTIONS */
void cpu::printCPU()
{ 
    std::cout << "---------------------------------------------------\n";
    std::cout << "A: 0x" << std::hex << (uint16_t)A << " X: 0x" << std::hex << (uint16_t)X << " Y: 0x" << std::hex << (uint16_t)Y << "\n";
    std::cout << "N V - B D I Z C\n";
    std::cout << (uint16_t)getStatus(N) << " " << (uint16_t)getStatus(V) << " " << (uint16_t)getStatus(U) << " " << (uint16_t)getStatus(B) << " " << (uint16_t)getStatus(D) << " " << (uint16_t)getStatus(I) << " " << (uint16_t)getStatus(Z) << " " << (uint16_t)getStatus(C) << "\n";
    std::cout << "PC: 0x" << std::hex << pc << " StackPtr: 0x" << std::hex << (uint16_t)stackPtr << "\n";
    std::cout << "total clock cycles: " << std::dec << systemCycles << "\n";
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
uint8_t cpu::getPC()
{
    return pc;
}
uint8_t cpu::getStackPtr()
{
    return stackPtr;
}