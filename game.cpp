#include "game.h"

game::game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return;
	}
	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return;
	};
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Bullet Hell - Maria Zajaczkowska");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	charset = loadImage("./cs8x8.bmp");
	background = loadImage("./background.bmp");
	health = loadImage("./health.bmp");
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
	running = true;
	shootSpeed = 5;
	firedBalls = -1;
	fireDelay = false;
	immortalDelay = false;
	getBonus = false;
	fireTick = 0;
	immortalTick = 0;
	menuRunning = true;
	onButton[0] = onButton[1] = false;
	exit = false;
	menuVersion = 0;
	level = 1;
	bullets = -1;
	randX = randY = 0;

	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	bialy = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	rozowy = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xC3);

	myMenu = new menu();
	myPlayer = new player(loadImage("char.bmp"));
	enemy1 = new enemy(loadImage("enemyA.bmp"), 200, 150);
	enemy2 = new enemy(loadImage("enemyB.bmp"), 200, 150);
	enemy3 = new enemy(loadImage("enemyC.bmp"), 200, 150);
	initBullets();
}

game::~game() {
	delete myPlayer;
	delete enemy1;
	delete myMenu;
	deleteBullets();
	SDL_FreeSurface(background);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return;
}

void game::showMenu() {
	while (menuRunning) {
		SDL_ShowCursor(SDL_ENABLE);

		handleMenuEvents();

		SDL_BlitSurface(background, &camera, screen, NULL);

		sprintf(text, myMenu->getMessage());
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, bialy, czarny);
		DrawString(screen, screen->w / 2 - strlen(text) * 4, 18, text, charset);

		for (int i = 0; i < 2; i++) {
			sprintf(text, myMenu->getButtonMessage(i));
			if (onButton[i]) DrawRectangle(screen, myMenu->getButton(i)->x, myMenu->getButton(i)->y, 200, 40, rozowy, czarny);
			else DrawRectangle(screen, myMenu->getButton(i)->x, myMenu->getButton(i)->y, 200, 40, bialy, czarny);
			DrawString(screen, screen->w / 2 - strlen(text) * 4, myMenu->getButton(i)->y + 18, text, charset);
		}
		
		
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	};
}

void game::handleMenuEvents() {
	int mouseX, mouseY;
	SDL_Event menuEvent;
	while (SDL_PollEvent(&menuEvent)) {
		switch (menuEvent.type) {
		case SDL_KEYDOWN:
			if (menuEvent.key.keysym.sym == SDLK_ESCAPE) menuRunning = false;
			else if (menuEvent.key.keysym.sym == SDLK_n) running = true, reset(), start();
			else if (menuEvent.key.keysym.sym == SDLK_SPACE) menuRunning = true;
			break;
		case SDL_KEYUP:
			menuRunning = true;
			break;
		case SDL_QUIT:
			menuRunning = false;
			break;
		case SDL_MOUSEMOTION:
			mouseX = menuEvent.motion.x;
			mouseY = menuEvent.motion.y;
			for (int i = 0; i < 2; i++) {
				if (mouseX >= myMenu->getButton(i)->x && mouseX <= (myMenu->getButton(i)->x + myMenu->getButton(i)->w) && mouseY >= myMenu->getButton(i)->y && mouseY <= (myMenu->getButton(i)->y + myMenu->getButton(i)->h)) {
					onButton[i] = true;
				}
				else { onButton[i] = false; }
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseX = menuEvent.button.x;
			mouseY = menuEvent.button.y;
			for (int i = 0; i < 2; i++) {
				if (mouseX >= myMenu->getButton(i)->x && mouseX <= (myMenu->getButton(i)->x + myMenu->getButton(i)->w) && mouseY >= myMenu->getButton(i)->y && mouseY <= (myMenu->getButton(i)->y + myMenu->getButton(i)->h)) {
					onButton[i] = false;
					if (i == 1) {
						if (!menuVersion || menuVersion == 3) menuRunning = false;//quit game
						else menuVersion = 0, gameOver();//go to menu
					}
					else {
						if (menuVersion == 2) level+= 1;
						else if (menuVersion == 3) {
							menuVersion = 0;
							gameOver();
							break;
						}
						running = true;
						reset();
						start();
						reset();
						break;
					}
				}
			}
			break;
		};
	};
}

void game::start() {
	SDL_ShowCursor(SDL_DISABLE);
	t1 = SDL_GetTicks();
	worldTime = 0;
	randPosition();
	while (running) {
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;
		worldTime += delta;
		handleEvents();

		SDL_BlitSurface(background, &camera, screen, NULL);

		myPlayer->showPlayer(screen);

		bonus();

		if (level == 2) level2();
		else if (level == 3) level3();
		else level1();

		if (fireDelay) {
			fireTick += delta;
			DrawRectangle(screen, myPlayer->getRect()->x, myPlayer->getRect()->y + myPlayer->getRect()->h - 10, 50, 8, bialy, czarny);
			DrawRectangle(screen, myPlayer->getRect()->x + 1, myPlayer->getRect()->y + myPlayer->getRect()->h - 9, (int)(fireTick*25), 6, rozowy, rozowy);
			if (fireTick >= 2) {
				for (int i = 0; i < firedBalls; i++) {
					delete fireballs[i];
				}
				fireTick = 0;
				firedBalls = -1;
				fireDelay = false;
			}
		}

		if (immortalDelay) {
			immortalTick += delta;
			if (immortalTick >= 2) {
				immortalTick = 0;
				immortalDelay = false;
			}
		}

		showmessage();
		healthInfo();
		
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		if (myPlayer->getMoving()) {
			move();
		}
		if (1000 / fps > (SDL_GetTicks() - t2)) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - t2));
		}
		if (worldTime >= 180 || myPlayer->getHealth() == 0) {
			gameOver();
		}
	}
}

