#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <enemy/EnemyManager.hpp>
#include <core/Map.hpp>
class Game{
public: 
    Game();
    ~Game();

    bool Initialize(const char* title, int width, int height, bool fullScreen);
    void Run();
    void ShutDown();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Mix_Music* bgMusic;
    bool isRunning;
    int screenWidth;
    int screenHeight;
    
    // Game objects
    Map* map;
    EnemyManager* enemyManager;
    
    // Game state
    int currentWaveIndex;
    bool waitingForNextWave;
    
    // Helper methods
    void handleEvents();
    void update(float deltaTime);
    void render();
    void setupWaves();
    void startNextWave();
};