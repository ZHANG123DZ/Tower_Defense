#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <ui/Button.hpp>
#include <tower/TowerManager.hpp>
#include <tower/Tower.hpp>
#include <tower/ArrowManager.hpp>
#include <core/Map.hpp>
#include <enemy/EnemyManager.hpp>

class Battle: public Page {
private:
    Route& route;
    SDL_Texture* texture;
    SDL_Rect* rect;
    Button* backButton;
    TowerManager* towerManager;
    std::vector<Tower* > towers;
    std::vector<Enemy*> enemies;
    ArrowManager* arrowManager;  
    SDL_Texture* arrowTexture;
    Map* map;
    EnemyManager* enemyManager;
    int level;
public:
    Battle(Route& route, int level);
    ~Battle();
    void render(SDL_Renderer* renderer) override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
};