#include <enemy/Enemy.hpp>
#include <cmath>
#include <iostream>
Enemy::Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path, float hp, float speed)
    : renderer(renderer), path(path), maxHP(hp), currentHP(hp), speed(speed), currentPathIndex(0),
      texture(nullptr)
{
    if (path.empty()) {
        std::cerr << "Error: Enemy path is empty!" << std::endl;
        return;
    }
    currentPosition = path[0];
    rect.w = SIZE;
    rect.h = SIZE;
    rect.x = currentPosition.x - SIZE / 2;
    rect.y = currentPosition.y - SIZE / 2;
}
Enemy::~Enemy() {}
void Enemy::setTexture(SDL_Texture* tex) {
    texture = tex;
}
void Enemy::update(float deltaTime) {
    if (isDead() || reachedEnd()) return;
    moveToNextWaypoint(deltaTime);
}
void Enemy::moveToNextWaypoint(float deltaTime) {
    if (currentPathIndex >= path.size() - 1) {
        currentPathIndex = path.size();
        return;
    }
    SDL_Point target = path[currentPathIndex + 1];
    float dx = target.x - currentPosition.x;
    float dy = target.y - currentPosition.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance < 5.0f) {
        currentPathIndex++;
        return;
    }
    float moveDistance = speed * deltaTime;
    if (moveDistance > distance) moveDistance = distance;
    currentPosition.x += static_cast<int>((dx / distance) * moveDistance);
    currentPosition.y += static_cast<int>((dy / distance) * moveDistance);
    rect.x = currentPosition.x - SIZE / 2;
    rect.y = currentPosition.y - SIZE / 2;
}

void Enemy::takeDamage(float damage) {
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
}
void Enemy::render() {
    if (!texture) return;
    // Vẽ quái
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    // Không vẽ thanh máu nếu quái chết
    if (currentHP <= 0) return;
    //  Vẽ thanh máu ở trên đầu quái
    int barHeight = 6;         // Chiều cao thanh máu
    int barOffset = 8;         // Khoảng cách phía trên đầu quái
    int barWidth = rect.w;     // Độ rộng thanh máu

    // Nền (màu đỏ)
    SDL_Rect hpBarBack = {
        rect.x,
        rect.y - barOffset,
        barWidth,
        barHeight
    };

    // Lượng máu còn lại (màu xanh)
    float hpPercent = currentHP / maxHP;
    if (hpPercent < 0) hpPercent = 0;
    if (hpPercent > 1) hpPercent = 1;

    SDL_Rect hpBarFront = {
        rect.x,
        rect.y - barOffset,
        static_cast<int>(barWidth * hpPercent),
        barHeight
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpBarBack);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hpBarFront);
}
