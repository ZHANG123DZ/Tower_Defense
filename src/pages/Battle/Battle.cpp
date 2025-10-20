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
        
    if (moneyTexture) {
        SDL_DestroyTexture(moneyTexture);
        moneyTexture = nullptr;
    }

    if (hpTexture) {
        SDL_DestroyTexture(hpTexture);
        hpTexture = nullptr;
    }

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
    font = TTF_OpenFont("../assets/fonts/Jersey15-Regular.ttf", 45);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // ===== TẢI NỀN BẢN ĐỒ =====
    std::string pathMap = "../assets/maps/map" + std::to_string(level) + ".png";
    SDL_Surface* surface = IMG_Load(pathMap.c_str());
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
    endGameModal = new Modal(route.getRenderer(), font, "", false, 500, 500);
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
    std::vector<int> prices = {100, 200, 300, 400};
    towerManager->setPrices(prices);
    // ===== ARROW MANAGER =====
    arrowManager = new ArrowManager();
    arrowTexture = IMG_LoadTexture(route.getRenderer(), "../assets/arrow/arrow.png");
    arrowManager->setArrowTexture(arrowTexture);

    // ===== MAP MANAGER =====
    std::string pathMapManager = "../assets/maps/map" + std::to_string(level) + ".png";
    map = new Map(route.getRenderer(), pathMapManager, 128, 128, 8);

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
    std::string pathRoute = "../assets/route/route" + std::to_string(level) + ".txt";
    std::ifstream routeFile(pathRoute);
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

    enemyManager = new EnemyManager(route.getRenderer(), routePoints);
    auto gameState = GameState::getInstance();
    enemyManager->setGameState(gameState);
    
    std::string configPath = "../assets/config/config"+ std::to_string(level) +".txt";
    std::ifstream configFile(configPath);

    int startingMoney = 0, startingHP = 0;
    configFile >> startingMoney >> startingHP;
    gameState->setMoney(startingMoney);
    enemyManager->setBaseHP(startingHP);

    // ===== LOAD WAVES =====
    std::string path = "../assets/waves/waves" + std::to_string(level) + ".txt";

    std::ifstream waveFile(path);
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
    std::string pathEnemy = "../assets/enemy/enemy" + std::to_string(level) + ".png";
    SDL_Surface* enemySurface = IMG_Load(pathEnemy.c_str());
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
            std::vector<int> prices = {100, 200, 300, 400};
            std::vector<float> damages = {30.0f, 40.0f, 50.0f, 60.0f};
            auto gameState = GameState::getInstance();
            int currentMoney = gameState->getMoney();
            static const std::vector<std::string> links = {
                "../assets/tower/woodTower.png",
                "../assets/tower/stoneTower.png",
                "../assets/tower/magicTower.png",
                "../assets/tower/machineTower.png"
            };

            if (selectTower >= 0 && selectTower < (int)links.size() && currentMoney>=prices[selectTower]) {
                SDL_Rect dest = clicked->getDestRect();
                Tower* newTower = new Tower(
                    route.getRenderer(),
                    links[selectTower],
                    damages[selectTower],
                    dest.x, dest.y, dest.w, dest.h
                );
                towerManager->addTower(newTower);
                clicked->setHasTower(true);
                gameState->decreaseMoney(prices[selectTower]);
            }
        }
    }
}

void Battle::updateMoneyTexture(SDL_Renderer* renderer, int money) {
    if (money == lastMoney) return;

    // Giải phóng texture cũ nếu có
    if (moneyTexture) {
        SDL_DestroyTexture(moneyTexture);
        moneyTexture = nullptr;
    }

    lastMoney = money;
    std::string moneyText = std::to_string(money);
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, moneyText.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    moneyTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!moneyTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    // Lưu kích thước để render dễ dàng 
    moneyRect = {550, 5, textSurface->w, textSurface->h};

    SDL_FreeSurface(textSurface);
}

