#include <pages/Battle.hpp>
#include <pages/Battlefields.hpp>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <core/GameState.hpp>
#include <iostream>

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

// =================================================================
// HÀM KHỞI TẠO (CONSTRUCTOR)
// =================================================================
Battle::Battle(Route& route, int level)
    : route(route), level(level),
      texture(nullptr), arrowTexture(nullptr),
      arrowManager(nullptr), towerManager(nullptr),
      enemyManager(nullptr), endGameModal(nullptr),
      backButton(nullptr), map(nullptr)
{
    // === SỬA LỖI FONT: Gán vào biến thành viên, không tạo biến cục bộ nữa ===
    font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // === Tải tài nguyên đồ họa và âm thanh ===
    SDL_Surface* surface = IMG_Load("../assets/maps/map.jpg");
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
        SDL_FreeSurface(surface);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    }
    Arrow::loadHitSound("../assets/music/arrow_hit.mp3");

    // === Khởi tạo các thành phần giao diện (UI) ===
    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};
    backButton = new Button(route.getRenderer(), 65, 900, 165, 60, "", font);
    SDL_Rect backSrc = {65, 900, 165, 60};
    backButton->applyStyle(styleBtn);
    backButton->setBackgroundTexture(texture, backSrc);
    backButton->setOnClick([this, &route]() {
        route.setPage(new Battlefields(route));
    });

    endGameModal = new Modal(route.getRenderer(), font, "", false, 400, 250);
    endGameModal->applyStyle({{139, 69, 19, 230}, {0, 0, 0, 100}});
    endGameModal->setOnClose([this, &route]() {
        route.setPage(new Battlefields(route));
    });
    endGameModal->setVisible(false);

    // === Khởi tạo các Manager của game ===
    towerManager = new TowerManager(route.getRenderer(), font);
    SDL_Texture* frameTex = IMG_LoadTexture(route.getRenderer(), "../assets/tower/frameTower.png");
    towerManager->setFrameTexture(frameTex);
    towerManager->loadTowers({
        "../assets/tower/woodTower.png",
        "../assets/tower/stoneTower.png",
        "../assets/tower/magicTower.png",
        "../assets/tower/machineTower.png"
    });

    arrowManager = new ArrowManager();
    arrowTexture = IMG_LoadTexture(route.getRenderer(), "../assets/arrow/arrow.png");
    arrowManager->setArrowTexture(arrowTexture);

    map = new Map(route.getRenderer(), "../assets/maps/map.jpg", 128, 128, 8);
    map->loadMap({
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {2,2,2,2,2,2,2,2},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0},
    });

    // === SỬA LỖI LOGIC: Cấu hình và khởi động EnemyManager phiên bản mới ===
    // 1. Khởi tạo EnemyManager với đường đi
    enemyManager = new EnemyManager(route.getRenderer(), {
        {0,512},{128,512},{256,512},{384,512},{512,512},{640,512},{768,512},{896,512}
    });

    // 2. Thêm các đợt quái (wave)
    enemyManager->addWave({.enemyCount = 5, .spawnInterval = 0.5f, .enemyHP = 100.0f, .enemySpeed = 80.0f});
    enemyManager->addWave({.enemyCount = 10, .spawnInterval = 0.4f, .enemyHP = 120.0f, .enemySpeed = 90.0f});
    enemyManager->addWave({.enemyCount = 15, .spawnInterval = 0.3f, .enemyHP = 150.0f, .enemySpeed = 100.0f});

    // 3. Gán texture cho kẻ địch
    SDL_Surface* enemySurface = IMG_Load("../assets/tower/woodTower.png");
    if (!enemySurface) {
        std::cerr << "Failed to load enemy image: " << IMG_GetError() << std::endl;
    } else {
        SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(route.getRenderer(), enemySurface);
        SDL_FreeSurface(enemySurface);
        enemyManager->setEnemyTexture(enemyTexture);
    }

    // 4. BẮT ĐẦU chuỗi wave tự động
    enemyManager->start();

    // === Thiết lập trạng thái ban đầu của game ===
    GameState::getInstance()->resume(); 
    GameState::getInstance()->setResult(GameResult::None);
    std::cout << "[Battle] Game state has been set to RUNNING." << std::endl;
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
                towers.push_back(newTower);
                clicked->setHasTower(true);
            }
        }
    }
}

void Battle::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    if (map) map->render();
    for (auto* tower : towers) tower->render(renderer);
    if (arrowManager) arrowManager->render(renderer);
    if (enemyManager) enemyManager->render();
    if (backButton) backButton->render();
    if (towerManager) towerManager->render();

    if (endGameModal && endGameModal->isVisible()) {
        endGameModal->render();
    }

    SDL_RenderPresent(renderer);
}

// =================================================================
// HÀM UPDATE
// =================================================================
void Battle::update() {
    constexpr float deltaTime = 1.0f / 60.0f;
    auto gameState = GameState::getInstance();

    if (gameState->isPaused() || (endGameModal && endGameModal->isVisible())) {
        return;
    }

    // Cập nhật tất cả các manager
    if (arrowManager) arrowManager->update(deltaTime);
    // if (towerManager) towerManager->update(deltaTime); // Nếu có
    if (enemyManager) enemyManager->update(deltaTime);

    // Lấy danh sách quái để cho tower tấn công
    const auto& enemyPtrs = enemyManager->getEnemies();
    std::vector<Enemy*> enemiesView;
    enemiesView.reserve(enemyPtrs.size());
    for (const auto& e : enemyPtrs) {
        enemiesView.push_back(e.get());
    }

    // Tower tấn công
    for (auto* tower : towers) {
        tower->update(deltaTime, enemiesView, arrowManager);
    }
    
    // --- SỬA ĐỔI: Kiểm tra thắng/thua bằng các hàm mới ---
    if (gameState->getResult() == GameResult::None) {
        // Điều kiện thua
        if (enemyManager->isGameOver()) {
            gameState->setResult(GameResult::Lose);
            if (endGameModal) {
                endGameModal->setText("You Lose!");
                endGameModal->setVisible(true);
            }
            gameState->pause();
        } 
        // Điều kiện thắng
        else if (enemyManager->isFinished()) {
            gameState->setResult(GameResult::Win);
            if (endGameModal) {
                endGameModal->setText("You Win!");
                endGameModal->setVisible(true);
            }
            gameState->pause();
        }
    }
}