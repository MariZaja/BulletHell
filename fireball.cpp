#include "fireball.h"

fireball::fireball(SDL_Surface* img, int x, int y, int nSpeed, int nDirection) {
	fireballImage = img;
	tempX = x;
	tempY = y;
	box.x = x;
	box.y = y;
	box.w = fireballImage->w;
	box.h = fireballImage->h;
	speed = 8;
	direction = nDirection;
	for (int i = 0; i < 4; i++) {
		clips[i].x = i * 50;
		clips[i].y = 0;
		clips[i].w = clips[i].h = 50;
	}
}

void fireball::move() {
	switch (direction % 4) {
		case 0:
			tempY -= speed;
			break;
		case 1:
			tempX += speed;
			break;
		case 2:
			tempY += speed;
			break;
		case 3:
			tempX -= speed;
			break;
	}
	box.x = tempX;
	box.y = tempY;
}

void fireball::show(SDL_Surface* screen, int x, int y) {
	if (fireballImage != NULL) {
		box.x -= x;
		box.y -= y;
		SDL_BlitSurface(fireballImage, &clips[direction % 4], screen, &box);
	}
}

SDL_Rect* fireball::getRect() {
	return &box;
}

void fireball::destroy() {
	box.x = box.y = tempX = tempY = -1;
	box.w = 0;
	box.h = 0;
	fireballImage = NULL;
}