#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <enemy/Enemy.hpp>
#include <string>

class Arrow {
private:
    static Mix_Chunk* hitSound;
    SDL_Texture* texture;
    SDL_Point position;
    SDL_Point direction;
    float speed = 30.0f;
    bool active = true;
    Enemy* target;
    const int size = 32;
    float damage = 50.0f; 
public:
    Arrow(SDL_Texture* texture, SDL_Point startPos, Enemy* target, float damage);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    bool isActive() const { return active; }

    //Âm thanh bắn tên
    static void loadHitSound(const std::string& path);
    static void freeSound();
};
