#include <pages/Battlefields.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <route/Route.hpp>
#include <ui/Button.hpp>
#include <pages/Home.hpp>
#include <pages/Battle.hpp>
#include <pages/History.hpp>

Battlefields::~Battlefields() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    delete backButton;
    delete historyButton;
    for (auto* btn : battles) delete btn;
}

Battlefields::Battlefields(Route& route) : route(route) {
    // Load font
    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    if (!font) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // Load background
    SDL_Surface* surface = IMG_Load("../assets/battlefields/battlefields.png"); 
    if (!surface) {
    }
    texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
    SDL_FreeSurface(surface); 

    // Load nhạc
    // Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/battlefieldsMusic.mp3");
    // if (!bgMusic) {
    //     std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    // }
    // Mix_PlayMusic(bgMusic, -1);

    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};

    // Tạo nút Back
    backButton = new Button(route.getRenderer(), 65, 900, 165, 60, "", font);

    SDL_Rect backSrc = {65, 900, 165, 60};
    // Gắn các thuộc tính cho nút back
    backButton->applyStyle(styleBtn);
    backButton->setBackgroundTexture(texture, backSrc);
    backButton->setOnClick([&]() {
        route.setPage(new Home(route));
    });

    historyButton = new Button(route.getRenderer(), 790, 900, 165, 60, "", font);

    SDL_Rect historySrc = {790, 900, 165, 60};
    // Gắn các thuộc tính cho nút history
    historyButton->applyStyle(styleBtn);
    historyButton->setBackgroundTexture(texture, historySrc);
    historyButton->setOnClick([&]() {
        route.setPage(new History(route));
    });
    // Nút các màn chơi
    struct LevelBtn {
        int x, y, w, h;
    };

    // Danh sách các nút vào các màn chơi
    std::vector<LevelBtn> levelPositions = {
        {150, 235, 120, 120},   
        {450, 205, 120, 120},   
        {750, 230, 120, 120},   
        {595, 715, 120, 120},   
    };

    // Tạo từng nút thủ công theo danh sách trên
    int levels = (int)levelPositions.size();
    for (int i = 0; i < levels; i++) {
        auto& pos = levelPositions[i];
        Button* btn = new Button(route.getRenderer(), pos.x, pos.y, pos.w, pos.h,
                                 "", font);
        btn->applyStyle(styleBtn);
        SDL_Rect btnRect = {pos.x, pos.y, pos.w, pos.h};
        btn->setBackgroundTexture(texture, btnRect);
        btn->setOnClick([&, i]() {
            route.setPage(new Battle(route, i + 1));
        });
        battles.push_back(btn);
    }
}

void Battlefields::handleEvent(SDL_Event& e) {
    backButton->handleEvent(e);
    historyButton->handleEvent(e);
    for (auto* btn : battles) btn->handleEvent(e);
}

void Battlefields::render(SDL_Renderer* renderer) {
    // Vẽ background
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
    // Vẽ nút
    backButton->render();
    historyButton->render();
    for (auto* btn : battles) btn->render();

    SDL_RenderPresent(renderer);
}

void Battlefields::update() {
}