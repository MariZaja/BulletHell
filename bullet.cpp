#include "bullet.h"

bullet::bullet(SDL_Surface* img, int x, int y, int xVel, int a){
    tempX = x;
    tempY = y;
    box.x = x;
    box.y = y;
    bulletImage = img;
    box.w = bulletImage->w;
    box.h = bulletImage->h;
    xvel = xVel;
    r = a;
    time = 0;
    angle = a;
}

void bullet::move(){
    tempX += xvel*cos(angle * M_PI / 90);
    tempY += xvel*sin(angle * M_PI / 90);
    box.x = (int)tempX;
    box.y = (int)tempY;
}

void bullet::move1() {
    tempX += xvel;
    tempY += xvel;
    box.x = (int)tempX;
    box.y = (int)tempY;
}

void bullet::show(SDL_Surface* screen, int x, int y){
    if (bulletImage != NULL) {
        box.x -= x;
        box.y -= y;
        SDL_BlitSurface(bulletImage, NULL, screen, &box);
    }
}

SDL_Rect* bullet::getRect(){
    return &box;
}

void bullet::setPosition(int x, int y) {
    box.x -= x;
    box.y -= y;
}

void bullet::destroyBullet() {
    box.x = box.y = tempX = tempY = -1;
    box.w = 0;
    box.h = 0;
    bulletImage = NULL;
}

void bullet::set_random_angle(){
    angle = rand() % (165 - 15 + 1) + 15;
}