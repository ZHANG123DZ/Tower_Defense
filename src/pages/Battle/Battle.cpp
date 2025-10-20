#include <pages/Battle.hpp>
#include <pages/Battlefields.hpp>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <core/GameState.hpp>
#include <iostream>
#include <fstream>
#include <vector> 

Battle::~Battle() {
    if (texture) SDL_DestroyTexture(texture);
    if (arrowTexture) SDL_DestroyTexture(arrowTexture);

    delete arrowManager;
    delete towerManager;
    delete enemyManager;
    delete endGameModal;
    delete backButton;
    delete map;

    Arrow::freeSound();
    Mix_CloseAudio();
}

Battle::Battle(Route& route, int level)
    : route(route), level(level),
      texture(nullptr), arrowTexture(nullptr),
      arrowManager(nullptr), towerManager(nullptr),
      enemyManager(nullptr), endGameModal(nullptr),
      backButton(nullptr), map(nullptr)
{
    // ===== TẢI FONT =====
    font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // ===== TẢI NỀN BẢN ĐỒ =====
    SDL_Surface* surface = IMG_Load("../assets/maps/map.jpg");
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
        SDL_FreeSurface(surface);
    }

    // ===== ÂM THANH =====
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    }
    Arrow::loadHitSound("../assets/music/arrow_hit.mp3");

    // ===== NÚT QUAY LẠI =====
    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};
    backButton = new Button(route.getRenderer(), 65, 900, 165, 60, "", font);
    SDL_Rect backSrc = {65, 900, 165, 60};
    backButton->applyStyle(styleBtn);
    backButton->setBackgroundTexture(texture, backSrc);
    backButton->setOnClick([this, &route]() {
        route.setPage(new Battlefields(route));
    });

    // ===== MODAL KẾT THÚC GAME =====
    endGameModal = new Modal(route.getRenderer(), font, "", false, 549, 455);
    endGameModal->setOnClose([this, &route]() {
        route.setPage(new Battlefields(route));
    });
    endGameModal->setVisible(false);

    // ===== TOWER MANAGER =====
    towerManager = new TowerManager(route.getRenderer(), font);
    SDL_Texture* frameTex = IMG_LoadTexture(route.getRenderer(), "../assets/tower/frameTower.png");
    towerManager->setFrameTexture(frameTex);
    towerManager->loadTowers({
        "../assets/tower/woodTower.png",
        "../assets/tower/stoneTower.png",
        "../assets/tower/magicTower.png",
        "../assets/tower/machineTower.png"
    });

    // ===== ARROW MANAGER =====
    arrowManager = new ArrowManager();
    arrowTexture = IMG_LoadTexture(route.getRenderer(), "../assets/arrow/arrow.png");
    arrowManager->setArrowTexture(arrowTexture);

    // ===== MAP MANAGER =====
    map = new Map(route.getRenderer(), "../assets/maps/map.jpg", 128, 128, 8);

    // Load map từ file
    std::ifstream mapFile("../assets/maps/map.txt");
    if (!mapFile.is_open()) {
        std::cerr << "Failed to open map.txt" << std::endl;
    } else {
        int cols, rows;
        mapFile >> cols >> rows;
        std::vector<std::vector<int>> mapData(rows, std::vector<int>(cols));
        for (int y = 0; y < rows; ++y)
            for (int x = 0; x < cols; ++x)
                mapFile >> mapData[y][x];
        map->loadMap(mapData);
        mapFile.close();
    }

    // ===== LOAD ROUTE =====
    std::ifstream routeFile("../assets/route/route.txt");
    std::vector<SDL_Point> routePoints;
    int x, y;

    if (!routeFile.is_open()) {
        std::cerr << "Failed to open route.txt" << std::endl;
    } else {
        while (routeFile >> x >> y) {
            routePoints.push_back({x, y});
        }
        routeFile.close();
    }

    enemyManager = new EnemyManager(route.getRenderer(), routePoints, gameState);


    // ===== LOAD WAVES =====
    std::ifstream waveFile("../assets/waves/waves.txt");
    if (!waveFile.is_open()) {
        std::cerr << "Failed to open waves.txt" << std::endl;
    } else {
        int enemyCount;
        float spawnInterval, enemyHP, enemySpeed;
        while (waveFile >> enemyCount >> spawnInterval >> enemyHP >> enemySpeed) {
            enemyManager->addWave({
                .enemyCount = enemyCount,
                .spawnInterval = spawnInterval,
                .enemyHP = enemyHP,
                .enemySpeed = enemySpeed
            });
        }
        waveFile.close();
    }

    // ===== TEXTURE KẺ ĐỊCH =====
    SDL_Surface* enemySurface = IMG_Load("../assets/tower/woodTower.png");
    if (!enemySurface) {
        std::cerr << "Failed to load enemy image: " << IMG_GetError() << std::endl;
    } else {
        SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(route.getRenderer(), enemySurface);
        SDL_FreeSurface(enemySurface);
        enemyManager->setEnemyTexture(enemyTexture);
    }

    // ===== BẮT ĐẦU GAME =====
    enemyManager->start();
    GameState::getInstance()->pause(); 
    GameState::getInstance()->setResult(GameResult::None);
    GameState::getInstance()->setPaused(false);
}