void game::level1() {

	enemyShoots();

	if ((int)worldTime % 4 == 0) {
		deleteBullets();
		initBullets();
	}

	enemy1->setPosition(200 - camera.x, 150 - camera.y);
	enemy1->show(screen);

	int scoreIncrease = 1;

	for (int i = 0; i < firedBalls; i++) {
		fireballs[i]->move();
		fireballs[i]->show(screen, camera.x, camera.y);
		if (collision(fireballs[i]->getRect(), enemy1->getRect())) {
			fireballs[i]->destroy();
			enemy1->setHealth();
			myPlayer->setScore(scoreIncrease);
			tempScore += scoreIncrease;
			scoreIncrease += 1;
		}
	}
	DrawRectangle(screen, enemy1->getRect()->x - 1, enemy1->getRect()->y + enemy1->getRect()->h - 10, enemy1->getRect()->w, 8, bialy, czarny);
	DrawRectangle(screen, enemy1->getRect()->x, enemy1->getRect()->y + enemy1->getRect()->h - 9, (int)(enemy1->getHealth()*enemy1->getRect()->w / 100), 6, rozowy, rozowy);
	if (enemy1->getHealth() == 0) {
		gameOver();
	}
}

void game::level2() {
	enemyShoots();

	if ((int)worldTime % shootSpeed == 4) {
		deleteBullets();
		initBullets();
	}

	enemy2->setPosition(200 - camera.x, 150 - camera.y);
	enemy2->show(screen);

	int scoreIncrease = 1;

	for (int i = 0; i < firedBalls; i++) {
		fireballs[i]->move();
		fireballs[i]->show(screen, camera.x, camera.y);
		if (collision(fireballs[i]->getRect(), enemy2->getRect())) {
			fireballs[i]->destroy();
			enemy2->setHealth();
			myPlayer->setScore(scoreIncrease);
			tempScore += scoreIncrease;
			scoreIncrease += 1;
		}
	}

	DrawRectangle(screen, enemy2->getRect()->x - 1, enemy2->getRect()->y + enemy2->getRect()->h - 10, enemy2->getRect()->w, 8, bialy, czarny);
	DrawRectangle(screen, enemy2->getRect()->x, enemy2->getRect()->y + enemy2->getRect()->h - 9, (int)(enemy2->getHealth() * enemy2->getRect()->w / 100), 6, rozowy, rozowy);
	if (enemy2->getHealth() == 0) {
		gameOver();
	}
}

