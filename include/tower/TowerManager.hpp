#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ui/Button.hpp>
#include <tower/Tower.hpp>
#include <route/Route.hpp>
#include <vector>
#include <string>

class TowerManager {
private:
    SDL_Renderer* renderer;
    std::vector<Button*> towerButtons;
    std::vector<int> prices;
    SDL_Texture* frameTexture = nullptr;
    int selectedTowerIndex = -1;
    TTF_Font* font;
    std::vector<Tower*> towers;
    Route& route;
public:
    TowerManager(SDL_Renderer* renderer, TTF_Font* font, Route& route);
    ~TowerManager();

    void loadTowers(const std::vector<std::string>& towerImages);
    void setFrameTexture(SDL_Texture* texture);
    void setPrices(const std::vector<int>& newPrices);

    void handleEvent(SDL_Event& e);
    // void update();
    void render();

    // Loại tháp đã chọn để đặt
    int getSelectedTower() const;
    void setSelectedTower(int index);
    bool isTowerSelectedForBuild() const {
        return selectedTowerIndex != -1;
    }

    // Các phương thức quản lý tháp
    void addTower(Tower* tower);
    void removeTower(Tower* tower);
    std::vector<Tower*> getTowers();
};
