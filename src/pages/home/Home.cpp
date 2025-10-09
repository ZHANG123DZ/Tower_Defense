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

    // Tạo nút Start
    startButton = new Button(route.getRenderer(), 200, 200, 200, 60, "Bắt đầu", font);

    ButtonStyleConfig startBtnStyle;
    startBtnStyle.bgColor = {0, 0, 255, 255};       // Blue
    startBtnStyle.textColor = {255, 255, 255, 255}; // White
    startBtnStyle.borderColor = {0, 0, 0, 255}; 
    startBtnStyle.borderRadius = 10;
    startBtnStyle.text = "Bắt đầu";
    startBtnStyle.font = font;

    startButton->applyStyle(startBtnStyle);
    startButton->setOnClick([&]() {
        route.setPage(new Battlefields(route));
    });

    // Load background
    SDL_Surface* surface = IMG_Load("../assets/home/home2.png"); 
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
    SDL_FreeSurface(surface); 
}

void Home::handleEvent(SDL_Event& e) {
    startButton->handleEvent(e);
}

void Home::render(SDL_Renderer* renderer) {
    // Vẽ background
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Vẽ nút
    startButton->render();

    SDL_RenderPresent(renderer);
}

void Home::update() {
}