#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <enemy/Enemy.hpp>
#include <core/GameState.hpp>

// Dùng lại struct Wave của bạn, rất hợp lý
struct Wave {
    int enemyCount;
    float spawnInterval;
    float enemyHP;
    float enemySpeed;
};

class EnemyManager {
public:
    EnemyManager(SDL_Renderer* renderer, const std::vector<SDL_Point>& path);
    ~EnemyManager() = default;

    // --- Cấu hình ---
    void addWave(const Wave& wave);
    void setEnemyTexture(SDL_Texture* tex);
    
    // --- Điều khiển ---
    void start(); // Gọi hàm này một lần duy nhất để bắt đầu chuỗi các wave
    void update(float deltaTime);
    void render();

    // --- Lấy thông tin ---
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const; // Dùng để cho Tower nhắm bắn
    bool isGameOver() const;
    bool isFinished() const; // Kiểm tra xem đã hoàn thành tất cả các wave chưa

    void setGameState(GameState* gameState);
private:
    // Sử dụng State Machine để quản lý trạng thái rõ ràng hơn
    enum class State {
        WAITING_TO_START, // Đang chờ lệnh start()
        SPAWNING_WAVE,    // Đang trong quá trình sinh quái
        BETWEEN_WAVES,    // Đang trong thời gian nghỉ giữa 2 wave
        FINISHED          // Đã hoàn thành tất cả các wave
    };

    void spawnEnemy();
    void startNextWave();

    // SDL & Dữ liệu Game
    SDL_Renderer* renderer;
    SDL_Texture* enemyTexture = nullptr;
    std::vector<SDL_Point> enemyPath;
    std::vector<Wave> waves;
    std::vector<std::unique_ptr<Enemy>> enemies;
    GameState* gameState = nullptr; 

    // Trạng thái và điều khiển Wave
    State currentState = State::WAITING_TO_START;
    int currentWaveIndex = -1;
    float timeBetweenWaves = 5.0f; // 5 giây nghỉ giữa các wave

    // Timers & Counters
    float spawnTimer = 0.0f;
    float interWaveTimer = 0.0f; // Timer đếm thời gian nghỉ
    int enemiesSpawnedThisWave = 0;
};