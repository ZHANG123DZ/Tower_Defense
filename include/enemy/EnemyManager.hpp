#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <enemy/Enemy.hpp>

struct Wave {
    int enemyCount;
    float spawnInterval;
    float enemyHP;
    float enemySpeed;
};

class EnemyManager {
private:
    SDL_Renderer* renderer;
    SDL_Texture* enemyTexture; // Texture dùng chung cho các quái
    std::vector<SDL_Point> enemyPath;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Wave system
    std::vector<Wave> waves;
    int currentWaveIndex;
    bool spawning;
    float spawnTimer;
    int enemiesSpawnedInWave;
    Wave currentWave;
    int totalSpawned;
    int enemiesReachedEnd;
    int baseHP; // tổng hp của thành
public:
    EnemyManager(SDL_Renderer* renderer, const std::vector<SDL_Point>& enemyPath);
    ~EnemyManager();

    void update(float deltaTime);
    void render();

    void startWave(const Wave& wave);
    void addWave(const Wave& wave);
    bool isWaveComplete() const;
    bool allEnemiesDead() const;

    void spawnEnemy(float hp, float speed);
    void removeDeadEnemies();

    int getAliveEnemyCount() const;
    int getTotalSpawnedCount() const { return totalSpawned; }
    int getCurrentWaveNumber() const { return currentWaveIndex + 1; }
    bool isSpawning() const { return spawning; }
    int getbaseHP() const;
    bool isGameOver() const;
    void damageEnemyAt(SDL_Point position, float damage, float radius = 30.0f);
    int getenemiesReachedEnd() const;
    void setEnemyTexture(SDL_Texture* tex);
};
