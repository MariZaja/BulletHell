#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef BULLET_H
#define BULLET_H

class bullet {
    SDL_Rect box; 
    int xvel, yvel, r; 
    double tempX, tempY;
    SDL_Surface* bulletImage; 
    int time;
    double angle;
public:
    bullet(SDL_Surface* img, int x, int y, int xvel, int yvel);
    void move();
    void move1();
    void show(SDL_Surface* screen, int x, int y);
    void setPosition(int x, int y);
    void set_random_angle();
    void destroyBullet();
    SDL_Rect* getRect();
};

#endif