#include <enemy/EnemyManager.hpp>
#include <core/GameState.hpp>
#include <iostream>
#include <algorithm>

EnemyManager::EnemyManager(SDL_Renderer* renderer, const std::vector<SDL_Point>& path)
    : renderer(renderer), enemyPath(path) {
}

void EnemyManager::addWave(const Wave& wave) {
    waves.push_back(wave);
}

void EnemyManager::setEnemyTexture(SDL_Texture* tex) {
    enemyTexture = tex;
}

void EnemyManager::start() {
    if (!waves.empty() && currentState == State::WAITING_TO_START) {
        currentState = State::BETWEEN_WAVES;
        interWaveTimer = timeBetweenWaves; 
    }
}

void EnemyManager::startNextWave() {
    currentWaveIndex++;
    if (currentWaveIndex >= (int)waves.size()) {
        currentState = State::FINISHED;
        return;
    }

    // Reset các biến đếm cho wave mới
    enemiesSpawnedThisWave = 0;
    spawnTimer = 0.0f;
    currentState = State::SPAWNING_WAVE;
}

void EnemyManager::spawnEnemy() {
    if (enemyPath.empty()) return;

    const Wave& currentWave = waves[currentWaveIndex];
    auto enemy = std::make_unique<Enemy>(renderer, enemyPath, currentWave.enemyHP, currentWave.enemySpeed);
    if (enemyTexture) {
        enemy->setTexture(enemyTexture);
    }
    enemies.push_back(std::move(enemy));
}

void EnemyManager::update(float deltaTime) {
    if (isGameOver()) {
        return;
    }

    // Quản lý trạng thái của manager
    switch (currentState) {
        case State::WAITING_TO_START:
            // Không làm gì cho đến khi được gọi start()
            break;

        case State::BETWEEN_WAVES:
            interWaveTimer += deltaTime;
            if (interWaveTimer >= timeBetweenWaves) {
                startNextWave();
            }
            break;

        case State::SPAWNING_WAVE: {
            const Wave& currentWave = waves[currentWaveIndex];

            // Logic sinh quái
            if (enemiesSpawnedThisWave < currentWave.enemyCount) {
                spawnTimer += deltaTime;
                if (spawnTimer >= currentWave.spawnInterval) {
                    spawnEnemy();
                    enemiesSpawnedThisWave++;
                    spawnTimer = 0.0f;
                }
            }
            // Kiểm tra khi nào wave kết thúc
            else if (enemies.empty()) {
                if (currentWaveIndex >= static_cast<int>(waves.size()) - 1) {
                    currentState = State::FINISHED;
                } else {
                    currentState = State::BETWEEN_WAVES;
                    interWaveTimer = 0.0f;
                }
            }
            break;
        }

        case State::FINISHED:
            break;
    }

    // Cập nhật enemy
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
    }

    // === Cộng tiền trước khi xóa ===
    int totalMoneyGained = 0;
    for (const auto& enemy : enemies) {
        if (enemy->isDead()) {
            totalMoneyGained += 100;
        }
    }

    if (totalMoneyGained > 0 && gameState) {
        gameState->addMoney(totalMoneyGained);
    }

    // === Xóa enemy đã chết hoặc đến đích ===
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](const std::unique_ptr<Enemy>& e) {
                if (e->reachedEnd()) {
                    this->baseHP--;
                    return true;
                }
                return e->isDead();
            }),
        enemies.end()
    );
}


void EnemyManager::render() {
    for (auto& e : enemies) {
        e->render();
    }
}

// Implement các hàm getter
const std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() const {
    return enemies;
}

void EnemyManager::setBaseHP(int hp) {
    baseHP = hp;
}

int EnemyManager::getBaseHP() const {
    return baseHP;
}

bool EnemyManager::isGameOver() const {
    return baseHP <= 0;
}

bool EnemyManager::isFinished() const {
    return currentState == State::FINISHED;
}

void EnemyManager::setGameState(GameState* gs) {
    this->gameState = gs;
}