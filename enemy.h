#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef ENEMY_H
#define ENEMY_H

class enemy {
    SDL_Rect box;
    int enemyX, enemyY, health, direction;
    SDL_Surface* enemyImage;
    SDL_Rect clips[2];
    double frame;
    static const int right = 0;
    static const int left = 1;
public:
    enemy(SDL_Surface* img, int x, int y);
    void show(SDL_Surface* screen);
    void setPosition(int x, int y);
    SDL_Rect* getRect();
    void setHealth();
    int getHealth();
    int getX();
    void move();
};

#endif
