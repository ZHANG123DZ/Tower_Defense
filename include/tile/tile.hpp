#pragma once
#include <iostream>
#include <SDL2/SDL.h>

class Tile {
protected:
    int id;
    SDL_Rect srcRect;
    SDL_Rect destRect;
public:
    Tile(int id, SDL_Rect src, SDL_Rect dest) : id(id), srcRect(src), destRect(dest) {};
    virtual ~Tile() = default;
    virtual void render(SDL_Renderer* renderer, SDL_Texture* tileSet) = 0;
    
    virtual void onEnter() {std::cout<< id << "\n";}
    virtual void onLeave() {}

    int getId() const {return id;}
    SDL_Rect getSrcRect() const {return srcRect;}
    SDL_Rect getDestRect() const {return destRect;}
};