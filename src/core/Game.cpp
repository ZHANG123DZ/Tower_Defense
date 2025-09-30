#include <iostream>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <core/Game.hpp>
#include <core/Map.hpp>
Map *map =nullptr;

std::vector<std::vector<int>> level = {
    {0,0,0,1,1,1,2,2},
    {0,0,0,1,1,1,2,2},
    {4,4,4,5,5,5,6,6},
    {4,4,4,5,5,5,6,6},
    {0,1,2,3,4,5,6,7},
    {0,1,2,3,4,5,6,7},
    {0,1,2,3,4,5,6,7},
    {0,1,2,3,4,5,6,7},
};

Game::Game() : isRunning(false) {}

Game::~Game() {
    ShutDown();
}

bool Game::Initialize(const char* title, int width, int height, bool fullScreen = false) {
    if ((SDL_Init(SDL_INIT_VIDEO) || SDL_Init(SDL_INIT_AUDIO) || SDL_Init(SDL_INIT_TIMER) ) < 0) {
        return false;
    }
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullScreen) flags = SDL_WINDOW_FULLSCREEN;
    //Tạo ra cửa sổ (window)
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) {
        SDL_Quit();
        return false;
    }
    //Tạo ra bút vẽ (renderer): dùng để vẽ hình ảnh, text, màu nền, texture,… lên cửa sổ.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Quit();
        return false;
    }
    SDL_RenderSetScale(renderer, 4.0f, 4.0f);
    //Muốn thay map nào thì để đường dẫn ảnh vào đây
    map = new Map(renderer, "assets/1 Tiles/FieldsTileset.png", 32, 32, 8);
    map->loadMap(level);
    isRunning = true;
    return true;
}

void Game::Run() {
    SDL_Event e;
    while (isRunning)
    {
        while (SDL_PollEvent(&e)) 
        {
            if (e.type == SDL_QUIT) isRunning = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        map->render();
        SDL_RenderPresent(renderer); 
        SDL_Delay(16);
    }
}

void Game::ShutDown() {
     if (map) {
        delete map;
        map = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}