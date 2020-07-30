#pragma once

#include <iostream>

class cpu{

    private:
        /* Registers */
        uint8_t A;
        uint8_t X;
        uint8_t Y;

        uint8_t stackPtr;

        uint8_t status; //processor status flags
        enum Flag {
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

    //Emulator Functions
    public:
        cpu();
        ~cpu();
        
        void reset();

        void setStatus(Flag flag, bool value);
        uint8_t getStatus(Flag flag);

    //Debugging Functions
    public:
       void print();
       uint8_t getStatusReg();
       uint8_t getA();
       uint8_t getX();
       uint8_t getY();
       uint8_t getPC();
       uint8_t getStackPtr();

};