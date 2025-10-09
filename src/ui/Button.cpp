#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <ui/Button.hpp>
#include <iostream>
#include <algorithm>

Button::Button(SDL_Renderer* renderer, int x, int y, int w, int h, const std::string& text, TTF_Font* font)
    : renderer(renderer), x(x), y(y), width(w), height(h), text(text), font(font)
{
    rect = { x, y, w, h };
    updateTextTexture();
}


Button::~Button() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
}

void Button::applyStyle(const ButtonStyleConfig& config) {
    borderColor = config.borderColor;
    textColor = config.textColor;
    bgColor = config.bgColor;
    borderRadius = config.borderRadius;

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
    int mx, my;
    bool inside = false;

    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.type == SDL_MOUSEMOTION) {
            mx = e.motion.x;
            my = e.motion.y;
        } else {
            mx = e.button.x;
            my = e.button.y;
        }

        inside = (mx >= rect.x && mx <= rect.x + rect.w &&
                  my >= rect.y && my <= rect.y + rect.h);

        isHovered = inside;

        if (inside) {
            if (e.type == SDL_MOUSEMOTION && onHoverCallback) {
                onHoverCallback();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && onClickCallback) {
                onClickCallback();
            }
        }
    }
}

void Button::render() {
    rect = { x, y, width, height };

    SDL_Color drawColor = isHovered
        ? SDL_Color{
            static_cast<Uint8>(std::max(0, bgColor.r - 40)),
            static_cast<Uint8>(std::max(0, bgColor.g - 40)),
            static_cast<Uint8>(std::max(0, bgColor.b - 40)),
            bgColor.a
        }
        : bgColor;

    if (borderRadius > 0) {
        roundedBoxRGBA(renderer,
                       rect.x, rect.y,
                       rect.x + rect.w, rect.y + rect.h,
                       borderRadius,
                       drawColor.r, drawColor.g, drawColor.b, drawColor.a);

        roundedRectangleRGBA(renderer,
                             rect.x, rect.y,
                             rect.x + rect.w, rect.y + rect.h,
                             borderRadius,
                             borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    } else {
        SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(renderer, &rect);
    }

    if (textTexture) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);

        SDL_Rect dstRect = {
            rect.x + (rect.w - textW) / 2,
            rect.y + (rect.h - textH) / 2,
            textW, textH
        };

        SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
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
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    backgroundTexture = bgTex;
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
