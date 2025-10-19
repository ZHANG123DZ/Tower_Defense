#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include "ui/Button.hpp"

struct ModalStyleConfig {
    SDL_Color backdropColor = {0, 0, 0, 128};
    SDL_Color bgColor = {255, 255, 255, 255};
};

class Modal {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    SDL_Rect modalRect;
    SDL_Color modalBgColor = { 255, 255, 255, 255 };
    SDL_Color backdropColor = { 0, 0, 0, 128 };

    std::string message;
    SDL_Texture* messageTexture = nullptr;

    bool visible = true;

    // Nút đóng
    bool hasCloseButton = false;
    Button* closeButton = nullptr;

    std::function<void()> onCloseCallback;

    void updateMessageTexture();

public:
    Modal(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, bool showCloseButton = true);
    ~Modal();

    void render();
    void handleEvent(const SDL_Event& e);

    void applyStyle(const ModalStyleConfig& config);
    void setOnClose(std::function<void()> callback);
    void setVisible(bool isVisible);
    bool isVisible() const;
};
