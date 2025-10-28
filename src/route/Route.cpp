#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <pages/Home.hpp>
#include <pages/Battlefields.hpp>
#include <iostream>

Home* home;

Route::Route(SDL_Renderer* renderer) : currentPage(nullptr), renderer(renderer) {
    setPage(new Home(*this));
    defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
}

Route::~Route() {
    for (auto const& [path, cursor] : cursorCache) {
        SDL_FreeCursor(cursor);
    }
    SDL_FreeCursor(defaultCursor);
}

void Route::handleEvent(SDL_Event& e) {
    if (currentPage) currentPage->handleEvent(e);
}

void Route::setPage(Page* page) {
    if (currentPage) delete currentPage;
    currentPage = page;
}

void Route::render() {
    if (currentPage) {
        currentPage->render(renderer);
    }
}

void Route::update() {
    if (currentPage) {
        currentPage->update();
    }
}

void Route::SetCursor(const std::string& path, int hot_x, int hot_y) {
    if (cursorCache.find(path) != cursorCache.end()) {
        SDL_SetCursor(cursorCache[path]);
        return;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load cursor image: " << path << " | Error: " << IMG_GetError() << std::endl;
        SetDefaultCursor();
        return;
    }

    SDL_Cursor* newCursor = SDL_CreateColorCursor(surface, hot_x, hot_y);
    SDL_FreeSurface(surface);

    if (!newCursor) {
        std::cerr << "Failed to create custom cursor from: " << path << " | Error: " << SDL_GetError() << std::endl;
        SetDefaultCursor();
        return;
    }

    cursorCache[path] = newCursor;
    SDL_SetCursor(newCursor);
}

void Route::SetDefaultCursor() {
    if (defaultCursor) {
        SDL_SetCursor(defaultCursor);
    }
}