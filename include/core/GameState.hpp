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
    int money = 0;
    int hp;
    int score = 0;
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
    void decreaseMoney(int amount);
    void setMoney(int amount);
    int getMoney() const;
    void resetMoney();
    static GameState* getInstance();
    void reset();
    void addScore(int p);
    int getScore();

    void setHp(int h);
    void decreaseHp(int h);
    int getHp();
};
