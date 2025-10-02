#include <iostream>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <core/Game.hpp>
#include <core/Map.hpp>

Map *map =nullptr;

std::vector<std::vector<int>> level = {
    {2,2,2,2,2,1,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2}
};


Game::Game() : isRunning(false) {}

Game::~Game() {
    ShutDown();
}

bool Game::Initialize(const char* title, int width, int height, bool fullScreen = false) {
    if ((SDL_Init(SDL_INIT_VIDEO) || SDL_Init(SDL_INIT_AUDIO) || SDL_Init(SDL_INIT_TIMER) ) < 0) {
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer error: " << Mix_GetError() << std::endl;
        return 1;
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

    TTF_Font* font = TTF_OpenFont("../assets/fonts/ArchivoBlack-Regular.ttf", 24);
    setFont(font);

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/a.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return 1;
    }
    Mix_PlayMusic(bgMusic, -1);
    //Muốn thay map nào thì để đường dẫn ảnh vào đây
    // map = new Map(renderer, "../assets/1 Tiles/battle.png", 32*4, 32*4, 12);
    // map->loadMap(level);

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
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                // map->handleClick(mouseX, mouseY);
            }
        } 
        SDL_Color color = {255, 255, 255}; // white
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_Rect dest = {100, 100, 300, 50};
        SDL_SetRenderDrawColor(renderer, 10, 12, 40, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        
        // map->render();
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
    if (font) {
        TTF_CloseFont(font);
    }
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
    }
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::setFont(TTF_Font* currentFont) {
    font = currentFont;
}

void Game::setBgMusic(Mix_Music* music) {
    bgMusic = music;
}
