#ifndef CHIP8EMU
#define CHIP8EMU

#include <string>

class chip8{

    private:
        unsigned short opcode;

        unsigned char memory[4096];

        //cpu registers
        unsigned char V[16];

        //Index register
        unsigned short I;

        // program counter
        unsigned short pc;

        //system memory map
        //0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
        //0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
        //0x200-0xFFF - Program ROM and work RAM

        //count down to 0 at 60 Hz
        unsigned char delay_timer;
        //buzzer sounds when 0 is reached
        unsigned char sound_timer;

        //stack and stack pointer
        unsigned short stack[16];
        unsigned short sp;

        //keypad for input
        unsigned char key[16];

    public:
        //graphics
        unsigned char gfx[64 * 32];

        bool sound;

        bool drawFlag = true;

        void initialize();

        void loadGame(std::string);

        void emulateCycle();
};

#endif /*CHIP8EMU*/
