#include <pages/Battlefields.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <pages/Home.hpp>

Battlefields::~Battlefields() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

Battlefields::Battlefields(Route& route) : route(route) {
    // Load font
    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // Tạo nút Start
    startButton = new Button(route.getRenderer(), 200, 200, 200, 60, "Bắt đầu", font);

    ButtonStyleConfig startBtnStyle;
    startBtnStyle.bgColor = {0, 0, 255, 255};  
    startBtnStyle.textColor = {255, 255, 255, 255}; 
    startBtnStyle.borderColor = {0, 0, 0, 255}; 
    startBtnStyle.borderRadius = 10;
    startBtnStyle.text = "Quay lại";
    startBtnStyle.font = font;

    startButton->applyStyle(startBtnStyle);
    startButton->setOnClick([&]() {
        route.setPage(new Home(route));
    });

    // Load background
    SDL_Surface* surface = IMG_Load("../assets/home/home.jpg"); 
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
    SDL_FreeSurface(surface); 
}

void Battlefields::handleEvent(SDL_Event& e) {
    startButton->handleEvent(e);
}

void Battlefields::render(SDL_Renderer* renderer) {
    // Vẽ background
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Vẽ nút
    startButton->render();

    SDL_RenderPresent(renderer);
}

void Battlefields::update() {
}