#include <enemy/EnemyManager.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

EnemyManager::EnemyManager(SDL_Renderer* renderer, const std::vector<SDL_Point>& enemyPath)
    : renderer(renderer),
      enemyPath(enemyPath),
      currentWaveIndex(-1),
      spawning(false),
      spawnTimer(0.0f),
      enemiesSpawnedInWave(0),
      totalSpawned(0),
      enemiesReachedEnd(0) {}

EnemyManager::~EnemyManager() {
    enemies.clear();
}

std::vector<Enemy*> EnemyManager::getEnemies() {
    std::vector<Enemy*> result;
    for (auto& e : enemies) {
        if (!e->isDead()) result.push_back(e.get());
    }
    return result;
}

void EnemyManager::addWave(const Wave& wave) {
    waves.push_back(wave);
}

void EnemyManager::startWave(const Wave& wave) {
    currentWave = wave;
    currentWaveIndex++;
    spawning = true;
    spawnTimer = 0.0f;
    enemiesSpawnedInWave = 0;
    std::cout << "Wave " << currentWaveIndex + 1 << " started! Enemies: " << wave.enemyCount << std::endl;
}

bool EnemyManager::isWaveComplete() const {
    return !spawning && allEnemiesDead();
}

bool EnemyManager::allEnemiesDead() const {
    for (const auto& e : enemies)
        if (!e->isDead()) return false;
    return true;
}

void EnemyManager::spawnEnemy(float hp, float speed) {
    auto enemy = std::make_unique<Enemy>(renderer, enemyPath, hp, speed);
    if (enemyTexture) { 
        enemy->setTexture(enemyTexture);  //Gán texture cho quái
    }
    enemies.push_back(std::move(enemy));
    totalSpawned++;
}


void EnemyManager::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e) {
                return e->isDead() || e->reachedEnd();
            }),
        enemies.end()
    );
}

void EnemyManager::update(float deltaTime) {
    if (spawning) {
        spawnTimer += deltaTime;
        if (enemiesSpawnedInWave < currentWave.enemyCount && spawnTimer >= currentWave.spawnInterval) {
            spawnEnemy(currentWave.enemyHP, currentWave.enemySpeed);
            enemiesSpawnedInWave++;
            spawnTimer = 0.0f;
        }

        if (enemiesSpawnedInWave >= currentWave.enemyCount) {
            spawning = false;
        }
    }

    for (auto& e : enemies) {
        e->update(deltaTime);
    }

    removeDeadEnemies();
}

void EnemyManager::render() {
    for (auto& e : enemies) {
        e->render();
    }
}

int EnemyManager::getAliveEnemyCount() const {
    int count = 0;
    for (const auto& e : enemies)
        if (!e->isDead()) count++;
    return count;
}

void EnemyManager::damageEnemyAt(SDL_Point position, float damage, float radius) {
    for (auto& e : enemies) {
        SDL_Point enemyPos = e->getPosition();
        float dx = enemyPos.x - position.x;
        float dy = enemyPos.y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= radius) {
            e->takeDamage(damage);
        }
    }
}
void EnemyManager::setEnemyTexture(SDL_Texture* tex) {
    enemyTexture = tex;
}