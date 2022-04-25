#include "menu.h"
#include "game.h"

menu::menu() {
	buttons[0].x = buttons[1].x = 220;
	buttons[0].y = 220;
	buttons[1].y = 270;
	buttons[0].w = buttons[1].w = 200;
	buttons[0].h = buttons[1].h = 40;
	sprintf(message, "Start");
	sprintf(buttonMessage[0], "New Game");
	sprintf(buttonMessage[1], "Quit Game");
}

char* menu::getMessage() {
	return message;
}

void menu::setMessage(char* mes) {
	sprintf(message, mes);
}

SDL_Rect* menu::getButton(int i) {
	return &buttons[i];
}

char* menu::getButtonMessage(int i) {
	return buttonMessage[i];
}

void menu::setButtonMessage(int i, char* mes) {
	sprintf(buttonMessage[i], mes);
}