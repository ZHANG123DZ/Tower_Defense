#include <pages/History.hpp>
#include <pages/Battlefields.hpp>
#include <fstream>
#include <sstream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

// Hàm hỗ trợ render text
void History::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "TTF_RenderText_Blended Error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
}

// Hàm đọc file history.txt
void History::loadHistoryFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open history file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        HistoryEntry entry;
        std::getline(ss, entry.time, '|');
        std::getline(ss, entry.level, '|');
        std::getline(ss, entry.status, '|');
        std::getline(ss, entry.score, '|');

        // Hàm trim để loại bỏ khoảng trắng 2 đầu
        auto trim = [](std::string& s) {
            size_t start = s.find_first_not_of(" \t");
            size_t end = s.find_last_not_of(" \t");
            if (start == std::string::npos || end == std::string::npos) s = "";
            else s = s.substr(start, end - start + 1);
        };

        trim(entry.time);
        trim(entry.level);
        trim(entry.status);
        trim(entry.score);

        historyEntries.push_back(entry);
    }

    file.close();
}

History::History(Route& route)
    : route(route), texture(nullptr), backButton(nullptr), bgMusic(nullptr), font(nullptr) {
    
    // Load font
    font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", 22);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    // Load background image
    SDL_Surface* surface = IMG_Load("../assets/data/history.png");
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(route.getRenderer(), surface);
        SDL_FreeSurface(surface);
    }

    // Load music
    bgMusic = Mix_LoadMUS("../assets/music/a.mp3");
    if (!bgMusic) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    }
    else {
        Mix_PlayMusic(bgMusic, -1); // loop nhạc
    }

    // Load lịch sử từ file
    loadHistoryFromFile("../assets/data/history.txt");

    // Tạo nút Back
    ButtonStyleConfig styleBtn;
    styleBtn.borderColor = {0, 0, 0, 0};

    backButton = new Button(route.getRenderer(), 45, 60, 160, 120, "", font);
    SDL_Rect backSrc = { 45, 60, 160, 120 };
    backButton->applyStyle(styleBtn);
    backButton->setBackgroundTexture(texture, backSrc);
    backButton->setOnClick([&]() {
        route.setPage(new Battlefields(route));
    });

    // Nếu cần, init rect (vd toàn màn hình)
    rect = { 0, 0, 1024, 1024 };
}

History::~History() {
    if (texture) SDL_DestroyTexture(texture);
    if (backButton) delete backButton;
    if (bgMusic) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgMusic);
    }
    if (font) TTF_CloseFont(font);
}

void History::handleEvent(SDL_Event& e) {
    if (backButton) backButton->handleEvent(e);
}

void History::render(SDL_Renderer* renderer) {
    // Vẽ background
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    // Vẽ nút back
    if (backButton) {
        backButton->render();
    }

    // Vẽ nội dung lịch sử
    int startX = 95;
    int startY = 340;
    int lineHeight = 60;

    SDL_Color white = { 255, 255, 255, 255 };

    for (size_t i = 0; i < historyEntries.size(); ++i) {
        const auto& entry = historyEntries[i];
        renderText(renderer, font, entry.time, startX, startY + i * lineHeight, white);
        renderText(renderer, font, entry.level, startX + 290, startY + i * lineHeight, white);
        renderText(renderer, font, entry.status, startX + 480, startY + i * lineHeight, white);
        renderText(renderer, font, entry.score, startX + 700, startY + i * lineHeight, white);
    }

    SDL_RenderPresent(renderer);
}

void History::update() {}