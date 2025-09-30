#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <tile/Tile.hpp>

class Map {
public:
    Map(SDL_Renderer* renderer, const std::string& tilesetPath, float tileWidth, float tileHeight, int tilesPerRow);
    ~Map();

    void loadMap(const std::vector<std::vector<int>>& mapData);
    void render();
private:
    SDL_Renderer* renderer;
    SDL_Texture* tileSet;

    int cols, rows, tilesPerRow;
    float tileWidth, tileHeight;
    std::vector<std::vector<int>> mapData;
    SDL_Rect getSrcRect(int tileIndex);
    SDL_Rect getDestRect(int row, int col);
};