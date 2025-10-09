#include <route/Route.hpp>
#include <pages/Page.hpp>
#include <pages/Home.hpp>
#include <pages/Battlefields.hpp>
#include <iostream>

Home* home;

Route::Route(SDL_Renderer* renderer) : currentPage(nullptr), renderer(renderer) {
    setPage(new Home(*this));
}

Route::~Route() {}

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