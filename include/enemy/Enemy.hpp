#pragma once
#include <SDL2/SDL.h>
#include <vector>
class Enemy {
public:
    Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path, 
          float hp = 100.0f, float speed = 50.0f); // 
    ~Enemy();
    void update(float deltaTime); // Cập nhật vị trí quái
    void render(); // Vẽ quái
    void takeDamage(float damage); // Quái bị trừ máu
    // Getters
    bool isDead() const { return currentHP <= 0; }
    bool reachedEnd() const { return currentPathIndex >= path.size(); }
    SDL_Point getPosition() const { return currentPosition; }
    float getCurrentHP() const { return currentHP; }
    float getMaxHP() const { return maxHP; }
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    float maxHP;
    SDL_Rect rect;
    float currentHP;
    float speed;
    std::vector<SDL_Point> path; // Đường đi của quái
    size_t currentPathIndex; // Chỉ số waypoint hiện tại trong path
    SDL_Point currentPosition; // Vị trí hiện tại của quái
    static const int SIZE = 20; // Kích thước vẽ quái
    void moveToNextWaypoint(float deltaTime);
};
