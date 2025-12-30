#include <SDL.h>
#include "chip8.h"
#include <iostream>
#include <vector>


//Screen dimension constants
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

//int main(int argc, char *argv[])
int main()
{
    bool gameIsRunning = true;

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
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Init(SDL_INIT_EVERYTHING);
    unsigned int scaler = 10;
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH*scaler, SCREEN_HEIGHT*scaler, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, scaler, scaler);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    

    while (gameIsRunning) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                gameIsRunning = false;
            }
        }
        
        chip.emulateCycle();
        if (chip.drawflag) {
            
            chip.drawflag = false;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            

            std::vector<SDL_Point> points;
            points.reserve(SCREEN_HEIGHT*SCREEN_WIDTH);

            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    if (chip.video[y*64 + x] != 0) { // Pixel is on
                        points.push_back(SDL_Point{x, y});
                    }
                }
            }
            if (!points.empty()){
                SDL_RenderDrawPoints(renderer, points.data(), static_cast<int>(points.size()));
            }
            SDL_RenderPresent(renderer);
        }
        SDL_Delay(2);
    }

    return 0;
}