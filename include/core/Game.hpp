#pragma once

#include <iostream>
#include <SDL2/SDL.h>

class Game {
public: 
    Game();
    ~Game();

    bool Initialize(const char* title, int width, int height, bool fullScreen);
    void Run();
    void ShutDown();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning = false;
    int _width = 0;
    int _height = 0;
};