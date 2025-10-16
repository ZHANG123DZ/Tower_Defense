#include <pages/Home.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <pages/Battlefields.hpp>

Home::~Home() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

Home::Home(Route& route) : route(route) {
    // Load font
    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // Load background
    SDL_Surface* surface = IMG_Load("../assets/home/home2.jpg"); 
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
    SDL_FreeSurface(surface); 

    // Load nhạc
    Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/a.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(bgMusic, -1);

    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};

    // Tạo nút Start
    startButton = new Button(route.getRenderer(), 370, 660, 280, 90, "", font);
    quitButton = new Button(route.getRenderer(), 370, 780, 280, 90, "", font);    

    SDL_Rect startSrc = {370, 660, 280, 90};
    SDL_Rect quitSrc  = {370, 780, 280, 90};

    // Gắn các thuộc tính cho nút start
    startButton->applyStyle(styleBtn);
    startButton->setBackgroundTexture(texture, startSrc);
    startButton->setOnClick([&]() {
        route.setPage(new Battlefields(route));
    });
    // Gắn các thuộc tính cho nút quit
    quitButton->applyStyle(styleBtn);
    quitButton->setBackgroundTexture(texture, quitSrc);
    quitButton->setOnClick([&]() {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    });
}

void Home::handleEvent(SDL_Event& e) {
    startButton->handleEvent(e);
    quitButton->handleEvent(e);
}

void Home::render(SDL_Renderer* renderer) {
    // Vẽ background
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Vẽ nút
    startButton->render();
    quitButton->render();
    SDL_RenderPresent(renderer);
}

void Home::update() {
}