#include <tower/TowerManager.hpp>
#include <iostream>
#include <algorithm>
#include <tower/Tower.hpp>

TowerManager::TowerManager(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font) {}

TowerManager::~TowerManager() {
    for (auto* btn : towerButtons) delete btn;
    for (auto* tower : towers) {
        delete tower;
    }
    towers.clear();
}

void TowerManager::setFrameTexture(SDL_Texture* texture) {
    frameTexture = texture;
}

void TowerManager::loadTowers(const std::vector<std::string>& towerImages) {
    struct TowerBtn { int x, y, w, h; };
    std::vector<TowerBtn> positions = {
        {240, 850, 128, 128},
        {380, 850, 128, 128},   
        {520, 850, 128, 128},   
        {660, 850, 128, 128},
    };

    int cnt = std::min((int)towerImages.size(), (int)positions.size());
    ButtonStyleConfig style;
    style.borderColor = {0, 0, 0, 0};

    for (int i = 0; i < cnt; i++) {
        SDL_Texture* tex = IMG_LoadTexture(renderer, towerImages[i].c_str());
        if (!tex) {
            std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
            continue;
        }

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        auto& pos = positions[i];
        Button* btn = new Button(renderer, pos.x, pos.y, pos.w, pos.h, "", font);
        btn->applyStyle(style);
        btn->setBackgroundTexture(tex);

        btn->setOnClick([this, i]() {
            selectedTowerIndex = i;
        });

        towerButtons.push_back(btn);
    }
}

void TowerManager::handleEvent(SDL_Event& e) {
    for (auto* btn : towerButtons) {
        btn->handleEvent(e);
    }
}

void TowerManager::render() {
    for (int i = 0; i < (int)towerButtons.size(); i++) {
        towerButtons[i]->render();

        if (i == selectedTowerIndex && frameTexture) {
            SDL_Rect rect = { towerButtons[i]->getX(), towerButtons[i]->getY(),
                              towerButtons[i]->getWidth(), towerButtons[i]->getHeight() };
            SDL_RenderCopy(renderer, frameTexture, nullptr, &rect);
        }
    }
    for (Tower* tower : towers) {
        if (tower) {
            tower->render(renderer);
        }
    }
}

int TowerManager::getSelectedTower() const {
    return selectedTowerIndex;
}

void TowerManager::addTower(Tower* tower) {
    if (tower) {
        towers.push_back(tower);
    }
}

void TowerManager::removeTower(Tower* tower) {
    auto it = std::find(towers.begin(), towers.end(), tower);
    if (it != towers.end()) {
        delete *it;              
        towers.erase(it);        
    }
}

std::vector<Tower*> TowerManager::getTowers() {
    return towers;
}