#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#ifndef MENU_H
#define MENU_H

class menu {
	char message[128];
	SDL_Rect buttons[2];
	char buttonMessage[2][128];
public:
	menu();
	char* getMessage();
	void setMessage(char* mes);
	SDL_Rect* getButton(int i);
	char* getButtonMessage(int i);
	void setButtonMessage(int i, char* mes);
};

#endif
