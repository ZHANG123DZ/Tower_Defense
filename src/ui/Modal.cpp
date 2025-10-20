#include <ui/Modal.hpp>
#include <iostream>

Modal::Modal(SDL_Renderer* renderer, TTF_Font* font, const std::string& message,
             bool showCloseButton, int modalWidth, int modalHeight)
    : renderer(renderer), font(font), message(message), hasCloseButton(showCloseButton)
{
    int windowWidth = 800;
    int windowHeight = 600;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Nếu không truyền kích thước, dùng mặc định
    if (modalWidth <= 0) modalWidth = windowWidth / 2;
    if (modalHeight <= 0) modalHeight = windowHeight / 3;

    // Canh giữa
    modalRect = {
        (windowWidth - modalWidth) / 2,
        (windowHeight - modalHeight) / 2,
        modalWidth,
        modalHeight
    };

    visible = false;
    updateMessageTexture();

    if (hasCloseButton) {
        closeButton = new Button(renderer,
            modalRect.x + modalRect.w - 40,
            modalRect.y + 10,
            30, 30, "X", font);

        ButtonStyleConfig style;
        style.bgColor = {200, 50, 50, 255};
        style.textColor = {255, 255, 255, 255};
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
        messageTexture = nullptr;
    }
    if (closeButton) {
        delete closeButton;
        closeButton = nullptr;
    }
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    for (auto btn : buttons) {
        delete btn;
    }
    buttons.clear();
}

void Modal::updateMessageTexture() {
    if (messageTexture) {
        SDL_DestroyTexture(messageTexture);
        messageTexture = nullptr;
    }

    if (message.empty()) return;

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, message.c_str(), {0, 0, 0, 255}, modalRect.w - 40);
    if (!surface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }

    messageTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Modal::render() {
    if (!visible) return;

    for (auto& btn : buttons) {
        btn->render();
    }

    if (backgroundTexture) {
        int texW, texH;
        SDL_QueryTexture(backgroundTexture, nullptr, nullptr, &texW, &texH);

        SDL_Rect dstRect = {
            modalRect.x + 20,
            modalRect.y + 60,
            texW,
            texH
        };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dstRect);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Nền mờ (backdrop)
    SDL_SetRenderDrawColor(renderer, backdropColor.r, backdropColor.g, backdropColor.b, backdropColor.a);
    SDL_RenderFillRect(renderer, nullptr);

    // Hộp modal
    SDL_SetRenderDrawColor(renderer, modalBgColor.r, modalBgColor.g, modalBgColor.b, modalBgColor.a);
    SDL_RenderFillRect(renderer, &modalRect);

    // Hiển thị message
    if (messageTexture) {
        int texW, texH;
        SDL_QueryTexture(messageTexture, nullptr, nullptr, &texW, &texH);

        SDL_Rect dstRect = {
            modalRect.x + 20,
            modalRect.y + 60,
            texW,
            texH
        };
        SDL_RenderCopy(renderer, messageTexture, nullptr, &dstRect);
    }

    // Nút đóng
    if (hasCloseButton && closeButton) {
        closeButton->render();
    }
}

void Modal::handleEvent(const SDL_Event& e) {
    if (!visible) return;

    if (hasCloseButton && closeButton) {
        closeButton->handleEvent(e);
    }
    for (auto& btn : buttons) {
        btn->handleEvent(e);
    }
}

void Modal::applyStyle(const ModalStyleConfig& config) {
    modalBgColor = config.bgColor;
    backdropColor = config.backdropColor;
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

void Modal::setBackgroundTexture(SDL_Texture* texture) {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    backgroundTexture = texture;
}

void Modal::setText(const std::string& newMessage) {
    message = newMessage;
    updateMessageTexture();
}

const std::string& Modal::getText() const {
    return message;
}

void Modal::addButton(Button* button) {
    buttons.push_back(button);
}