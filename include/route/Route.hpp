#pragma once

#include <SDL2/SDL.h>
#include <iostream>

#include <pages/Page.hpp>

class Route {
private:
    SDL_Renderer* renderer;
    Page* currentPage;
public:
    Route(SDL_Renderer* renderer);
    ~Route();
    void handleEvent(SDL_Event& e);
    void render();
    void update();
    void setPage(Page* page);
    SDL_Renderer* getRenderer() const { return renderer; }
};
