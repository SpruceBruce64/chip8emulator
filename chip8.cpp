#include "chip8.h"
#include <stdio.h>
#include <fstream>

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8::initialize(){
    sound = false;
    //Initialize registers and memory
    pc     = 0x200; // Program counter starts at 0x200
    opcode = 0;     // Reset current op code
    I      = 0;     // Reset index register
    sp     = 0;     // Rest stack pointer

    //clear display
    //clear stack
    //clear registers V0-VF
    // clear memory

    //Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];

    //reset timers

}

void chip8::loadGame(std::string gamePath){
    printf("%s\n",gamePath.c_str());
    std::ifstream file(gamePath.c_str(), std::ios::binary);
    //for(int i = 0; i < file.)
    int i = 0;
    while(!file.eof()){
        memory[0x200 + i] = file.get();
        printf("%d ",memory[0x200 + i]);
        i++;
    }
    printf("\n");
    file.close();
}

void chip8::emulateCycle(){
    // fetch op code
    opcode = memory[pc] << 8 | memory[pc + 1];
    //printf("opcode: %X \n", opcode);
    // decode op code
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000: // 0x00E0: Clears the screen
                    //execute opcode
                    for(int i = 0; i < 2048; i++){
                        gfx[i] = 0;
                    }
                    drawFlag = true;
                    pc += 2;
                break;

                case 0x000E: // 0x00EE: Returns from subroutine
                    //execute opcode
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                break;

                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
                break;
            }
        break;

        case 0x1000: // 1NNN: Jumps to address NNN.
            pc = opcode & 0x0FFF;
        break;

        case 0x2000: // 2NNN: Calls subroutine at NNN.
            //if(sp < 15){
                stack[sp] = pc;
                sp++;
                pc = opcode & 0x0FFF;
            //}
        break;

        case 0x3000: // 3XNN: Skips the next instruction if VX equals NN (usually the next instruction is a jump to skip a code block).
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break;

        case 0x6000: // 6XNN: Sets VX to NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
        break;

        case 0x7000: // 7XNN: Adds NN to VX (carry flag is not changed).
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
        break;

        case 0xA000: // ANNN: Sets I to the address NNN
            //Execute opcode
            I = opcode & 0x0FFF;
            pc += 2;
        break;

        case 0xC000: // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
        break;

        case 0xD000:
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for(int yline = 0; yline < height; yline++){
                pixel = memory[I + yline];
                for(int xline = 0; xline < 8; xline++){
                    if((pixel & (0x80 >> xline)) != 0){
                        if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                            V[0xF] = 1;
                        gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }
            drawFlag = true;
            pc += 2;
        }
        break;

        case 0xF000:
            switch(opcode & 0x00FF){

                case 0x0007: // 0xFX07: Sets VX to the value of the delay timer.
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break;

                case 0x0015: // 0xFX15: Sets the delay timer to VX.
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;

                case 0x0029: // 0xFX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    pc += 2;
                break;

                case 0x0033: // 0xFX35: Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
                    //execute opcode
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
                    pc += 2;
                break;

                case 0x0065: // 0xFX65: Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified.
                    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++){
                        V[i] = memory[I + i];
                    }
                    pc += 2;
                break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
                break;
            }
        break;

        default:
            printf("unknown op code, 0x%X\n", opcode);
    }
    // execute op code

    // update timers
    if(delay_timer > 0)
        --delay_timer;
    
    if(sound_timer > 0){
        if(sound_timer == 1)
            printf("BOOP!\n");
        --sound_timer;
    }
}