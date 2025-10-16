#include <pages/Battle.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <pages/Home.hpp>
#include <pages/Battlefields.hpp>
#include <tower/TowerManager.hpp>
#include <tile/Tile.hpp>
#include <core/Map.hpp>
#include <enemy/EnemyManager.hpp>
#include <tower/Arrow.hpp>

Battle::~Battle() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (arrowManager) {
        arrowManager->clear();
        delete arrowManager;
        arrowManager = nullptr;
    }
    for (auto* tower : towers) {
        delete tower;
    }
    towers.clear();

    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    Arrow::freeSound();
    Mix_CloseAudio();
}

Battle::Battle(Route& route, int level) : route(route), level(level) {
    // Load font
    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // Load background
    SDL_Surface* surface = IMG_Load("../assets/maps/map.jpg"); 
    if (!surface) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
    SDL_FreeSurface(surface); 

    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};

    // Tạo nút Back
    backButton = new Button(route.getRenderer(), 65, 900, 165, 60, "", font);

    SDL_Rect backSrc = {65, 900, 165, 60};
    //Gắn các thuộc tính cho nút back
    backButton->applyStyle(styleBtn);
    backButton->setBackgroundTexture(texture, backSrc);
    backButton->setOnClick([&]() {
        route.setPage(new Battlefields(route));
    });

    towerManager = new TowerManager(route.getRenderer(), font);
    SDL_Texture* frameTex = IMG_LoadTexture(route.getRenderer(), "../assets/tower/frameTower.png");
    towerManager->setFrameTexture(frameTex);

    // Load tower images
    towerManager->loadTowers({
        "../assets/tower/woodTower.png",
        "../assets/tower/stoneTower.png",
        "../assets/tower/magicTower.png",
        "../assets/tower/machineTower.png"
    });

    arrowManager = new ArrowManager();
    arrowTexture = IMG_LoadTexture(route.getRenderer(), "../assets/arrow/arrow.png");
    arrowManager->setArrowTexture(arrowTexture);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Arrow::loadHitSound("../assets/music/arrow_hit.mp3");

    map = new Map(route.getRenderer(), "../assets/maps/map.jpg", 128, 128, 8);

    map->loadMap({
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2, 2, 2, 2},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
    });

    enemyManager = new EnemyManager(route.getRenderer(), {{0,512},{128,512},{256,512},{384,512}, {512,512}, {640,512}, {768,512}, {896,512}});
    enemyManager->addWave({5, 1.0f, 100.0f, 80.0f});
    enemyManager->addWave({10, 0.8f, 150.0f, 90.0f});
    enemyManager->addWave({15, 0.6f, 200.0f, 100.0f});
    enemyManager->addWave({20, 0.5f, 250.0f, 110.0f}); 
    enemyManager->startWave({5, 1.0f, 100.0f, 80.0f});
    enemyManager->startWave({10, 0.8f, 150.0f, 90.0f});
    enemyManager->startWave({15, 0.6f, 200.0f, 100.0f});
    enemyManager->startWave({20, 0.5f, 250.0f, 110.0f});
    SDL_Surface* enemySurface = IMG_Load("../assets/tower/magicTower.png");
    if(!enemySurface) {
        std::cerr << "Failed to load enemy image: " << IMG_GetError() << std::endl;
    }
    else {
        SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(route.getRenderer(), enemySurface);
        SDL_FreeSurface(enemySurface);
        enemyManager->setEnemyTexture(enemyTexture);
    }
}

void Battle::handleEvent(SDL_Event& e) {
    backButton->handleEvent(e);
    towerManager->handleEvent(e);
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx = e.button.x;
        int my = e.button.y;

        Tile* clicked = map->getTileAt(mx, my);
        if (!clicked) return;

        if (clicked->getId() == 1 && !clicked->getHasTower()) { 
            int selectTower = towerManager->getSelectedTower();
            std::string link;
            switch (selectTower)
            {
            case 0:
                link = "../assets/tower/woodTower.png";
                break;
            case 1:
                link = "../assets/tower/stoneTower.png";
                break;    
            case 2:
                link = "../assets/tower/magicTower.png";
                break;   
            case 3:
                link = "../assets/tower/machineTower.png";
                break;   
            default:
                link = "";
                break;
            }
            if (selectTower >= 0 || !link.empty()) {
                SDL_Rect dest = clicked->getDestRect();

                Tower* newTower = new Tower(
                    route.getRenderer(),
                    link,
                    dest.x,
                    dest.y,
                    dest.w,
                    dest.h
                );
                towers.push_back(newTower);
                clicked->setHasTower(true);
            }
        }
    }
}

void Battle::render(SDL_Renderer* renderer) {
    // Vẽ background
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    // Vẽ nút
    map->render();
    for (auto* tower : towers) {
        tower->render(renderer);
    }
    backButton->render();
    towerManager->render();
    arrowManager->render(renderer);
    enemyManager->update(0.016f);
    enemyManager->render();
    SDL_RenderPresent(renderer);
}

void Battle::update() {
    float deltaTime = 1.0f / 60.0f;

    enemyManager->update(deltaTime);

    std::vector<Enemy*> activeEnemies = enemyManager->getEnemies();

    for (auto* tower : towers) {
        tower->update(deltaTime, activeEnemies, arrowManager);
    }

    arrowManager->update(deltaTime);
}