#pragma once
#include <iostream>
#include <SDL2/SDL.h>

class Tile {
protected:
    int id;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    bool hasTower = false;
public:
    Tile(int id, SDL_Rect src, SDL_Rect dest) : id(id), srcRect(src), destRect(dest) {};
    virtual ~Tile() = default;
    virtual void render(SDL_Renderer* renderer, SDL_Texture* tileSet) = 0;

    virtual void onLeave() {}

    int getId() const {return id;}
    SDL_Rect getSrcRect() const {return srcRect;}
    SDL_Rect getDestRect() const {return destRect;}

    bool getHasTower() const { return hasTower; }
    void setHasTower(bool value) { hasTower = value; }
};