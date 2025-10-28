#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <map>

#include <pages/Page.hpp>

class Page;
class Route {
private:
    SDL_Renderer* renderer;
    Page* currentPage;
    std::map<std::string, SDL_Cursor*> cursorCache;
    SDL_Cursor* defaultCursor;
public:
    Route(SDL_Renderer* renderer);
    ~Route();

    void handleEvent(SDL_Event& e);
    void render();
    void update();
    void setPage(Page* page);
    SDL_Renderer* getRenderer() const { return renderer; }

    // Cursor
    void SetCursor(const std::string& path, int hot_x = 0, int hot_y = 0);
    void SetDefaultCursor();
};
