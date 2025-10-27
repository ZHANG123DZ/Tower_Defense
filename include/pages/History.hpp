#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <ui/Button.hpp>

struct HistoryEntry {
    std::string time;
    std::string level;
    std::string status;
    std::string score;
};

class History : public Page {
private:
    Route& route;
    SDL_Texture* texture;
    SDL_Rect rect;               
    Button* backButton;
    Mix_Music* bgMusic;
    TTF_Font* font;

    std::vector<HistoryEntry> historyEntries;

    void loadHistoryFromFile(const std::string& filename);
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);

public:
    History(Route& route);
    ~History();

    void render(SDL_Renderer* renderer) override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
};
