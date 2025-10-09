#include <iostream>
#include <string.h>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <route/Route.hpp>
#include <core/Game.hpp>
#include <core/Map.hpp>
#include <ui/Modal.hpp>
#include <ui/Button.hpp>
#include <pages/Home.hpp>
#include <pages/Page.hpp>

Route* route = nullptr;
Map *map =nullptr;

std::vector<std::vector<int>> level = {
    {2,2,2,2,2,1,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2},
    {2,2,2,2,2,2,2,2,2,2,2,2}
};


Game::Game() : isRunning(false) {}

Game::~Game() {
    ShutDown();
}

bool Game::Initialize(const char* title, int width, int height, bool fullScreen = false) {
    if ((SDL_Init(SDL_INIT_VIDEO) || SDL_Init(SDL_INIT_AUDIO) || SDL_Init(SDL_INIT_TIMER) ) < 0) {
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer error: " << Mix_GetError() << std::endl;
        return 1;
    }

    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullScreen) flags = SDL_WINDOW_FULLSCREEN;
    //Tạo ra cửa sổ (window)
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) {
        SDL_Quit();
        return false;
    }
    SDL_Surface* icon = IMG_Load("../assets/1 Tiles/battle.png");
    if (!icon) {
        std::cerr << "Failed to load icon: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    //Tạo ra bút vẽ (renderer): dùng để vẽ hình ảnh, text, màu nền, texture,… lên cửa sổ.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Quit();
        return false;
    }

    TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 24);
    setFont(font);

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    Mix_Music* bgMusic = Mix_LoadMUS("../assets/music/a.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_PlayMusic(bgMusic, -1);
    //Muốn thay map nào thì để đường dẫn ảnh vào đây
    route = new Route(renderer);
    // map = new Map(renderer, "../assets/1 Tiles/battle.png", 32*4, 32*4, 12);
    // map->loadMap(level);
    isRunning = true;
    return true;
}

void Game::Run() {
    SDL_Event e;

     // Tạo nút
    // Button myButton(renderer, 200, 200, 200, 60, "Click Me", font);

    // Style ban đầu
    // ButtonStyleConfig style;
    // style.bgColor = {0, 0, 255, 255};       // Blue
    // style.textColor = {255, 255, 255, 255}; // White
    // style.borderColor = {0, 0, 0, 255}; 
    // style.borderRadius = 99;    // Black
    // style.font = font;
    // style.text = "Click Me";
    // myButton.applyStyle(style);

    // Gắn callback
    // myButton.setOnClick([]() {
    //     std::cout << "Đã click nút kkkk!\n";
    // });

    // myButton.setOnHover([&]() {
    //     // Khi hover thì đổi màu
    //     ButtonStyleConfig hoverStyle = style;
    //     hoverStyle.bgColor = {255, 100, 0, 255}; // Cam
    //     myButton.applyStyle(hoverStyle);
    // });



    // Modal* modal = new Modal(renderer, font, "Bạn chắc chắn muốn thoát không?", true);
    
    // ModalStyleConfig style;
    // style.bgColor = { 255, 255, 230, 255 };
    // style.backdropColor = { 0, 0, 0, 180 };
    // style.borderRadius = 16;

    // modal->applyStyle(style);

    // modal->setOnClose([modal]() {
    //     modal->setVisible(false);
    // });

    // SDL_Surface* surface = IMG_Load("../assets/home/home2.png"); // đường dẫn ảnh
    // if (!surface) {
    //     std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
    //     return;
    // }
    // SDL_Texture* homeTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_FreeSurface(surface);

    while (isRunning)
    {
        while (SDL_PollEvent(&e)) 
        {   
            // myButton.handleEvent(e);
            if (e.type == SDL_QUIT) isRunning = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
            
            // if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            //     int mouseX = e.button.x;
            //     int mouseY = e.button.y;

            //     map->handleClick(mouseX, mouseY);  
            // }
            
            route->handleEvent(e);
            // if (modal->isVisible()) {
            //     modal->handleEvent(e);
            // }
        } 
        route->update();
        // SDL_Color color = {255, 255, 255}; // white
        // SDL_Surface* surface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", color);
        // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // SDL_FreeSurface(surface);

        // SDL_Rect dest = {100, 100, 300, 50};
        // SDL_SetRenderDrawColor(renderer, 10, 12, 40, 255);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);

        // if (modal->isVisible()) {
        //         modal->handleEvent(e);
        //         modal->render();
        //     }

        // myButton.render();
        // if (showModal) {
        //     modal.render();
        // }
        // SDL_RenderCopy(renderer, texture, nullptr, &dest);
        // SDL_RenderCopy(renderer, homeTexture, NULL, NULL);
        // map->render();

        route->render();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Game::ShutDown() {
     if (map) {
        delete map;
        map = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
    }
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
    }
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::setFont(TTF_Font* currentFont) {
    font = currentFont;
}

void Game::setBgMusic(Mix_Music* music) {
    bgMusic = music;
}
