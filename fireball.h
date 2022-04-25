#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef FIREBALL_H
#define FIREBALL_H

class fireball {
    SDL_Rect box;
    int speed;
    int tempX, tempY;
    SDL_Surface* fireballImage;
    int direction;
    SDL_Rect clips[4];
public:
    fireball(SDL_Surface* img, int x, int y, int nSpeed, int nDirection);
    void move();
    void show(SDL_Surface* screen, int x, int y);
    void destroy();
    SDL_Rect* getRect();
};

#endif