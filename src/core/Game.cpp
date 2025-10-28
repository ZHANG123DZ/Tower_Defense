#include <iostream>
#include <string.h>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <route/Route.hpp>
#include <core/Game.hpp>
#include <core/Map.hpp>
#include <ui/Modal.hpp>
#include <ui/Button.hpp>
#include <pages/Home.hpp>
#include <pages/Page.hpp>

Game::Game() : isRunning(false), gameRoute(nullptr) {}

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
    SDL_Surface* icon = IMG_Load("../assets/icon/icon.png");
    if (!icon) {
        std::cerr << "Failed to load icon: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    //Tạo ra bút vẽ (renderer): dùng để vẽ hình ảnh, text, màu nền, texture,… lên cửa sổ.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Quit();
        return false;
    }

    // SetCursor
    gameRoute = new Route(renderer);
    if (gameRoute) {
        gameRoute->SetCursor("../assets/cursors/default_cursor.png", 0, 0);
    }
    
    // Load font chữ
    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    setFont(font);

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    gameRoute = new Route(renderer);
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
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("Click tại: (%d, %d)\n", x, y);
            }
            if (gameRoute) gameRoute->handleEvent(e);
        } 
        if (gameRoute) gameRoute->update();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        if (gameRoute) gameRoute->render();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Game::ShutDown() {
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
