#pragma once
#include <cstdint>
#include <algorithm>

enum class GameResult {
    None,
    Win,
    Lose
};

class GameState {
private:
    GameResult result;
    int money;
    bool paused;
    bool showEndModal;

public:
    GameState();
    void setResult(GameResult newResult);
    GameResult getResult() const;
    bool isWin() const;
    bool isLose() const;
    void pause();
    void setPaused(bool state);
    bool isPaused() const;
    void showEndGameModal();
    bool shouldShowEndGameModal() const;
    void hideEndGameModal();
    void addMoney(int amount);
    int getMoney() const;
    void resetMoney();
    static GameState* getInstance();
    void reset();
};
