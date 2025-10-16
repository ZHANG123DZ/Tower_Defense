#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <core/Game.hpp>

using namespace std;

int main(int argc, char* argv[]) {
    Game game;
    bool isRunning = game.Initialize("Tower Defense", 1024, 1024, false);
    if (isRunning) {
        game.Run();
    }
    game.ShutDown();
    return 0;
}