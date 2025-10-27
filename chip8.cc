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
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = (opcode & 0x000F);
    uint16_t kk = (opcode & 0x00FF);
    uint16_t nnn = (opcode & 0x0FFF);
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
        pc = nnn;
        break;
    
    case 0x2000:
        // CALL addr
        sp = sp + 1;
        stack[sp] = pc;
        pc = nnn;
        break;

    case 0x3000:
        // SE Vx, byte
        //unsigned int address = 0x0F00 & opcode;
        if (registers[x] == kk) {
            pc = pc + 4;
        } else {
            pc = pc + 2;
        }
        break;
    
    case 0x4000:
        // SNE Vx, byte
        //0x0F00 & opcode;
        if (registers[x] != kk) {
            pc = pc + 4;
        } else {
            pc = pc + 2;
        }
        break;

    case 0x5000:
        // SE Vx, Vy
        if (registers[x] != registers[y]) {
            pc  = pc + 4;
        } else {
            pc = pc + 2;
        }
        break;

    case 0x6000:
        // LD Vx, byte
        registers[x] = kk;
        pc = pc + 2;
        break;

    case 0x7000:
        // ADD Vx, byte
        registers[x] = registers[x] + kk;
        pc = pc + 2;
        break;
    
    case 0x8000: {
        switch (opcode & 0x000F)
        {
            case 0x0000:
                // LD Vx, Vy
                registers[x] = registers[y];
                pc = pc + 2;
                break;
            
            case 0x0001:
                // OR Vx, Vy
                registers[x] = registers[x] | registers[y];
                pc = pc + 2;
                break;

            case 0x0002:
                // AND Vx, Vy
                registers[x] = registers[x] & registers[y];
                pc = pc + 2;
                break;

            case 0x0003:
                // XOR Vx, Vy
                registers[x] = registers[x] ^ registers[y];
                pc = pc + 2;
                break;
            
            case 0x0004: {
                // Add Vx, Vy, set VF = carry
                uint16_t result = registers[x] + registers[y];
                if (result > 255) { // We have overflow
                    registers[15] = 1;
                } else {
                    registers[15] = 0;
                }
                registers[x] = result; // takes the 8 lower bits by default
                pc = pc + 2;
                break;
            }
            
            default:
                std::cerr << "Unknown opcode: " << opcode << std::endl;
                break;
        
        }
        break;
    }


    
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