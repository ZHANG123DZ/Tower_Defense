#include "ui/Modal.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <iostream>

Modal::Modal(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, bool showCloseButton)
    : renderer(renderer), font(font), message(message), hasCloseButton(showCloseButton)
{
    // Kích thước modal
    int modalWidth = 400;
    int modalHeight = 200;
    int windowWidth = 800, windowHeight = 600;

    modalRect = {
        (windowWidth - modalWidth) / 2,
        (windowHeight - modalHeight) / 2,
        modalWidth,
        modalHeight
    };

    updateMessageTexture();

    if (hasCloseButton) {
        TTF_Font* smallFont = font;

        closeButton = new Button(renderer,
            modalRect.x + modalRect.w - 40,
            modalRect.y + 10,
            30, 30, "X", smallFont);

        ButtonStyleConfig style;
        style.bgColor = { 255, 100, 100, 255 };
        style.textColor = { 255, 255, 255, 255 };
        style.borderRadius = 15;
        closeButton->applyStyle(style);

        closeButton->setOnClick([this]() {
            this->visible = false;
            if (onCloseCallback) onCloseCallback();
        });
    }
}

Modal::~Modal() {
    if (messageTexture) {
        SDL_DestroyTexture(messageTexture);
    }
    if (closeButton) {
        delete closeButton;
    }
}

void Modal::updateMessageTexture() {
    if (messageTexture) SDL_DestroyTexture(messageTexture);

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, message.c_str(), { 0, 0, 0, 255 }, modalRect.w - 40);
    if (!surface) {
        std::cerr << "Text render failed: " << TTF_GetError() << std::endl;
        return;
    }

    messageTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Modal::render() {
    if (!visible) return;

    // Backdrop
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backdropColor.r, backdropColor.g, backdropColor.b, backdropColor.a);
    SDL_RenderFillRect(renderer, nullptr);

    // Modal background
    if (borderRadius > 0) {
        roundedBoxRGBA(
            renderer,
            modalRect.x, modalRect.y,
            modalRect.x + modalRect.w,
            modalRect.y + modalRect.h,
            borderRadius,
            modalBgColor.r, modalBgColor.g, modalBgColor.b, modalBgColor.a
        );
    } else {
        SDL_SetRenderDrawColor(renderer, modalBgColor.r, modalBgColor.g, modalBgColor.b, modalBgColor.a);
        SDL_RenderFillRect(renderer, &modalRect);
    }

    // Message text
    if (messageTexture) {
        int texW, texH;
        SDL_QueryTexture(messageTexture, nullptr, nullptr, &texW, &texH);

        SDL_Rect dst = {
            modalRect.x + 20,
            modalRect.y + 60,
            texW, texH
        };
        SDL_RenderCopy(renderer, messageTexture, nullptr, &dst);
    }

    // Close button
    if (hasCloseButton && closeButton) {
        closeButton->render();
    }
}

void Modal::handleEvent(const SDL_Event& e) {
    if (!visible) return;

    if (hasCloseButton && closeButton) {
        closeButton->handleEvent(e);
    }
}

void Modal::applyStyle(const ModalStyleConfig& config) {
    modalBgColor = config.bgColor;
    backdropColor = config.backdropColor;
    borderRadius = config.borderRadius;
}

void Modal::setOnClose(std::function<void()> callback) {
    onCloseCallback = callback;
}

void Modal::setVisible(bool isVisible) {
    visible = isVisible;
}

bool Modal::isVisible() const {
    return visible;
}
