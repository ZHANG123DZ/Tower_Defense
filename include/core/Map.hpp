#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <tile/Tile.hpp>

class Map {
public:
    Map(SDL_Renderer* renderer, const std::string& tilesetPath, int tileWidth, int tileHeight, int tilesPerRow);
    ~Map();
    friend class Tile;
    void loadMap(const std::vector<std::vector<int>>& mapData);
    void render();
    void handleClick(int mouseX, int mouseY);
    Tile* getTileAt(int x, int y);
private:
    SDL_Renderer* renderer;
    SDL_Texture* tileSet;

    int cols, rows, tilesPerRow;
    int tileWidth, tileHeight;
    std::vector<std::vector<int>> mapData;
    std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
    SDL_Rect getSrcRect(int tileIndex);
    SDL_Rect getDestRect(int row, int col);
};