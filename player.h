#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef PLAYER_H
#define PLAYER_H

class player {
	SDL_Rect box;
	SDL_Surface* playerImage,* health, *fireballsImage;
	int characterX, characterY, characterHealth, score;
	SDL_Rect clips[4];
    int presentDirection;
    bool moving = false;
    struct fireball {
        SDL_Rect box;
        int direction;
        int speed;
    };
    
public:
	player(SDL_Surface* img);
	~player();
    SDL_Rect* getRect();
    void setPositionX(int x);
    void setPositionY(int y);
    int getPositionX();
    int getPositionY();
    void showPlayer(SDL_Surface* screen);
    bool getMoving();
    void setMoving(bool m);
    void setDirection(int d);
    int getDirection();
    int getHealth();
    void setHealth(int h);
    void showHealth(int);
    void shoot();
    int getScore();
    void setScore(int change);
    void reset();
};


#endif
