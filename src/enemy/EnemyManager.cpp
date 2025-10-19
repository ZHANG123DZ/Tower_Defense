#include <enemy/EnemyManager.hpp>
#include <iostream>
#include <algorithm>

EnemyManager::EnemyManager(SDL_Renderer* renderer, const std::vector<SDL_Point>& path)
    : renderer(renderer), enemyPath(path) {
    // Khởi tạo các giá trị ban đầu nếu cần
}

void EnemyManager::addWave(const Wave& wave) {
    waves.push_back(wave);
}

void EnemyManager::setEnemyTexture(SDL_Texture* tex) {
    enemyTexture = tex;
}

void EnemyManager::start() {
    if (!waves.empty() && currentState == State::WAITING_TO_START) {
        std::cout << "[Manager] Starting the first wave sequence." << std::endl;
        // Bắt đầu ngay wave đầu tiên bằng cách chuyển sang trạng thái nghỉ
        // và đặt timer để nó kích hoạt ngay lập tức
        currentState = State::BETWEEN_WAVES;
        interWaveTimer = timeBetweenWaves; 
    }
}

void EnemyManager::startNextWave() {
    currentWaveIndex++;
    if (currentWaveIndex >= (int)waves.size()) {
        currentState = State::FINISHED;
        std::cout << "[Manager] All waves have been cleared!" << std::endl;
        return;
    }

    // Reset các biến đếm cho wave mới
    enemiesSpawnedThisWave = 0;
    spawnTimer = 0.0f;
    currentState = State::SPAWNING_WAVE;

    std::cout << "========================================" << std::endl;
    std::cout << "[Manager] Starting Wave " << currentWaveIndex + 1 << "/" << waves.size() << std::endl;
    std::cout << "========================================" << std::endl;
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
    if (isGameOver()) return;

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
            else if (enemies.empty()) { // Đã spawn hết và không còn con nào sống
                std::cout << "[Manager] Wave " << currentWaveIndex + 1 << " completed." << std::endl;
                if (currentWaveIndex >= (int)waves.size() - 1) {
                    currentState = State::FINISHED; // Sóng cuối cùng đã xong
                } else {
                    currentState = State::BETWEEN_WAVES; // Chuyển sang trạng thái nghỉ
                    interWaveTimer = 0.0f;
                }
            }
            break;
        }

        case State::FINISHED:
            // Tất cả các wave đã hoàn thành
            break;
    }

    // Cập nhật và xóa kẻ địch (luôn chạy trừ khi game đã xong)
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
    }

    // Xóa những kẻ địch đã chết hoặc đã đến đích
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](const std::unique_ptr<Enemy>& e) {
                if (e->reachedEnd()) {
                    this->baseHP--;
                    std::cout << "[Game] An enemy reached the base! HP left: " << this->baseHP << std::endl;
                    return true;
                }
                if (e->isDead()) {
                    // Có thể thêm logic cộng tiền ở đây hoặc trả về số lượng bị giết
                    return true;
                }
                return false;
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

int EnemyManager::getBaseHP() const {
    return baseHP;
}

bool EnemyManager::isGameOver() const {
    return baseHP <= 0;
}

bool EnemyManager::isFinished() const {
    return currentState == State::FINISHED;
}