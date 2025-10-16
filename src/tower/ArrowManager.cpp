#include <iostream>
#include <tower/ArrowManager.hpp>
#include <enemy/Enemy.hpp>
#include <algorithm>

ArrowManager::~ArrowManager() {
    clear();
}

void ArrowManager::setArrowTexture(SDL_Texture* tex) {
    arrowTexture = tex;
}

void ArrowManager::spawnArrow(SDL_Point startPos, Enemy* target) {
    if (arrowTexture && target) {
        Arrow* newArrow = new Arrow(arrowTexture, startPos, target);
        arrows.push_back(newArrow);
    }
}

void ArrowManager::update(float deltaTime) {
    for (auto* arrow : arrows) {
        arrow->update(deltaTime);
    }

    arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
        [](Arrow* arrow) {
            if (!arrow->isActive()) {
                arrow->freeSound();
                delete arrow;
                return true;
            }
            return false;
        }), arrows.end());
}

void ArrowManager::render(SDL_Renderer* renderer) {
    for (auto* arrow : arrows) {
        arrow->render(renderer);
    }
}

void ArrowManager::clear() {
    for (auto* arrow : arrows) {
        delete arrow;
    }
    arrows.clear();
}