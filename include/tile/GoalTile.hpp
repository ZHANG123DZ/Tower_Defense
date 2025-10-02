#pragma one
#include <tile/Tile.hpp>
#include <SDL2/SDL.h>

class GoalTile : public Tile {
public:
    GoalTile(int id, SDL_Rect src, SDL_Rect dest) : Tile(id, src, dest) {}

    void render(SDL_Renderer* renderer, SDL_Texture* tileSet) override {
        SDL_RenderCopy(renderer, tileSet, &srcRect, &destRect);
    }
};