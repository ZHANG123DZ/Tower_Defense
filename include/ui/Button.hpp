#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct ButtonStyleConfig {
    SDL_Color borderColor = {0, 0, 0, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 255, 255};
    TTF_Font* font = nullptr;
    std::string text = "";
};

class Button {
private:
    int x, y, width, height;
    SDL_Rect rect;
    SDL_Rect srcRect;
    bool hasSrcRect = false;

    SDL_Color borderColor, textColor, bgColor;
    std::string text;
    TTF_Font* font = nullptr;

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_Texture* backgroundTexture = nullptr;

    bool isHovered = false;

    std::function<void()> onClickCallback;
    std::function<void()> onHoverCallback;

    void updateTextTexture();

public:
    Button(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font);
    ~Button();

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void applyStyle(const ButtonStyleConfig& config);

    void setOnClick(std::function<void()> callback);
    void setOnHover(std::function<void()> callback);

    void setText(const std::string& newText);
    void setFont(TTF_Font* newFont);

    void setBackgroundTexture(SDL_Texture* bgTex);
    void setBackgroundTexture(SDL_Texture* bgTex, const SDL_Rect& srcRect);

    void handleEvent(const SDL_Event& e);
    void render();
};
