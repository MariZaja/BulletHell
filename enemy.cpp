#include "enemy.h"

enemy::enemy(SDL_Surface* img, int x, int y) {
	enemyImage = img;
	enemyX = x;
	enemyY = y;
	box.x = x;
	box.y = y;
	box.w = enemyImage->w;
	box.h = enemyImage->h;
	health = 100;
	direction = right;
}

void enemy::show(SDL_Surface* screen) {
	SDL_BlitSurface(enemyImage, NULL, screen, &box);
}

void enemy::setPosition(int x, int y) {
	box.x = x;
	box.y = y;
}

SDL_Rect* enemy::getRect() {
	return &box;
}

void enemy::setHealth() {
	health -= 1;
}

int enemy::getHealth() {
	return health;
}

void enemy::move() {
	if (direction == right) {
		enemyX += 2;
		if (enemyX >= 1900) {
			direction = left;
		}
	}
	else {
		enemyX -= 2;
		if (enemyX <= 100) {
			direction = right;
		}
	}
	box.x = enemyX;
}

int enemy::getX() {
	return enemyX;
}