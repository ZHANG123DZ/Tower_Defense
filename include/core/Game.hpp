#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Game {
public: 
    Game();
    ~Game();

    bool Initialize(const char* title, int width, int height, bool fullScreen);
    void Run();
    void ShutDown();
    void setFont(TTF_Font* font);
    void setBgMusic(Mix_Music* bgMusic);
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font;
    Mix_Music* bgMusic;
    bool isRunning = false;
    int _width = 0;
    int _height = 0;
};