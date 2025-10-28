#include "chip8.h"
#include <iostream>
#include <random>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs=std::filesystem;

std::mt19937 rng{std::random_device{}()};
std::uniform_int_distribution<int> dist(0, 255);

chip8::chip8(){

}

void chip8::initialize(){
    pc = 0x200;
    opcode = 0;
    index = 0;
    sp = 0;
    for (uint32_t& pixel : video) { pixel = 0;} // Clear display
    for (uint16_t& address : stack) {address = 0;} // Clear address
    for (uint8_t& reg : registers) {reg = 0;} // Clear registers V0-VF
    for (uint8_t& instruction : memory) {instruction = 0;} // Clear memory

    // Loading fontset
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }
}

void chip8::emulateCycle(){
    // Fetches opcode using current pc
    opcode = memory[pc] << 8 | memory[pc + 1];
    opcodeDecoderExecuter();

    //Update timers
    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        if (soundTimer == 1) {
            std::cout << "BEEEEEEEEEEEP" << std::endl;
        }
        soundTimer--;
    }

}

bool chip8::loadProgram(std::string& filename){
    fs::path filepath = filename;
    if (!fs::exists(filepath)) {
        cerr << "Error: Invalid filename, can't find the program" << endl;
        return false;
    }

    uintmax_t fileSize = fs::file_size(filepath); // filesize/2 = nbr instructions
    ifstream file(filepath, ios::binary);
    std::vector<char> buffer(fileSize);
    if (file.read(buffer.data(), fileSize)) {
        // Worked
    } else {
        cerr << "Error: Couldn't read file" << endl;
        return false;
    }

    if (0x200 + fileSize > sizeof(memory)) {
        cerr << "Error: ROM is too big" << endl;
    }

    for (uintmax_t i = 0; i < fileSize; i++){
        memory[0x200 + i] = static_cast<uint8_t>(buffer[i]);
    }

    return true;
}

