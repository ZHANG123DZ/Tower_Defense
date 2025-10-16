#include <tower/Arrow.hpp>
#include <cmath>
#include <numeric>
#include <string>
#include <iostream>

Mix_Chunk* Arrow::hitSound = nullptr;

Arrow::Arrow(SDL_Texture* texture, SDL_Point startPos, Enemy* target)
    : texture(texture), position(startPos), target(target) {

    if (target) {
        SDL_Point enemyPos = target->getPosition();
        int dx = enemyPos.x - position.x;
        int dy = enemyPos.y - position.y;

        int gcd = std::gcd(std::abs(dx), std::abs(dy));

        if (gcd != 0) {
            direction = { dx / gcd, dy / gcd };
        } else {
            direction = { 0, 0 };
        }
    }
}

void Arrow::update(float deltaTime) {
    if (!target || target->isDead()) {
        active = false;
        return;
    }

    // Di chuyển arrow theo hướng
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    // Tính khoảng cách giữa arrow và enemy
    SDL_Point enemyPos = target->getPosition();
    float dx = static_cast<float>(enemyPos.x) - position.x;
    float dy = static_cast<float>(enemyPos.y) - position.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 20.0f) { // Khoảng cách va chạm
        if (hitSound) {
            Mix_PlayChannel(-1, hitSound, 0);
        }
        target->takeDamage(50.0f);
        active = false;
    }
}

void Arrow::render(SDL_Renderer* renderer) {
    SDL_Rect dst = {
        static_cast<int>(position.x - size / 2),
        static_cast<int>(position.y - size / 2),
        size, size
    };

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void Arrow::loadHitSound(const std::string& path) {
    hitSound = Mix_LoadWAV("../assets/music/arrow_hit.mp3");
    if (!hitSound) {
        std::cerr << "Failed to load arrow hit sound: " << Mix_GetError() << std::endl;
    }
}

void Arrow::freeSound() {
    if (hitSound) {
        Mix_FreeChunk(hitSound);
        hitSound = nullptr;
    }
}