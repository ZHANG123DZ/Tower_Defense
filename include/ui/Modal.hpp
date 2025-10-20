#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include <ui/Button.hpp>

struct ModalStyleConfig {
    SDL_Color bgColor;
    SDL_Color backdropColor;
};

class Modal {
public:
    Modal(SDL_Renderer* renderer, TTF_Font* font, const std::string& message,
          bool showCloseButton = true, int modalWidth = 0, int modalHeight = 0);
    ~Modal();

    void render();
    void handleEvent(const SDL_Event& e);
    void applyStyle(const ModalStyleConfig& config);
    void setOnClose(std::function<void()> callback);
    void setVisible(bool isVisible);
    bool isVisible() const;
    
    void setBackgroundTexture(SDL_Texture* texture);
    void setText(const std::string& newMessage);
    const std::string& getText() const;

private:
    SDL_Texture* backgroundTexture = nullptr;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* messageTexture = nullptr;
    SDL_Rect modalRect{};
    SDL_Color modalBgColor{};
    SDL_Color backdropColor{};
    bool hasCloseButton;
    bool visible = false;
    std::string message;
    Button* closeButton = nullptr;
    std::function<void()> onCloseCallback;

    void updateMessageTexture();
};
