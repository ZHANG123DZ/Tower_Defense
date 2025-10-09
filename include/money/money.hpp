#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Money {
private:
    int num;
    SDL_Texture* texture;
    SDL_Renderer* renderer;  
public:
    Money(SDL_Renderer* renderer) : renderer(renderer) {
        //3 buoc
    }
    ~Money() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
    void render() {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
    };
    void handleEvent(SDL_Event& e) {
        //check logic
    };
    void update() {

    };
};