void game::level3() {

	initBullets3();
	
	enemyShoots();

	if ((int)worldTime % shootSpeed == 4) {
		deleteBullets();
		initBullets();
	}

	enemy3->move();
	enemy3->setPosition(enemy3->getRect()->x - camera.x, 150 - camera.y);
	enemy3->show(screen);

	int scoreIncrease = 1;

	for (int i = 0; i < firedBalls; i++) {
		fireballs[i]->move();
		fireballs[i]->show(screen, camera.x, camera.y);
		if (collision(fireballs[i]->getRect(), enemy3->getRect())) {
			fireballs[i]->destroy();
			enemy3->setHealth();
			myPlayer->setScore(scoreIncrease);
			tempScore += scoreIncrease;
			scoreIncrease += 1;
		}
	}

	DrawRectangle(screen, enemy3->getRect()->x, enemy3->getRect()->y + enemy3->getRect()->h - 10, enemy3->getRect()->w, 8, bialy, czarny);
	DrawRectangle(screen, enemy3->getRect()->x, enemy3->getRect()->y + enemy3->getRect()->h - 9, (int)(enemy3->getHealth() * enemy3->getRect()->w / 100), 6, rozowy, rozowy);
	if (enemy3->getHealth() == 0) {
		gameOver();
	}
}

void game::gameOver() {
	delete myMenu;
	myMenu = new menu();
	if (worldTime >= 180) {
		myMenu->setMessage("Time is up! Game Over!");
		myMenu->setButtonMessage(0, "Try Again");
		myMenu->setButtonMessage(1, "Menu");
		menuVersion = 1;
		myPlayer->setScore(-tempScore);
	}
	else if (myPlayer->getHealth() == 0) {
		myMenu->setMessage("You lost! Game Over!");
		myMenu->setButtonMessage(0, "Try Again");
		myMenu->setButtonMessage(1, "Menu");
		menuVersion = 1;
		myPlayer->setScore(-tempScore);
	}
	else if (enemy1->getHealth() == 0 || enemy2->getHealth() == 0 || enemy3->getHealth() == 0) {
		myMenu->setMessage("Congratulation! You won!");
		if (level == 3) {
			myMenu->setButtonMessage(0, "Menu");
			myMenu->setButtonMessage(1, "Quit");
			menuVersion = 3;
			level = 1;
			delete myPlayer;
			myPlayer = new player(loadImage("char.bmp"));
		}
		else {
			myMenu->setButtonMessage(0, "Next Level");
			myMenu->setButtonMessage(1, "Menu");
			menuVersion = 2;
		}
	}
	if (exit) {
		running = false;
		menuRunning = false;
		return;
	}
	tempScore = 0;
	running = false;
	menuRunning = true;
}

void game::showmessage() {
	DrawRectangle(screen, 4, 25, 100, 20, bialy, czarny);
	sprintf(text, "%.1lf s", worldTime);
	DrawString(screen, 108 / 2 - strlen(text) * 8 / 2, 32, text, charset);

	DrawRectangle(screen, SCREEN_WIDTH-124, 25, 120, 20, bialy, czarny);
	sprintf(text, "Score -> %d", myPlayer->getScore());
	DrawString(screen, 530, 32, text, charset);

	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 18, bialy, czarny);
	sprintf(text, "Esc - exit, \030 - Up, \031 - Down, \032 - Left, \033 - Right, n - new game, m - menu");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
}

SDL_Surface* game::loadImage(const char* filename) {
	SDL_Surface* tmp = SDL_LoadBMP(filename);
	if (tmp == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", filename, SDL_GetError());
		SDL_FreeSurface(tmp);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};
	return tmp;
}

void game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) exit = true, gameOver();
				else if (event.key.keysym.sym == SDLK_UP) myPlayer->setDirection(0), myPlayer->setMoving(true);
				else if (event.key.keysym.sym == SDLK_RIGHT) myPlayer->setDirection(1), myPlayer->setMoving(true);
				else if (event.key.keysym.sym == SDLK_DOWN) myPlayer->setDirection(2), myPlayer->setMoving(true);
				else if (event.key.keysym.sym == SDLK_LEFT) myPlayer->setDirection(3), myPlayer->setMoving(true);
				else if (event.key.keysym.sym == SDLK_SPACE) shoot();
				else if (event.key.keysym.sym == SDLK_n) reset();
				else if (event.key.keysym.sym == SDLK_m) gameOver();
				break;
			case SDL_KEYUP:
				myPlayer->setMoving(false);
				break;
			case SDL_QUIT:
				exit = true;
				gameOver();
				break;
		};
	};
}

