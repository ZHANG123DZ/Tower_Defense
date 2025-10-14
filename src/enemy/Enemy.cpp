#include <enemy/Enemy.hpp>
#include <cmath>
#include <iostream>
Enemy::Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path,float hp, float speed): renderer(renderer), path(path), maxHP(hp), currentHP(hp), speed(speed), currentPathIndex(0) {
    if (path.empty()) {
        std::cerr << "Error: Enemy path is empty!" << std::endl; // Không thể tạo quái nếu không có đường đi
        return;
    }
    currentPosition = path[0];
}
Enemy::~Enemy() {
}
void Enemy::update(float deltaTime) {
    if (isDead() || reachedEnd()) return;
    moveToNextWaypoint(deltaTime);
}

void Enemy::moveToNextWaypoint(float deltaTime) {
    if (currentPathIndex >= path.size() - 1) { // Đã đến cuối đường
        currentPathIndex = path.size(); // Đánh dấu đã đến cuối
        return;
    }
    
    SDL_Point target = path[currentPathIndex + 1];
    
    // Tính hướng di chuyển
    float dx = target.x - currentPosition.x;
    float dy = target.y - currentPosition.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    // Nếu đã đến waypoint
    if (distance < 5.0f) {
        currentPathIndex++;
        return;
    }
    
    // Di chuyển về phía target
    float moveDistance = speed * deltaTime;
    //moveDistance = Quãng đường quái có thể di chuyển trong 1 frame (dựa vào tốc độ và thời gian).
    if (moveDistance > distance) {
        moveDistance = distance;
    }
    
    currentPosition.x += (int)((dx / distance) * moveDistance);
    currentPosition.y += (int)((dy / distance) * moveDistance);
}

void Enemy::render() {
    if (isDead()) return;
    
    SDL_Rect bodyRect = {
        currentPosition.x - SIZE / 2,
        currentPosition.y - SIZE / 2,
        SIZE,
        SIZE
    };
    
    // Màu đỏ
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bodyRect);
    
    // Viền đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bodyRect);
    
    int barWidth = SIZE;
    int barHeight = 3;
    float healthPercent = currentHP / maxHP;
    
    // Background (đỏ đậm)
    SDL_Rect bgRect = {
        currentPosition.x - barWidth / 2,
        currentPosition.y - SIZE / 2 - 6,
        barWidth,
        barHeight
    };
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Foreground (xanh lá)
    SDL_Rect fgRect = {
        currentPosition.x - barWidth / 2,
        currentPosition.y - SIZE / 2 - 6,
        (int)(barWidth * healthPercent),
        barHeight
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &fgRect);
}
void Enemy::takeDamage(float damage) {
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
}
