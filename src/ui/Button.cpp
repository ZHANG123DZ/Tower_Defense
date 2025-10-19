#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ui/Button.hpp>
#include <iostream>
#include <algorithm>

Button::Button(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font)
    : renderer(renderer), x(x), y(y), width(w), height(h), text(text), font(font),
      backgroundTexture(nullptr), textTexture(nullptr), hasSrcRect(false)
{
    rect = { x, y, w, h };
    updateTextTexture();
}

Button::~Button() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

void Button::applyStyle(const ButtonStyleConfig& config) {
    borderColor = config.borderColor;
    textColor = config.textColor;
    bgColor = config.bgColor;

    if (!config.text.empty()) text = config.text;
    if (config.font) font = config.font;

    updateTextTexture();
}

void Button::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

void Button::setOnHover(std::function<void()> callback) {
    onHoverCallback = callback;
}

void Button::handleEvent(const SDL_Event& e) {
    int mx = 0, my = 0;

    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.type == SDL_MOUSEMOTION) {
            mx = e.motion.x;
            my = e.motion.y;
        } else {
            mx = e.button.x;
            my = e.button.y;
        }

        bool inside = (mx >= rect.x && mx <= rect.x + rect.w &&
                       my >= rect.y && my <= rect.y + rect.h);

        isHovered = inside;

        if (inside) {
            if (e.type == SDL_MOUSEMOTION && onHoverCallback) {
                onHoverCallback();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN &&
                e.button.button == SDL_BUTTON_LEFT && onClickCallback) {
                onClickCallback();
            }
        }
    }
}

void Button::render() {
    SDL_Rect dstRect = { x, y, width, height };

    if (backgroundTexture) {
        SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);

        if (hasSrcRect) {
            SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &dstRect);
        } else {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dstRect);
        }
    } else {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &dstRect);
    }

    // Vẽ viền 
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &dstRect);

    // Vẽ text
    if (textTexture) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);

        SDL_Rect dstText = {
            dstRect.x + (dstRect.w - textW) / 2,
            dstRect.y + (dstRect.h - textH) / 2,
            textW, textH
        };

        SDL_RenderCopy(renderer, textTexture, nullptr, &dstText);
    }

    // Hiệu ứng hover overlay nhẹ
    if (isHovered) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 40);
        SDL_RenderFillRect(renderer, &dstRect);
    }
}

void Button::setText(const std::string& newText) {
    text = newText;
    updateTextTexture();
}

void Button::setFont(TTF_Font* newFont) {
    font = newFont;
    updateTextTexture();
}

void Button::setBackgroundTexture(SDL_Texture* bgTex) {
    backgroundTexture = bgTex;
    SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);
    hasSrcRect = false;
}

void Button::setBackgroundTexture(SDL_Texture* bgTex, const SDL_Rect& srcRect) {
    backgroundTexture = bgTex;
    SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);
    this->srcRect = srcRect;
    hasSrcRect = true;
}

void Button::updateTextTexture() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    if (!font || text.empty()) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << "\n";
        return;
    }

    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!textTexture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << "\n";
    }
}
