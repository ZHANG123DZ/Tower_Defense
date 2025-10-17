#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <enemy/EnemyManager.hpp>
#include <core/Game.hpp>
#include <core/Map.hpp>
#include <enemy/Enemy.hpp>

Map* map = nullptr;
EnemyManager* enemyManager = nullptr;
// tổng hp của thành
//int baseHealth = 20;
int lastenemyReachedEndCount = 0;
std::vector<std::vector<int>> level = {
    {2,2,2,2,2,1,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2}
};

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), font(nullptr), bgMusic(nullptr) {}
Game::~Game() { ShutDown(); }

bool Game::Initialize(const char* title, int width, int height, bool fullScreen) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf init failed: " << TTF_GetError() << std::endl;
        return false;
    }

    // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    //     std::cerr << "SDL_mixer error: " << Mix_GetError() << std::endl;
    //     return false;
    // }

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullScreen) flags = SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/a.mp3");
    // if (bgMusic) Mix_PlayMusic(bgMusic, -1);

    // Tạo map & enemy manager
    map = new Map(renderer, "../assets/1 Tiles/battle.jpg", 32 * 4, 32 * 4, 12);
    enemyManager = new EnemyManager(renderer, {{100,100},{400,100},{400,400},{700,400}});
    enemyManager->addWave({2, 1.0f, 100.0f, 80.0f});
    enemyManager->addWave({10, 0.8f, 150.0f, 90.0f});
    enemyManager->addWave({20, 0.6f, 200.0f, 100.0f});
    enemyManager->addWave({30, 0.5f, 250.0f, 110.0f}); 
    enemyManager->addWave({40, 0.4f, 300.0f, 130.0f}); 
    enemyManager->startWave(enemyManager->getWaves()[0]);  // bắt đầu wave 1
    
    map->loadMap(level);
    SDL_Surface* enemySurface = IMG_Load("../assets/1 Tiles/download.png");
    if(!enemySurface) {
        std::cerr << "Failed to load enemy image: " << IMG_GetError() << std::endl;
        return false;
    }
    else {
        SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
        SDL_FreeSurface(enemySurface);
        enemyManager->setEnemyTexture(enemyTexture); //  Hàm này ta sẽ thêm vào EnemyManager
    }
    isRunning = true;
    return true;
}
/// ////
void Game::Run() {
    SDL_Event e;
    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) isRunning = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) isRunning = false;

            // SPACE -> Gây sát thương vùng quanh chuột
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                enemyManager->damageEnemyAt({x, y}, 30.0f, 100.0f);
                std::cout << "Damage at (" << x << "," << y << ")\n";
            }
        }

        SDL_SetRenderDrawColor(renderer, 10, 12, 40, 255);
        SDL_RenderClear(renderer);

        map->render();
        enemyManager->update(0.016f);
        if(enemyManager->isGameOver()){
            std::cout << "GAME OVER! You Lose!" << std::endl;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Your base has been destroyed!", window);
            isRunning = false; // Kết thúc game
        }
        SDL_SetRenderDrawColor(renderer, 10, 12, 40, 255);
        SDL_RenderClear(renderer);
        enemyManager->render();
        //Thanh máu của thành
        int baseHP = enemyManager->getbaseHP();
        int maxHP = 20; // Giả sử max HP là 20
        int barWidth = 200;
        int barHeight = 20;
        int x = 800 - barWidth - 20;//
        int y = 20;
        SDL_Rect border = { x - 2, y - 2, barWidth + 4, barHeight + 4 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &border);
        // máu còn lại
        SDL_Rect hpRect = {x , y , static_cast<int>((barWidth * baseHP) / (float)maxHP) , barHeight};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &hpRect);
        //hiển thị frame
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Game::ShutDown() {
    delete map;
    delete enemyManager;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    if (font) TTF_CloseFont(font);
    if (bgMusic) Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
