#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Enemy {
public:
    // Constructor: truyền renderer, đường đi, máu, tốc độ
    Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path,
          float hp = 100.0f, float speed = 50.0f);

    ~Enemy();

    void update(float deltaTime);      // Cập nhật vị trí và trạng thái
    void render();                     // Vẽ enemy lên màn hình
    void takeDamage(float damage);     // Giảm máu

    // Getters
    bool isDead() const { return currentHP <= 0; }
    bool reachedEnd() const { return currentPathIndex >= path.size() - 1; }
    SDL_Point getPosition() const { return currentPosition; }
    float getCurrentHP() const { return currentHP; }
    float getMaxHP() const { return maxHP; }

    void setTexture(SDL_Texture* tex);
    void onReachBase() { /* Có thể thêm hiệu ứng hoặc logic riêng */ }

private:
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    float maxHP = 100.0f;
    float currentHP = 100.0f;
    float speed = 50.0f;

    std::vector<SDL_Point> path;  // Danh sách các điểm trên đường đi
    size_t currentPathIndex = 0;  // Chỉ số điểm hiện tại trong path
    SDL_Point currentPosition{};  // Vị trí hiện tại của quái

    SDL_Rect rect{};              // Vùng vẽ của enemy

    static const int SIZE = 80;   // Kích thước enemy (width & height)

    void moveToNextWaypoint(float deltaTime); // Di chuyển theo path
};
