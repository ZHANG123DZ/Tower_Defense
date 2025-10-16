#pragma once
#include <SDL2/SDL.h>
#include <vector>
class Enemy {
public:
    Enemy(SDL_Renderer* renderer, const std::vector<SDL_Point>& path, 
          float hp = 100.0f, float speed = 50.0f);
    ~Enemy();
    void update(float deltaTime);
    void render();
    void takeDamage(float damage);
    // Getters
    bool isDead() const { return currentHP <= 0; }
    bool reachedEnd() const { return currentPathIndex >= path.size(); }
    SDL_Point getPosition() const { return currentPosition; }
    float getCurrentHP() const { return currentHP; }
    float getMaxHP() const { return maxHP; }
    void setTexture(SDL_Texture*tex);
    
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    float maxHP;
    SDL_Rect rect;
    float currentHP;
    float speed;
    std::vector<SDL_Point> path;
    size_t currentPathIndex;
    SDL_Point currentPosition;
    static const int SIZE = 80;
    void moveToNextWaypoint(float deltaTime);
};