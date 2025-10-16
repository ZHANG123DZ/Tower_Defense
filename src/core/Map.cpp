#include <core/Map.hpp>
#include <iostream>
#include <memory>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <tile/Tile.hpp>
#include <tile/PathTile.hpp>
#include <tile/TowerSpotTile.hpp>
#include <tile/WallTile.hpp>

Map::Map(SDL_Renderer* renderer, const std::string& tilesetPath, int tileWidth, int tileHeight, int tilesPerRow):renderer(renderer), tileWidth(tileWidth), tileHeight(tileHeight), tilesPerRow(tilesPerRow) {
    SDL_Surface* surface = IMG_Load(tilesetPath.c_str());
    if (!surface) {
        std::cerr << "Failed to load tileset: " << IMG_GetError() << std::endl;
        return;
    }
    tileSet = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tileSet) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
}

Map::~Map() {
    if (tileSet) {
        SDL_DestroyTexture(tileSet);
    }
}

Tile* Map::getTileAt(int x, int y) {
    int col = x / tileWidth;
    int row = y / tileHeight;
    if (row < 0 || row >= rows || col < 0 || col >= cols) return nullptr;
    if (!tiles[row][col]) return nullptr;
    return tiles[row][col].get();
}

void Map::loadMap(const std::vector<std::vector<int>>& data) {
    mapData = data;
    rows = (int)mapData.size();
    cols = (int)mapData[0].size();

    int index, tileId;

    for (int row = 0; row <rows; row++) {
        std::vector<std::unique_ptr<Tile>> rowTiles;
        for (int col=0; col<cols; col++) {
            index = row * cols + col;
            tileId = mapData[row][col];
            if (tileId < 0) {
                rowTiles.push_back(nullptr);
                continue;
            }

            SDL_Rect src = getSrcRect(index);
            SDL_Rect dest = getDestRect(row, col);

            std::unique_ptr<Tile> tile;
            if (tileId == 0) {
                tile = std::make_unique<PathTile>(tileId, src, dest);
            } else if (tileId == 1) {
                tile = std::make_unique<TowerSpotTile>(tileId, src, dest);
            } else if (tileId == 2) {
                tile = std::make_unique<WallTile>(tileId, src, dest);
            }
            
            rowTiles.push_back(std::move(tile));
        }
        tiles.push_back(std::move(rowTiles));
    }
}

void Map::render() {
    if (!tileSet) return;
    for (auto& row: tiles) {
        for (auto& tile: row) {
            tile->render(renderer, tileSet);
        }
    }
}

SDL_Rect Map::getSrcRect(int tileIndex) {
    SDL_Rect src;
    src.w = tileWidth;
    src.h = tileHeight;
    src.x = (tileIndex % tilesPerRow) * tileWidth;
    src.y = (tileIndex / tilesPerRow) * tileHeight;
    return src;
}

SDL_Rect Map::getDestRect(int row, int col) {
    SDL_Rect dest;
    dest.x = col * tileWidth;
    dest.y = row * tileHeight;
    dest.w = tileWidth;
    dest.h = tileHeight;
    return dest;
}

void Map::handleClick(int mouseX, int mouseY) {
    int col = (mouseX) / tileWidth;
    int row = (mouseY) / tileHeight;

    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        std::unique_ptr<Tile>& tile = tiles[row][col];
        if (tile) {
            tile->onEnter();
        }
    }
}