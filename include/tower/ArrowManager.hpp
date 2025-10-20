#pragma once
#include <vector>
#include <tower/Arrow.hpp>
#include <enemy/Enemy.hpp>

class ArrowManager {
private:
    std::vector<Arrow*> arrows;
    SDL_Texture* arrowTexture = nullptr;

public:
    ArrowManager() = default;
    ~ArrowManager();

    void setArrowTexture(SDL_Texture* tex);
    void spawnArrow(SDL_Point startPos, Enemy* target, float damage);
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void clear();
};
