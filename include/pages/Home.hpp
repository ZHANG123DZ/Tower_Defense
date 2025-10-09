#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <ui/Button.hpp>

class Home: public Page {
private:
    Route& route; 
    SDL_Texture* texture;
    SDL_Rect* rect;
    Button* startButton;
    Button* profileButton;
public:
    Home(Route& route);
    ~Home();
    void render(SDL_Renderer* renderer) override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
};