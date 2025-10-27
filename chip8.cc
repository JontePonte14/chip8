#include "chip8.h"

void chip8::initialize(){
    pc = 0x200;
    opcode = 0;
    index = 0;
    sp = 0;

}

void chip8::emulateCycle(){

}

void chip8::updatePC(){
    pc = pc + 2;
}