#pragma once

#include <iostream>
#include <vector>
#include <string.h>
#include <tower/Arrow.hpp>
#include <tower/ArrowManager.hpp>

class Tower {
private:
    int price;
    SDL_Texture* texture;
    SDL_Rect rect;
    float damage = 50.0f;
    float range = 200.0f;
    float cooldown = 1.0f;
    float timeSinceLastShot = 0.0f;
    SDL_Texture* arrowTexture;

public:
    Tower(SDL_Renderer* renderer, const std::string& imagePath, float damage,
          int x, int y, int w, int h);
    ~Tower();
    void render(SDL_Renderer* renderer);
    void setArrowTexture(SDL_Texture* tex) { arrowTexture = tex; }
    void update(float deltaTime, std::vector<Enemy*>& enemies, ArrowManager* arrowManager);
    bool isEnemyInRange(Enemy* enemy) const;
    SDL_Point getCenter() const;

    void setPrice(int price);
    int getPrice();
};
