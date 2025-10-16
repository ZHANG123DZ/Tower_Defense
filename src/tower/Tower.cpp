#include <tower/Tower.hpp>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <math.h>
#include <enemy/Enemy.hpp>

Tower::Tower(SDL_Renderer* renderer, const std::string& imagePath, int x, int y, int w, int h) {
    texture = IMG_LoadTexture(renderer, imagePath.c_str());
    if (!texture) {
        std::cerr << "IMG_LoadTexture error: " << IMG_GetError() << std::endl;
    }
    rect = {x, y, w, h};
}

Tower::~Tower() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Tower::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}


SDL_Point Tower::getCenter() const {
    return { rect.x + rect.w / 2, rect.y + rect.h / 2 };
}

bool Tower::isEnemyInRange(Enemy* enemy) const {
    SDL_Point enemyPos = enemy->getPosition();
    SDL_Point center = getCenter();
    float dx = center.x - enemyPos.x;
    float dy = center.y - enemyPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    return dist <= range;
}

void Tower::update(float deltaTime, std::vector<Enemy*>& enemies, ArrowManager* arrowManager) {
    timeSinceLastShot += deltaTime;

    if (timeSinceLastShot >= cooldown) {
        for (auto* enemy : enemies) {
            if (!enemy->isDead() && isEnemyInRange(enemy)) {
                SDL_Point startPos = getCenter();
                arrowManager->spawnArrow(startPos, enemy);
                timeSinceLastShot = 0.0f;
                break;
            }
        }
    }
}
