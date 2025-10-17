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
      enemiesReachedEnd(0),
      baseHP(20),
      timeBetweenWaves(5.0f),  // nghỉ 5 giây giữa các wave
      waveTimer(0.0f)
{
}

EnemyManager::~EnemyManager() {
    enemies.clear();
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

    //Kiểm tra xem enemyPath có bị trống không
    if (enemyPath.empty()) {
        std::cout << "[Warning] Enemy path is EMPTY when starting wave " 
                  << currentWaveIndex + 1 << std::endl;
    } else {
        std::cout << "[Info] Starting wave " << currentWaveIndex + 1
                  << " with path length: " << enemyPath.size() << std::endl;
    }

    std::cout << "Wave " << currentWaveIndex + 1 
              << " started! Enemies: " << wave.enemyCount << std::endl;
}



bool EnemyManager::allEnemiesDead() const {
    for (const auto& e : enemies)
    if (!e->isDead()) return false;
    return true;
}
void EnemyManager::spawnEnemy(float hp, float speed) {
    //Nếu path bị trống thì bỏ qua spawn để tránh lỗi
    if (enemyPath.empty()) {
        std::cout << "[Error] Cannot spawn enemy — path is empty!" << std::endl;
        return;
    }
    auto enemy = std::make_unique<Enemy>(renderer, enemyPath, hp, speed);
    if (enemyTexture) { 
        enemy->setTexture(enemyTexture);
    }
    enemies.push_back(std::move(enemy));
    totalSpawned++;
}
void EnemyManager::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
        [this](const std::unique_ptr<Enemy>& e) {
            if (e->reachedEnd()) {
                this->enemiesReachedEnd++;
                baseHP--;
                std::cout << "Enemy reached the base! HP left: " << baseHP << std::endl;
                    return true;
                }
                return e->isDead();
            }),
            enemies.end()
        );
    }
    
    void EnemyManager::update(float deltaTime) {
    if (isGameOver()) return;
    
    // Nếu đang spawn quái trong wave hiện tại
    if (spawning) {
        spawnTimer += deltaTime;
        if (enemiesSpawnedInWave < currentWave.enemyCount && spawnTimer >= currentWave.spawnInterval) {
            spawnEnemy(currentWave.enemyHP, currentWave.enemySpeed);
            enemiesSpawnedInWave++;
            spawnTimer = 0.0f;
        }
        
        // Khi đã spawn đủ và không còn quái sống → kết thúc wave
        if (enemiesSpawnedInWave >= currentWave.enemyCount && allEnemiesDead()) {
            spawning = false;
            waveTimer = 0.0f; // bắt đầu đếm thời gian nghỉ
            std::cout << "Wave " << currentWaveIndex + 1 << " completed! Waiting for next wave...\n";
        }
    }
    else {
        // Nếu wave đã xong thì đếm thời gian nghỉ
        if (currentWaveIndex >= 0 && currentWaveIndex < (int)waves.size() - 1) {
            waveTimer += deltaTime;
            if (waveTimer >= timeBetweenWaves) {
                startWave(waves[currentWaveIndex + 1]); // tự động bắt đầu wave kế tiếp
            }
        }
    }
    
    // Cập nhật di chuyển quái
    for (auto& e : enemies) {
        e->update(deltaTime);
    }
    
    removeDeadEnemies();
}

void EnemyManager::render() {
    for (auto& e : enemies)
    e->render();
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
        if (distance <= radius)
        e->takeDamage(damage);
    }
}

void EnemyManager::setEnemyTexture(SDL_Texture* tex) {
    enemyTexture = tex;
}

int EnemyManager::getenemiesReachedEnd() const {
    return enemiesReachedEnd;
}

int EnemyManager::getbaseHP() const {
    return baseHP;
}

bool EnemyManager::isGameOver() const {
    return baseHP <= 0;
}
const std::vector<Wave>& EnemyManager::getWaves() const {
    return waves;
}
bool EnemyManager::isWaveComplete() const {
    return !spawning && allEnemiesDead();
}