void game::reset() {
	delete enemy1;
	delete enemy2;
	delete enemy3;
	deleteBullets();
	worldTime = 0;
	camera.x = camera.y = 0;
	getBonus = false;

	myPlayer->reset();
	enemy1 = new enemy(loadImage("enemyA.bmp"), 200, 150);
	enemy2 = new enemy(loadImage("enemyB.bmp"), 200, 150);
	enemy3 = new enemy(loadImage("enemyC.bmp"), 200, 150);
	initBullets();
}


void game::move() {
	int direction = myPlayer->getDirection();
	int pos;
	int tempX = myPlayer->getPositionX(), tempY = myPlayer->getPositionY();
	if (direction == 0) {
		pos = myPlayer->getPositionY();
		pos -= speed;
		if (pos <= 40) {
			pos = 40;
			camera.y -= speed;
			if (camera.y <= 0) {
				camera.y = 0;
			}
		}
		myPlayer->setPositionY(pos);
	}
	else if (direction == 1) {
		pos = myPlayer->getPositionX();
		pos += speed;
		if (pos >= 590) {
			pos = 590;
			camera.x += speed;
			if (camera.x >= 2100 - SCREEN_WIDTH) {
				camera.x = 2100 - SCREEN_WIDTH;
			}
		}
		myPlayer->setPositionX(pos);
	}
	else if (direction == 2) {
		pos = myPlayer->getPositionY();
		pos += speed;
		if (pos >= 430) {
			pos = 430;
			camera.y += speed;
			if (camera.y >= 1400 - SCREEN_HEIGHT) {
				camera.y = 1400 - SCREEN_HEIGHT;
			}
		}
		myPlayer->setPositionY(pos);
	}
	else if (direction == 3) {
		pos = myPlayer->getPositionX();
		pos -= speed;
		if (pos <= 10) {
			pos = 10;
			camera.x -= speed;
			if (camera.x <= 0) {
				camera.x = 0;
			}
		}
		myPlayer->setPositionX(pos);
	}
	if (collision(myPlayer->getRect(), enemy1->getRect())) {
		myPlayer->setPositionX(tempX);
		myPlayer->setPositionY(tempY);
	}
}

void game::DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void game::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void game::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
  	*(Uint32*)p = color;
};

void game::DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void game::DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

bool game::collision(SDL_Rect* rec1, SDL_Rect* rec2){
	if (rec1->y >= rec2->y + rec2->h)
		return 0;
	if (rec1->x >= rec2->x + rec2->w)
		return 0;
	if (rec1->y + rec1->h <= rec2->y)
		return 0;
	if (rec1->x + rec1->w <= rec2->x)
		return 0;
	return 1;
}

void game::initBullets() {
	int angle = 0;
	int setX, setY;
	if (level == 1 || level == 2) setX = 278, setY = 200;
	else if (level == 3) setX = enemy3->getX() + 50, setY = 220;
	for (int i = 1; i <= 12; i++) {
		bulletsA[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle);
		bulletsB[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle + 3);
		bulletsC[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle + 6);
		bulletsD[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle + 9);
		angle += 15;
	}
}

void game::initBullets3() {
	int setX = enemy3->getX() + 50, setY = 220, angle = 0;

	if ((int)worldTime % shootSpeed == 0 || ((int)worldTime % (shootSpeed * 3) == 0)) {
		for (int i = 1; i <= 12; i++) {
			bulletsA[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle);
			angle += 15;
		}
	}
	else if ((int)worldTime % shootSpeed == 1 && (int)worldTime % (shootSpeed * 3) > 4 || ((int)worldTime % (shootSpeed * 3) == 0)) {
		for (int i = 1; i <= 12; i++) {
			bulletsB[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle);
			angle += 15;
		}
	}
	else if ((int)worldTime % shootSpeed == 2 && (int)worldTime % (shootSpeed * 3) > 4 || ((int)worldTime % (shootSpeed * 3) == 0)) {
		for (int i = 1; i <= 12; i++) {
			bulletsC[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle);
			angle += 15;
		}
	}
	else if ((int)worldTime % shootSpeed == 3 && (int)worldTime % (shootSpeed * 3) > 4 || ((int)worldTime % (shootSpeed * 3) == 0)) {
		for (int i = 1; i <= 12; i++) {
			bulletsD[i - 1] = new bullet(loadImage("fireball.bmp"), setX, setY, 5, angle);
			angle += 15;
		}
	}
}

