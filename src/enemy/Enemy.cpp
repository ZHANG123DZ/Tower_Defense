#include <enemy/Enemy.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

Enemy::Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path, float hp, float speed)
    : renderer(renderer), path(path), maxHP(hp), currentHP(hp), speed(speed),
      currentPathIndex(0), texture(nullptr)
{
    if (path.empty()) {
        std::cerr << "[Enemy] ❌ ERROR: Path is empty!" << std::endl;
        return;
    }

    // Khởi tạo vị trí ban đầu tại điểm đầu tiên của path
    currentPosition = path[0];
    rect = { currentPosition.x - SIZE / 2, currentPosition.y - SIZE / 2, SIZE, SIZE };

    // In log spawn
    std::cout << "[Enemy] ✅ Spawned | Pos=(" << currentPosition.x << "," << currentPosition.y
              << ") | HP=" << hp << " | Speed=" << speed << std::endl;
}

Enemy::~Enemy() {}

void Enemy::setTexture(SDL_Texture* tex) {
    texture = tex;
    if (texture)
        std::cout << "[Enemy] Texture assigned successfully." << std::endl;
    else
        std::cerr << "[Enemy] ⚠️ Texture is NULL!" << std::endl;
}

void Enemy::update(float deltaTime) {
    // Nếu chết hoặc đến đích thì không di chuyển nữa
    if (isDead()) {
        std::cout << "[Enemy] 💀 Dead, skipping update." << std::endl;
        return;
    }
    if (reachedEnd()) {
        std::cout << "[Enemy] 🏁 Reached end of path." << std::endl;
        return;
    }

    moveToNextWaypoint(deltaTime);
}

void Enemy::moveToNextWaypoint(float deltaTime) {
    if (path.empty() || currentPathIndex >= path.size() - 1) return;

    SDL_Point target = path[currentPathIndex + 1];

    float dx = static_cast<float>(target.x - currentPosition.x);
    float dy = static_cast<float>(target.y - currentPosition.y);
    float distance = std::sqrt(dx * dx + dy * dy);

    // Nếu tới gần điểm tiếp theo, chuyển sang waypoint tiếp theo
    if (distance < 1.0f) {
        currentPathIndex++;
        if (currentPathIndex >= path.size() - 1) {
            std::cout << "[Enemy] 🏁 Arrived at final waypoint." << std::endl;
            return;
        }
        target = path[currentPathIndex + 1];
        dx = static_cast<float>(target.x - currentPosition.x);
        dy = static_cast<float>(target.y - currentPosition.y);
        distance = std::sqrt(dx * dx + dy * dy);
    }

    float moveDistance = speed * deltaTime;
    if (moveDistance > distance) moveDistance = distance;

    // ⚙️ Tính vị trí mới với float, rồi làm tròn sang int để tránh bị đứng yên
    float newX = currentPosition.x + (dx / distance) * moveDistance;
    float newY = currentPosition.y + (dy / distance) * moveDistance;

    currentPosition.x = static_cast<int>(newX);
    currentPosition.y = static_cast<int>(newY);

    rect.x = currentPosition.x - SIZE / 2;
    rect.y = currentPosition.y - SIZE / 2;

    std::cout << "[Enemy] 🚶 Moving | Index=" << currentPathIndex
              << " | Pos=(" << currentPosition.x << "," << currentPosition.y << ")"
              << " | Target=(" << target.x << "," << target.y << ")"
              << " | d=" << distance << " | dt=" << deltaTime << std::endl;
}

void Enemy::takeDamage(float damage) {
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;

    std::cout << "[Enemy] 💥 Took damage: -" << damage
              << " | HP: " << currentHP << "/" << maxHP << std::endl;
}

void Enemy::render() {
    if (!texture) {
        std::cerr << "[Enemy] ⚠️ Texture missing, cannot render." << std::endl;
        return;
    }

    // Vẽ enemy
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    // Nếu chết thì không vẽ thanh máu
    if (currentHP <= 0) return;

    // ---- Thanh máu ----
    int barHeight = 6;
    int barOffset = 10;
    int barWidth = rect.w;

    SDL_Rect hpBarBack = {
        rect.x,
        rect.y - barOffset,
        barWidth,
        barHeight
    };

    float hpPercent = std::clamp(currentHP / maxHP, 0.0f, 1.0f);
    SDL_Rect hpBarFront = {
        rect.x,
        rect.y - barOffset,
        static_cast<int>(barWidth * hpPercent),
        barHeight
    };

    // Nền đỏ
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpBarBack);

    // Phần còn lại xanh
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hpBarFront);
}
