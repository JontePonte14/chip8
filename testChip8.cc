#include "chip8.h"
#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "Testing file" << std::endl;
    chip8 chip;
    chip.initialize();
    
    std::string programName = "/home/jonathan/Documents/cpp_project/chip8/chip8-roms/games/Airplane.ch8";
    if (chip.loadProgram(programName)){
        std::cout << "Succesfully loaded the program" << std::endl;
    } else {
        std::cout << "Shutting down program" << std::endl;
        return 1;
    }

    chip.printMemory();
    std::cout << std::hex << std::setfill('0');
    for (int i = 0; i < 10; i++) {
        std::cout << "PC: 0x" << chip.pc << std::endl;
        chip.emulateCycle();
        std::cout << "opcode " << i << ": 0x" 
            << std::setw(4) << chip.opcode << std::endl;
    }
}
