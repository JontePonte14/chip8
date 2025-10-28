#include "chip8.h"
#include <iostream>

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
}
