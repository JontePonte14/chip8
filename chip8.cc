#include "chip8.h"
#include <iostream>

chip8::chip8(){

}

void chip8::initialize(){
    pc = 0x200;
    opcode = 0;
    index = 0;
    sp = 0;


    // Clear display	
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Loading fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
}

void chip8::emulateCycle(){
    // Fetches opcode using current pc
    opcode = memory[pc] << 8 | memory[pc + 1];
    
    //Deocde instruction


}

void chip8::opcodeDecoderExecuter(){

    switch (opcode & 0xF000)
    {
    case 0x0000:
        // CLS
        if (opcode == 0x00E0){
            pc = pc + 2;
        }
        // RET
        else if (opcode == 0x00EE){
            pc = pc + 2;
        }
        // Error
        else {
            std::cerr << "Error: Unknown Opcode: " << opcode << std::endl;
        }
        break;
    
    case 0x1000:
        // JP addr
        pc = opcode & 0x0FFF;
        break;
    
    case 0x2000:
        // CALL addr
        sp = sp + 1;
        stack[sp] = pc;
        pc = opcode & 0x0FFF;
        break;

    case 0x300:
        // SE Vx, byte
        //unsigned int address = 0x0F00 & opcode;
        if (registers[0x0F00 & opcode] == (0x00FF & opcode)) {
            pc = pc + 4;
        } else {
            pc = pc + 2;
        }
        break;
    
    case 0x400:
        // SNE Vx, byte
        0x0F00 & opcode;
        if (registers[0x0F00 & opcode] != (0x00FF & opcode)) {
            pc = pc + 4;
        } else {
            pc = pc + 2;
        }
        break;
        
    //Set I = nnn.
    case 0xA000:
        index = opcode & 0x0FFF;
        pc = pc + 2;
        break;
    
    default:
        std::cerr << "Error: Unknown Opcode: " << opcode << std::endl;
        break;
    }
}

void chip8::updatePC(){
    pc = pc + 2;
}