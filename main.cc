#include <SDL.h>
#include "chip8.h"
#include <iostream>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//int main(int argc, char *argv[])
int main()
{
    // Loading program
    std::cout << "Testing file" << std::endl;
    chip8 chip;
    chip.initialize();
    
    std::string programName = "/home/jonathan/Documents/cpp_project/chip8/chip8-roms/games/Airplane.ch8";
    if (chip.loadProgram(programName)){
        std::cout << "Succesfully loaded the program" << std::endl;
    } else {
        std::cout << "Failed loading the program, Shutting down program" << std::endl;
        return 1;
    }

    // Starting to run window

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    return 0;
}