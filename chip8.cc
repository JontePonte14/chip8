#include "chip8.h"
#include <iostream>

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
    
    // Opcode that starts with zero
    case 0x0000:

        // Clear the display.
        // CLS
        if (opcode == 0x00E0){

        }
        // RET
        //  Return from a subroutine. 
        else if (opcode == 0x00EE){

        }
        // SYS addr
        // Jump to a machine code routine at nnn. 
        else {
            pc = opcode & 0x0FFF;
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