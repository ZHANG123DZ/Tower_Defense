#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <ui/Button.hpp>
#include <tower/TowerManager.hpp>
#include <tower/Tower.hpp>
#include <tower/ArrowManager.hpp>
#include <core/Map.hpp>
#include <enemy/EnemyManager.hpp>
#include <core/GameState.hpp>
#include <ui/Modal.hpp>
#include <vector>

class Battle : public Page {
private:
    Modal* endGameModal = nullptr;
    GameState* gameState = nullptr;
    Route& route;
    SDL_Texture* texture = nullptr;
    SDL_Rect* rect = nullptr;
    Button* backButton = nullptr;
    TowerManager* towerManager = nullptr;
    ArrowManager* arrowManager = nullptr;  
    SDL_Texture* arrowTexture = nullptr;
    Map* map = nullptr;
    EnemyManager* enemyManager = nullptr;
     TTF_Font* font = nullptr;
    int level;

public:
    Battle(Route& route, int level);
    ~Battle();

    void render(SDL_Renderer* renderer) override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
};