void chip8::printMemory(){
    cout << "Printing out the first bytes" << endl;
    std::cout << std::hex << std::setfill('0');

    for (int i = 0; i < 20; i = i + 2) {
        uint8_t hi = memory[0x200 + i];
        uint8_t lo = memory[0x200 + i + 1];
        uint16_t doubleByte = (hi << 8) | lo; 

    std::cout << "Instruction " << (i / 2) << ": 0x"
              << std::setw(4) << doubleByte << std::endl;
    }
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
        if (opcode == 0x00E0) {
            for (uint32_t& pixel : video) {
                pixel = 0;
            }
            drawflag = true;
            pc += 2;
        // RET
        } else if (opcode == 0x00EE) {
            sp--;
            pc = stack[sp];
        } else {
            // SYS 0nnn: ignore in modern interpreters
            pc += 2;
        }
        break;
    
    case 0x1000:
        // JP addr
        pc = nnn;
        break;
    
    case 0x2000:
        // CALL addr
        stack[sp] = pc + 2;
        sp++;
        pc = nnn;
        break;

    case 0x3000:
        // SE Vx, byte
        //unsigned int address = 0x0F00 & opcode;
        if (registers[x] == kk) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;
    
    case 0x4000:
        // SNE Vx, byte
        //0x0F00 & opcode;
        if (registers[x] != kk) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;

    case 0x5000:
        // SE Vx, Vy
        if (n == 0) {
            if (registers[x] == registers[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
        } else {
            std::cerr << "Unknown opcode: " << opcode << std::endl;
        }

        break;

    case 0x6000:
        // LD Vx, byte
        registers[x] = kk;
        pc += 2;
        break;

    case 0x7000:
        // ADD Vx, byte
        registers[x] = registers[x] + kk;
        pc += 2;
        break;
    
    case 0x8000: {
        switch (opcode & 0x000F)
        {
            case 0x0000:
                // LD Vx, Vy
                registers[x] = registers[y];
                pc += 2;
                break;
            
            case 0x0001:
                // OR Vx, Vy
                registers[x] = registers[x] | registers[y];
                pc += 2;
                break;

            case 0x0002:
                // AND Vx, Vy
                registers[x] = registers[x] & registers[y];
                pc += 2;
                break;

            case 0x0003:
                // XOR Vx, Vy
                registers[x] = registers[x] ^ registers[y];
                pc += 2;
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
                pc += 2;
                break;
            }

            case 0x0005: {
                // SUB Vx, Vy
                if (registers[x] > registers[y]) {
                    registers[15] = 1;
                } else {
                    registers[15] = 0;
                }
                registers[x] = registers[x] - registers[y];
                pc += 2;
                break;
            }

            case 0x0006:
                // SHR Vx {, Vy}
                if (registers[x] % 2 == 0) {
                    registers[15] = 0;
                } else {
                    registers[15] = 1;
                }
                registers[x] = registers[x] >> 1;
                pc +=2;
                break;

            case 0x0007:
                // SUBN Vx, Vy
                if (registers[y] > registers[x]) {
                    registers[15] = 1;
                } else {
                    registers[15] = 0;
                }
                registers[x] = registers[y] - registers[x];
                pc += 2;
                break;
            
            case 0x000E:
                // SHL Vx {, Vy}
                // If Vx is greater than or equal 128
                // i.e msb is 1
                if (registers[x] & 0x80) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }
                registers[x] = registers[x] << 1;
                pc +=2;
                break;

            default:
                std::cerr << "Unknown opcode: " << opcode << std::endl;
                break;
        
        }
        break;
    }

    case 0x9000:
        // SNE Vx, Vy
        if (n == 0) {
            if (registers[x] != registers[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
        } else {
            std::cerr << "Unknown opcode: " << opcode << std::endl;
        }
        break;
    
    case 0xA000:
        // LD I, addr
        //Set I = nnn.    
        index = opcode & 0x0FFF;
        pc += 2;
        break;

    case 0xB000:
        // JP V0, addr
        pc = nnn + registers[0];
        break;

    case 0xC000: {
        // RND Vx, byte
        uint8_t randomNbr = static_cast<uint8_t>(dist(rng));
        registers[x] = randomNbr & kk;
        pc += 2;
        break;
        }

    case 0xD000: {
        // DRW Vx, Vy, nibble
        registers[15] = 0; // Clear collision from previous instruction
        for (int r = 0; r < n; r++){
            uint8_t spriteByte = memory[index + r];
            
            for (int c = 0; c < 8; c++){
                uint8_t spriteBit = (spriteByte >> (7-c)) & 0x01;

                // We want to draw
                if (spriteBit) {
                    int xCoord = (registers[x] + c) % 64;
                    int yCoord = (registers[y] + r) % 32;
                    int pixelIndex = yCoord * 64 + xCoord;

                    // Collision 
                    if (video[pixelIndex]) {
                        registers[15] = 1;
                    }

                    video[pixelIndex] = video[pixelIndex] ^ 1;
                }

            }
        }
        drawflag = true;
        pc += 2;

        break;
    }

    case 0xE000: {
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            // SKPP Vx
            // Keypad in down position 1 or zero? Needs to be checked
            if (keypad[registers[x]]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        
        case 0x00A1:
            // SKNP Vx
            if (!keypad[registers[x]]) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;
        
        default:
            std::cerr << "Error: Unknown Opcode: " << opcode << std::endl;
            break;
        }
        break;
    }

    case 0xF000: {
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            // LD Vx, DT
            registers[x] = delayTimer;
            pc += 2;
            break;
        
        case 0x000A:
            // LD Vx, K
            // Only increment PC if keypress is found
            // else stay on the same instruction
            for (int i = 0; i < 16; i++){
                if (keypad[i]){
                    registers[x] = i;
                    pc += 2;
                    break;
                }
            }

            break;

        case 0x0015:
            // LD DT, Vx
            delayTimer = registers[x];
            pc += 2;
            break;
        
        case 0x0018:
            // LD ST, Vx
            soundTimer = registers[x];
            pc += 2;
            break;
        
        case 0x001E:
            // ADD I, Vx
            index = index + registers[x];
            pc += 2;
            break;
        
        case 0x0029:
            // LD F, Vx
            index = registers[x] * 5;
            pc += 2;
            break;
        
        case 0x0033:{
            // LD B, Vx
            uint8_t nbr = registers[x];
            memory[index] = nbr / 100;
            memory[index+1] = (nbr / 10) % 10;
            memory[index+2] = nbr % 10;
            pc += 2;
            break;
        }

        case 0x0055:
            // LD [I], Vx
            for (int i = 0; i <= x; i++){
                memory[index+i] = registers[i];
            }
            pc += 2;
            break;

        case 0x0065:
            // LD Vx, [I]  
            for (int i = 0; i <= x; i++) {
                registers[i] = memory[index + i];
            }  
            pc += 2;
            break;
        
        default:
            std::cerr << "Error: Unknown Opcode: " << opcode << std::endl;
            break;
        }
        break;
    }
    
    default:
        std::cerr << "Error: Unknown Opcode: " << opcode << std::endl;
        break;
    }
}