void Battle::updateHPTexture(SDL_Renderer* renderer, int hp) {
    if (hp == lastHP) return;  // Nếu máu không đổi thì không làm gì

    if (hpTexture) {
        SDL_DestroyTexture(hpTexture);
        hpTexture = nullptr;
    }

    lastHP = hp;
    std::string hpText = std::to_string(hp);
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, hpText.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    hpTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!hpTexture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    hpRect = {950, 10, textSurface->w, textSurface->h};

    SDL_FreeSurface(textSurface);
}

void Battle::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    
    if (map) map->render();
    if (arrowManager) arrowManager->render(renderer);
    if (enemyManager) enemyManager->render();
    if (backButton) backButton->render();
    if (towerManager) towerManager->render();

    if (moneyTexture) {
        SDL_RenderCopy(renderer, moneyTexture, nullptr, &moneyRect);
    }
    if (hpTexture) {
        SDL_RenderCopy(renderer, hpTexture, nullptr, &hpRect);
    }
    if (endGameModal && endGameModal->isVisible()) {
        endGameModal->render();
    }

    SDL_RenderPresent(renderer);
}

void Battle::update() {
    constexpr float deltaTime = 1.0f / 60.0f;
    auto gameState = GameState::getInstance();

    int money = gameState->getMoney();
    updateMoneyTexture(route.getRenderer(), money);

    int currentHp = enemyManager->getBaseHP();
    updateHPTexture(route.getRenderer(), currentHp);

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
                Route* routePtr = &route;

                SDL_Texture* modalBg = IMG_LoadTexture(route.getRenderer(), "../assets/data/defeat.png");
                endGameModal->setBackgroundTexture(modalBg);
                Button* retryBtn = new Button(route.getRenderer(), 325, 745, 140, 40, "", font);
                Button* menuBtn = new Button(route.getRenderer(), 470, 745, 140, 40, "", font);
                Button* exitBtn = new Button(route.getRenderer(), 615, 745, 140, 40, "", font);
                
                retryBtn->setOnClick([this, routePtr]() {
                    routePtr->setPage(new Battle(*routePtr, level));
                });
                menuBtn->setOnClick([this, routePtr]() {
                    routePtr->setPage(new Battlefields(*routePtr));
                });
                exitBtn->setOnClick([]() {
                    SDL_Event quitEvent;
                    quitEvent.type = SDL_QUIT;
                    SDL_PushEvent(&quitEvent);
                });
                endGameModal->addButton(retryBtn);
                endGameModal->addButton(menuBtn);
                endGameModal->addButton(exitBtn);
                endGameModal->setVisible(true);
            }
            gameState->pause();
        } 
        // Điều kiện thắng
        else if (enemyManager->isFinished()) {
            gameState->setResult(GameResult::Win);
            if (endGameModal) {
                Route* routePtr = &route;

                SDL_Texture* modalBg = IMG_LoadTexture(route.getRenderer(), "../assets/data/victory.png");
                endGameModal->setBackgroundTexture(modalBg);

                Button* nextLevelBtn = new Button(route.getRenderer(), 325, 700, 140, 40, "", font);
                Button* retryBtn = new Button(route.getRenderer(), 490, 700, 140, 40, "", font);
                Button* menuBtn = new Button(route.getRenderer(), 650, 700, 140, 40, "", font);
                
                nextLevelBtn->setOnClick([this, routePtr]() {
                    routePtr->setPage(new Battle(*routePtr, level < maxLevel ? level + 1 : level));
                });

                retryBtn->setOnClick([this, routePtr]() {
                    routePtr->setPage(new Battle(*routePtr, level));
                });

                menuBtn->setOnClick([routePtr]() {
                    routePtr->setPage(new Battlefields(*routePtr));
                });

                endGameModal->addButton(nextLevelBtn);
                endGameModal->addButton(retryBtn);
                endGameModal->addButton(menuBtn);
                endGameModal->setVisible(true);
            }
            gameState->pause();
        }
    }
}
