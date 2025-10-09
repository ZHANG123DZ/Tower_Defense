#pragma once

#include <SDL2/SDL.h>

class Page {
public:
    virtual ~Page() {}
    virtual void handleEvent(SDL_Event& e) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
};