#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ui/Button.hpp>
#include <vector>
#include <string>

class TowerManager {
private:
    SDL_Renderer* renderer;
    std::vector<Button*> towerButtons;
    SDL_Texture* frameTexture = nullptr;
    int selectedTowerIndex = -1;
    TTF_Font* font;

public:
    TowerManager(SDL_Renderer* renderer, TTF_Font* font);
    ~TowerManager();

    void loadTowers(const std::vector<std::string>& towerImages);
    void setFrameTexture(SDL_Texture* texture);

    void handleEvent(SDL_Event& e);
    void render();

    int getSelectedTower() const;
};