void Battle::handleEvent(SDL_Event& e) {
    if (endGameModal && endGameModal->isVisible()) {
        endGameModal->handleEvent(e);
        return;
    }

    if (backButton) backButton->handleEvent(e);
    if (towerManager) towerManager->handleEvent(e);

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx = e.button.x;
        int my = e.button.y;

        Tile* clicked = map->getTileAt(mx, my);
        if (!clicked) return;

        if (clicked->getId() == 1 && !clicked->getHasTower()) {
            int selectTower = towerManager->getSelectedTower();
            static const std::vector<std::string> links = {
                "../assets/tower/woodTower.png",
                "../assets/tower/stoneTower.png",
                "../assets/tower/magicTower.png",
                "../assets/tower/machineTower.png"
            };

            if (selectTower >= 0 && selectTower < (int)links.size()) {
                SDL_Rect dest = clicked->getDestRect();
                Tower* newTower = new Tower(
                    route.getRenderer(),
                    links[selectTower],
                    dest.x, dest.y, dest.w, dest.h
                );
                towerManager->addTower(newTower);
                clicked->setHasTower(true);
            }
        }
    }
}

void Battle::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    
    if (map) map->render();
    if (arrowManager) arrowManager->render(renderer);
    if (enemyManager) enemyManager->render();
    if (backButton) backButton->render();
    if (towerManager) towerManager->render();

    if (endGameModal && endGameModal->isVisible()) {
        endGameModal->render();
    }

    SDL_RenderPresent(renderer);
}

void Battle::update() {
    constexpr float deltaTime = 1.0f / 60.0f;
    auto gameState = GameState::getInstance();
    int money = gameState->getMoney();
    if (gameState->isPaused() || (endGameModal && endGameModal->isVisible())) {
        return;
    }
    
    // Cập nhật tất cả các manager
    if (arrowManager) arrowManager->update(deltaTime);
    // if (towerManager) towerManager->update(deltaTime);
    if (enemyManager) enemyManager->update(deltaTime);

    // Lấy danh sách quái để cho tower tấn công
    const auto& enemyPtrs = enemyManager->getEnemies();
    std::vector<Enemy*> enemiesView;
    enemiesView.reserve(enemyPtrs.size());
    for (const auto& e : enemyPtrs) {
        enemiesView.push_back(e.get());
    }

    // Tower tấn công
    auto towers = towerManager->getTowers(); 
    for (auto* tower : towers) {
        tower->update(deltaTime, enemiesView, arrowManager);
    }
    
    // --- SỬA ĐỔI: Kiểm tra thắng/thua bằng các hàm mới ---
    if (gameState->getResult() == GameResult::None) {
        // Điều kiện thua
        if (enemyManager->isGameOver()) {

            gameState->setResult(GameResult::Lose);
            if (endGameModal) {
                SDL_Texture* modalBg = IMG_LoadTexture(route.getRenderer(), "../assets/data/defeat.png");
                endGameModal->setBackgroundTexture(modalBg);
                endGameModal->setVisible(true);
            }
            gameState->pause();
        } 
        // Điều kiện thắng
        else if (enemyManager->isFinished()) {
            gameState->setResult(GameResult::Win);
            if (endGameModal) {
                SDL_Texture* modalBg = IMG_LoadTexture(route.getRenderer(), "../assets/data/victory.png");
                endGameModal->setBackgroundTexture(modalBg);
                endGameModal->setVisible(true);
            }
            gameState->pause();
        }
    }
}