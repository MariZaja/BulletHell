#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "base.h"
#include "fireball.h"
#include "menu.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef GAME_H
#define GAME_H
class game {
	SDL_Surface* screen, * charset;
	SDL_Rect camera, coord, playerRect;
	SDL_Surface* background,* health;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	char text[128], message[128];
	bool direction[4] = { 0, 0, 0, 0 };
	int t1, t2, frames, bialy, czarny, rozowy, shootSpeed, firedBalls, menuVersion, level, bullets, tempScore;
	int randX, randY;
	bool onButton[2];
	double delta, worldTime, fireTick, immortalTick;
	SDL_Surface* loadImage(const char* filename);
	void loadMap(const char* filename);
	void showMap();
	void handleEvents();
	void handleMenuEvents();
	void showmessage();
	void move();
	void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
	void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
	void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
	bool collision(SDL_Rect* rec1, SDL_Rect* rec2);
	void initBullets();
	void initBullets3();
	void enemyShoots();
	void enemyShoot(bullet* bullets[12]);
	void deleteBullets();
	void healthInfo();
	void reset();
	void gameOver();
	void shoot();
	void level1();
	void level2();
	void level3();
	void randPosition();
	void bonus();
	char* getButtonMessage(int i);
	bool running, menuRunning, gameOverBool, fireDelay, immortalDelay, exit, getBonus;
	static const int SCREEN_WIDTH = 640; //px
	static const int SCREEN_HEIGHT = 480; //px
	static const int speed = 5; //px
	static const int fps = 60;
	player* myPlayer;
	enemy* enemy1, *enemy2, *enemy3;
	bullet* bulletsA[12];
	bullet* bulletsB[12];
	bullet* bulletsC[12];
	bullet* bulletsD[12];
	fireball* fireballs[20];
	menu* myMenu;
	

public:
	game();
	~game();
	void start();
	void showMenu();
};

#endif