void game::shoot() {
	if (firedBalls < 20) {
		firedBalls += 1;
		fireballs[firedBalls] = new fireball(loadImage("bullets.bmp"), myPlayer->getRect()->x + camera.x, myPlayer->getRect()->y + camera.y, 5, myPlayer->getDirection());
		for (int i = 0; i < firedBalls; i++) {
			fireballs[i]->move();
			fireballs[i]->show(screen, 0, 0);
		}
	}
	else {
		fireDelay = true;
	}
}

void game::enemyShoot(bullet* bullets[12]) {
	for (int i = 0; i < 12; i++) {
		bullets[i]->move();
		bullets[i]->show(screen, camera.x, camera.y);
		if (collision(bullets[i]->getRect(), myPlayer->getRect())) {
			bullets[i]->destroyBullet();
			if (!immortalDelay) {
				myPlayer->setHealth(-1);
				myPlayer->setScore(-5);
				immortalDelay = true;
			}
		}
	}
}

void game::enemyShoots() {

	if ((int)worldTime % shootSpeed >= 0 ||
		(level == 1 && (int)worldTime % 4 == 1)) 
	{
		enemyShoot(bulletsA);

		if ((int)worldTime % shootSpeed >= 1 && level == 2 ||
			(int)worldTime % shootSpeed >= 1 && level == 3 && (int)worldTime % (shootSpeed * 3) > 4 ||
			(level == 3 && (int)worldTime % (shootSpeed*3) == 0)) 
		{
			enemyShoot(bulletsB);

			if ((int)worldTime % shootSpeed >= 2 && level == 2 ||
				(int)worldTime % shootSpeed >= 2 && level == 3 && (int)worldTime % (shootSpeed * 3) > 4 ||
				(level == 3 && (int)worldTime % (shootSpeed * 3) == 0)) 
			{
				enemyShoot(bulletsC);

				if ((int)worldTime % shootSpeed >= 3 && level == 2 ||
					(int)worldTime % shootSpeed >= 3 && level == 3 && (int)worldTime % (shootSpeed * 3) > 4 ||
					(level == 3 && (int)worldTime % (shootSpeed * 3) == 0)) 
				{
					enemyShoot(bulletsD);
				}
			}
		}
	}
}

void game::deleteBullets() {
	for (int i = 0; i < 12; i++) {
		delete bulletsA[i];
		delete bulletsB[i];
		delete bulletsC[i];
		delete bulletsD[i];
	}
}

void game::healthInfo(){
	int healthX = 240, healthY = 45;

	for (int i = 0; i < myPlayer->getHealth(); i++) {
		DrawSurface(screen, health, healthX, healthY);
		healthX += 40;
	}
}

void game::randPosition() {
	bool correct = false;
	while (!correct) {
		correct = false;
		randX = rand() % 2100;
		randY = rand() % 1400;
		if (randX >= 910 && randX <= 2000 && randY >= 10 && randY <= 900) correct = true;
		if (randX >= 10 && randX <= 900 && randY >= 910 && randY <= 1300) correct = true;
	}
	
}

void game::bonus() {
	if (!getBonus) {
		SDL_Rect healthRect;
		healthRect.x = randX - camera.x;
		healthRect.y = randY - camera.y;
		healthRect.h = healthRect.w = 30;
		SDL_BlitSurface(health, NULL, screen, &healthRect);
		if (collision(&healthRect, myPlayer->getRect())) {
			myPlayer->setHealth(1);
			myPlayer->setScore(15);
			tempScore += 15;
			getBonus = true;
		}
	}
}