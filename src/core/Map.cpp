#include <core/Map.hpp>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

Map::Map(SDL_Renderer* renderer, const std::string& tilesetPath, float tileWidth, float tileHeight, int tilesPerRow):renderer(renderer), tileWidth(tileWidth), tileHeight(tileHeight), tilesPerRow(tilesPerRow) {
    SDL_Surface* surface = IMG_Load(tilesetPath.c_str());
    if (!surface) {
        std::cerr << "Failed to load tileset: " << IMG_GetError() << std::endl;
    }
    tileSet = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tileSet) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

Map::~Map() {
    if (tileSet) {
        SDL_DestroyTexture(tileSet);
    }
}

void Map::loadMap(const std::vector<std::vector<int>>& data) {
    mapData = data;
    rows = (int)mapData.size();
    cols = (int)mapData[0].size();
}

void Map::render() {
    for (int row = 0; row <rows; row++) {
        for (int col=0; col<cols; col++) {
            int tileIndex = mapData[row][col];
            if (tileIndex < 0) continue;

            SDL_Rect src = getSrcRect(tileIndex);
            SDL_Rect dest = getDestRect(row, col);
            SDL_RenderCopy(renderer, tileSet, &src, &dest);
        }
    }
}

SDL_Rect Map::getSrcRect(int tileIndex) {
    SDL_Rect src;
    src.w = tileWidth;
    src.h = tileHeight;
    src.x = (tileIndex % tilesPerRow)*tileWidth;
    src.y = (tileIndex / tilesPerRow)*tileHeight;
    return src;
}

SDL_Rect Map::getDestRect(int row, int col) {
    SDL_Rect dest;
    dest.x = col * tileWidth;
    dest.y = row*tileHeight;
    dest.w = tileWidth;
    dest.h = tileHeight;
    return dest;
}