#include "player.h"

player::player(SDL_Surface* img) {
	playerImage = img;
	score = 0;
	box.x = 320;
	box.y = 380;
	box.w = 50;
	box.h = 50;
	characterX = 320;
	characterY = 380;
	characterHealth = 5;
	presentDirection = 4;
	for (int i = 0; i < 4; i++) {
		clips[i].x = i * 50;
		clips[i].y = 0;
		clips[i].w = clips[i].h = 50;
	}
}


player::~player() {
	SDL_FreeSurface(playerImage);
}

SDL_Rect* player::getRect(){
	return &box;
}
int player::getPositionX(){
	return box.x;
}
int player::getPositionY(){
	return box.y;
}

void player::setPositionX(int x){
	box.x = x;
}

void player::setPositionY(int y) {
	box.y = y;
}

void player::showPlayer(SDL_Surface* screen){
	SDL_BlitSurface(playerImage, &clips[presentDirection%4], screen, &box);
}

void player::setDirection(int d){
	presentDirection = d;
}

int player::getDirection(){
	return presentDirection;
}

bool player::getMoving() {
	return moving;
}

void player::setMoving(bool m){
	moving = m;
}

int player::getHealth() {
	return characterHealth;
}

void player::setHealth(int h) {
	characterHealth += h;
}

int player::getScore() {
	return score;
}

void player::setScore(int change) {
	score += change;
	if (score < 0) {
		score = 0;
	}
}

void player::reset() {
	box.x = 320;
	box.y = 380;
	box.w = 50;
	box.h = 50;
	characterX = 320;
	characterY = 380;
	characterHealth = 5;
	presentDirection = 4;
}