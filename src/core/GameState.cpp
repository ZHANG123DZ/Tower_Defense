#include <core/GameState.hpp>

GameState* GameState::getInstance() {
    static GameState instance;
    return &instance;
}

GameState::GameState()
    : result(GameResult::None), money(0), paused(false), showEndModal(false) {}

void GameState::setResult(GameResult newResult) {
    result = newResult;
    paused = true;
    showEndModal = true;
}

GameResult GameState::getResult() const { return result; }
bool GameState::isWin() const { return result == GameResult::Win; }
bool GameState::isLose() const { return result == GameResult::Lose; }

void GameState::pause() { paused = true; }
void GameState::setPaused(bool state) { paused = state; }
bool GameState::isPaused() const { return paused; }

void GameState::showEndGameModal() { showEndModal = true; paused = true; }
bool GameState::shouldShowEndGameModal() const { return showEndModal; }
void GameState::hideEndGameModal() { showEndModal = false; }

void GameState::addMoney(int amount) { money += amount; }
void GameState::decreaseMoney(int amount) { money -= amount;}
void GameState::setMoney(int amount) { money = amount; }
int GameState::getMoney() const { return money; }
void GameState::resetMoney() { money = 0; }

void GameState::reset() {
    result = GameResult::None;
    money = 0;
    paused = false;
    showEndModal = false;
}
