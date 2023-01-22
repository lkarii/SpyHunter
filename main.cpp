#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <cstdlib>
#include <time.h>
#include <Windows.h>
#include "Background.h"
#include "Bonus.h"
#include "Bullet.h"
#include "Car.h"
#include "CollidedObject.h"
#include "Enemy.h"
#include "Neutral.h"
#include "ObjectGenerator.h"
#include "Texture.h"


extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	700
#define SCREEN_HEIGHT	750
#define CAR_WIDTH  50
#define CAR_HEIGHT  76
#define NUM_OF_ENEMYS 100
#define SPACE_BW_CARS 550
#define NUMBER_OF_ENEMYS 100
#define BACKGROUND_SCROLL_SPEED 2
#define MARGIN 10
#define BONUS_FOR_COLLISION_WITH_CAR 10
#define BONUS_FOR_KILLED_CAR 50
#define BULLET_SPEED 5
#define BULLET_WIDTH 16
#define BULLET_HEIGHT 28
#define CAR_SPEED 3
#define DISTANCE_BETWEEN_BONUSES 1000
#define BASE_ENEMY_SPEED 2
#define BASE_NOT_ENEMY_SPEED 2



void DrawString(SDL_Surface* screen, int x, int y, const char* text,
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
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


void saveGame(Car* car, Enemy* enemy, Neutral* notEnemy, Bullet* carBullet, Bullet* enemyBullet,
	Bonus* carBonus, Bonus* weaponBonus, double* worldTime, double* delta,
	double* distance, int* gameState)
{
	HANDLE hFile;
	DWORD dwBytesWritten;
	char filename[256];
	SYSTEMTIME st;
	GetLocalTime(&st);

	sprintf_s(filename, "savegame_%04d-%02d-%02d_%02d-%02d-%02d.bin", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Error opening file!\n");
		return;
	}

	WriteFile(hFile, car, sizeof(Car), &dwBytesWritten, NULL);
	WriteFile(hFile, enemy, sizeof(Enemy), &dwBytesWritten, NULL);
	WriteFile(hFile, notEnemy, sizeof(Neutral), &dwBytesWritten, NULL);
	WriteFile(hFile, carBullet, sizeof(Bullet), &dwBytesWritten, NULL);
	WriteFile(hFile, enemyBullet, sizeof(Bullet), &dwBytesWritten, NULL);
	WriteFile(hFile, carBonus, sizeof(Bonus), &dwBytesWritten, NULL);
	WriteFile(hFile, weaponBonus, sizeof(Bonus), &dwBytesWritten, NULL);
	WriteFile(hFile, worldTime, sizeof(double), &dwBytesWritten, NULL);
	WriteFile(hFile, delta, sizeof(double), &dwBytesWritten, NULL);
	WriteFile(hFile, distance, sizeof(double), &dwBytesWritten, NULL);
	WriteFile(hFile, gameState, sizeof(int), &dwBytesWritten, NULL);

	CloseHandle(hFile);

	printf("Game saved successfully!\n");
}

void loadGame(Car* car, Enemy* enemy, Neutral* notEnemy, Bullet* carBullet, Bullet* enemybullet, Bonus* carBonus, Bonus* weaponBonus, double* worldTime, double* delta,
	double* distance, int* gameState)
{
	HANDLE hFind;
	WIN32_FIND_DATAA fdFile;
	char sPath[256];
	int i = 0;
	char** files = NULL;
	int choice;

	sprintf_s(sPath, "saves\\*.bin");

	hFind = FindFirstFileA(sPath, &fdFile);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Error opening directory!\n");
		return;
	}

	do {
		if (!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			files = (char**)realloc(files, sizeof(char*) * (i + 1));
			files[i] = strdup(fdFile.cFileName);
			i++;
		}
	} while (FindNextFileA(hFind, &fdFile));

	FindClose(hFind);

	if (i == 0) {
		printf("No save files found!\n");
		return;
	}

	// выводим список сохраненных игр
	printf("Select a save file:\n");
	for (int j = 0; j < i; j++) {
		printf("%d) %s\n", j + 1, files[j]);
	}

	// получаем выбор пользователя
	scanf("%d", &choice);
	if (choice < 1 || choice > i) {
		printf("Invalid choice!\n");
		return;
	}

	HANDLE hFile = CreateFileA(files[choice - 1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("Error opening file!\n");
		return;
	}

	DWORD dwBytesRead;
	ReadFile(hFile, car, sizeof(Car), &dwBytesRead, NULL);
	ReadFile(hFile, enemy, sizeof(Enemy), &dwBytesRead, NULL);
	ReadFile(hFile, notEnemy, sizeof(Neutral), &dwBytesRead, NULL);
	ReadFile(hFile, carBullet, sizeof(Bullet), &dwBytesRead, NULL);
	ReadFile(hFile, enemybullet, sizeof(Bullet), &dwBytesRead, NULL);
	ReadFile(hFile, carBonus, sizeof(Bonus), &dwBytesRead, NULL);
	ReadFile(hFile, weaponBonus, sizeof(Bonus), &dwBytesRead, NULL);
	ReadFile(hFile, worldTime, sizeof(double), &dwBytesRead, NULL);
	ReadFile(hFile, delta, sizeof(double), &dwBytesRead, NULL);
	ReadFile(hFile, distance, sizeof(double), &dwBytesRead, NULL);
	ReadFile(hFile, gameState, sizeof(int), &dwBytesRead, NULL);
	CloseHandle(hFile);
	printf("Game loaded successfully!\n");
}



void newGame(Car* car, Enemy* enemy, Neutral* notEnemy, Bullet* carBullet, Bullet* enemyBullet, Bonus* carBonus, Bonus* weaponBonus, double* worldTime, double* delta, double* distance, int* gameState)
{
	car->carX = SCREEN_WIDTH / 2.5; car->carY = SCREEN_HEIGHT - 4 * CAR_HEIGHT; car->carLives = 2; car->carSpeedX = 0; car->carSpeedY = 0; car->score = 0;
	enemy->enemyX = ObjectGenerator::getRandomXPosition(); enemy->enemyY = -80; enemy->enemySpeedX = BASE_ENEMY_SPEED; enemy->enemySpeedY = BASE_ENEMY_SPEED;
	notEnemy->notEnemyX = ObjectGenerator::getRandomXPosition(); notEnemy->notEnemyY = -80; notEnemy->notEnemySpeedX = 0; notEnemy->notEnemySpeedY = BASE_NOT_ENEMY_SPEED;
	carBullet->bulletX = car->getX();
	enemyBullet->bulletX = enemy->getPosX();
	carBonus->bonusX = ObjectGenerator::getRandomXPosition(); carBonus->bonusY = -100; weaponBonus->bonusX = ObjectGenerator::getRandomXPosition(); weaponBonus->bonusY = -200;
	*worldTime = 0;
	*delta = 0;
	*distance = 0;
	*gameState = 1;
}

void playGame(Car* car, Enemy* enemy, Neutral* notEnemy, Bonus* carBonus, Bonus* weaponBonus, Background* background,
	Bullet* carBullet, Bullet* enemyBullet, double* worldTime, double* delta, double* distance, SDL_Renderer* renderer)
{
	*worldTime += *delta;
	if (car->canScored)
	{
		*distance += background->getSpeed();
	}

	car->move();
	enemy->move();
	notEnemy->move();
	carBonus->move();
	weaponBonus->move();
	carBullet->move();
	if (carBullet->active)
	{
		carBullet->shoot(*car, *weaponBonus);
	}
	enemyBullet->moveEnemyBullet();
	int index = rand() % 150;
	if (index % 19 == 0)
	{
		enemyBullet->enemyShoot(*enemy);
	}
	background->scroll();
	background->render(renderer);
}


void endOfGame(SDL_Renderer *renderer, Car car)
{
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	charset = SDL_LoadBMP("./cs8x8.bmp");

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetColorKey(charset, true, 0x000000);
	char text[128];

	SDL_Rect infoRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_GetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &infoRect);
	sprintf(text, "G A M E  \n  O V E R");
	DrawString(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, text, charset);

	sprintf(text, "Your score: %i ", car.getScore());
	DrawString(screen, 10, 25, text, charset);
	SDL_UpdateTexture(scrtex, &infoRect, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, &infoRect, &infoRect);

	SDL_RenderPresent(renderer);
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

bool checkFrontCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

bool checkSideCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

int ObjectGenerator::lastPosition = -1;

void close(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Window* Window, Texture gBGTexture, Texture carTexture, Texture enemyTexture, Texture notEnemyTexture, Texture bulletTexture, 
	Texture bonusCarTexture, Texture bonusWeaponTexture, Texture bulletEnemyTexture)
{
	gBGTexture.free();
	carTexture.free();
	enemyTexture.free();
	notEnemyTexture.free();
	bulletTexture.free();
	bonusCarTexture.free();
	bonusWeaponTexture.free();
	bulletEnemyTexture.free();
	//SDL_FreeSurface(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(Window);
	Window = NULL;
	renderer = NULL;

	SDL_Quit();
}

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv) {
#pragma region init
	int t1, t2, quit, frames, rc;
	int bgX = 0, bgY = SCREEN_HEIGHT;
	int carX = 0;
	int carY = 0;
	int carSpeedX = 0;
	int carSpeedY = 0;
	int carLives = 2;
	int enemyX = SCREEN_WIDTH / 10;
	int enemyY = SCREEN_HEIGHT;
	int enemySpeedX = 0;
	int enemySpeedY = 2;
	int notEnemyX = 0;
	int notEnemyY = 0;
	int score = 0;
	int bulletX = -100;
	int bulletY = -100;
	bool isPause = false;
	bool isKeyPPressed = false;
	double delta, worldTime, fpsTimer, fps, BgSpeed, enemySpeed;
	int gamePaused = 0;
	int gameState = 1; //1-игра 2-конец игры, игрок убит
	double distance = 0;
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	BgSpeed = 70;
	enemySpeed = 70;

	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* Window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event e;
	Car car;
	Enemy enemy;
	Neutral notEnemy;
	Bullet carBullet(car);
	Bullet enemyBullet(car);
	Bonus carBonus = Bonus(1);
	Bonus weaponBonus = Bonus(1);
	Texture carTexture, bgTexture, enemyTexture, notEnemyTexture, bulletTexture, bonusCarTexture, bonusWeaponTexture, bulletEnemyTexture;


#pragma endregion

	// okno konsoli nie jest widoczne, jeżeli chcemy zobaczyć
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmienić na "Console"
	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

#pragma region init window
	Window = SDL_CreateWindow("Karyna Lukashova 187714", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

	}
	renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	}

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

#pragma endregion

#pragma region load textures
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		return 1;
	};
	if (!bgTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/back.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!carTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/mainCar.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!enemyTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/blue_enemy_car.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!notEnemyTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/not_enemy.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!bulletTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/bullet.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!bonusCarTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/bonus_car.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!bonusWeaponTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/bonus_weapon.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
	if (!bulletEnemyTexture.CreateFromSprite(renderer, SDL_LoadBMP("./img/enemy_bullet.bmp")))
	{
		printf("Failed to load background texture!\n");
	}
#pragma endregion

	SDL_RenderClear(renderer);
	Background* background = new Background(&bgTexture);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetColorKey(charset, true, 0x000000);
	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	t1 = SDL_GetTicks();

#pragma region mainloop

	while (gameState != 3)
	{
		while (gameState==1)
		{
			t2 = SDL_GetTicks();

			delta = (t2 - t1) * 0.001;
			t1 = t2;
			SDL_FillRect(screen, NULL, czarny);
			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};


			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					gameState = 3;
					quit = 1;
					close(renderer, screen, Window, bgTexture, carTexture, enemyTexture, notEnemyTexture, bulletTexture, bonusCarTexture, bonusWeaponTexture, bulletEnemyTexture);
				} break;
				case SDL_QUIT: gameState = 3; quit = 1; break;
				};
				enemy.handleEvent(event);
				car.handleEvent(event);
				notEnemy.handleEvent(event);
				carBullet.handleEvent(event, car, weaponBonus);
				background->handleEvent(event);
			};
			const Uint8* keystates = SDL_GetKeyboardState(0);
			if (keystates[SDL_SCANCODE_N]) {
				newGame(&car, &enemy, &notEnemy, &carBullet, &enemyBullet, &carBonus, &weaponBonus, &worldTime, &delta, &distance, &gameState);
			}
			if (keystates[SDL_SCANCODE_SPACE]) {
				carBullet.bulletX = car.carX;
			}
			if (keystates[SDL_SCANCODE_P]) {
				if (!isKeyPPressed)
				{
					isPause = !isPause;
					isKeyPPressed = true;
				}
			}
			else
			{
				isKeyPPressed = false;
			}
			if (isPause == false)
			{
				playGame(&car, &enemy, &notEnemy, &carBonus, &weaponBonus, background, &carBullet, &enemyBullet, &worldTime, &delta, &distance, renderer);
			}
			if (keystates[SDL_SCANCODE_S]) {
				saveGame(&car, &enemy, &notEnemy, &carBullet, &enemyBullet, &carBonus, &weaponBonus, &worldTime, &delta, &distance, &gameState);
			}
			if (keystates[SDL_SCANCODE_L]) {
				loadGame(&car, &enemy, &notEnemy, &carBullet, &enemyBullet, &carBonus, &weaponBonus, &worldTime, &delta, &distance, &gameState);
			}


			carBonus.render(renderer, &bonusCarTexture);
			weaponBonus.render(renderer, &bonusWeaponTexture);
			enemy.render(renderer, &enemyTexture);
			notEnemy.render(renderer, &notEnemyTexture);

			car.checkInterval();
			if ((car.alive == false) && (car.carLives > 0))
			{
				car.render(renderer, &carTexture);
			}
			else if (car.alive == true)
			{
				car.render(renderer, &carTexture);
			}
			else if ((car.alive == false) && (car.carLives = 0))
			{
				endOfGame(renderer, car);
			}
			carBullet.render(renderer, &bulletTexture, car.getX(), car.getY());
			if (enemyBullet.active)
			{
				enemyBullet.render(renderer, &bulletEnemyTexture, enemy.getPosX(), enemy.getPosY());
			}


			SDL_Rect infoRect = { 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50 };
			SDL_GetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderFillRect(renderer, &infoRect);
			sprintf(text, "Lives: %u distance: %.1lf time: %.1lf s  fps: %.0lf klatek / s", car.carLives, distance / 100., worldTime, fps);
			DrawString(screen, 10, 10, text, charset);
			sprintf(text, "Score: %i ", car.getScore());
			DrawString(screen, 10, 25, text, charset);
			sprintf(text, "a b c d e f");
			DrawString(screen, 590, 20, text, charset);
			sprintf(text, "g i j k l m n");
			DrawString(screen, 590, 30, text, charset);
			SDL_UpdateTexture(scrtex, &infoRect, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, &infoRect, &infoRect);

			SDL_RenderPresent(renderer);
#pragma region COLLISION

			if (checkFrontCollision(enemy.collider, carBullet.collider))
			{
				if (carBullet.active)
				{
					if (car.canScored)
					{
						car.score += BONUS_FOR_KILLED_CAR;
					}
					enemy.killed();
					carBullet.active = false;
					enemyBullet.active = false;
				}
			}

			if (checkSideCollision(car.collider, enemy.collider))
			{
				if (car.canScored)
				{
					car.score += BONUS_FOR_COLLISION_WITH_CAR;
				}
				enemy.collisionWithPlayer();
			}

			if (checkSideCollision(car.collider, notEnemy.collider))
			{
				notEnemy.collisionWithPlayer();
				car.collisionWithNeutral();
			}

			if (checkFrontCollision(notEnemy.collider, carBullet.collider))
			{
				notEnemy.killed();
				carBullet.active = false;
				quit = 1;
			}

			if (checkCollision(car.collider, enemyBullet.collider))
			{
				if (enemyBullet.active)
				{
					enemyBullet.active = false;
					enemyBullet.index = -1;
					if (car.carLives > 0)
					{
						car.carLives--;
					}
					if (car.carLives == 0)
					{
						car.killed();
						gameState = 2;
					}
				}
			}

			if (checkCollision(carBonus.collider, car.collider))
			{

				if (carBonus.isActive())
				{
					car.carLives += 1;
					carBonus.taken();
				}
			}

			if (checkCollision(weaponBonus.collider, car.collider))
			{
				if (weaponBonus.isActive())
				{
					weaponBonus.taken();
				}
			}


#pragma endregion

			frames++;
		}
		while (gameState == 2)
		{
			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					gameState = 3;
					quit = 1;
					close(renderer, screen, Window, bgTexture, carTexture, enemyTexture, notEnemyTexture, bulletTexture, bonusCarTexture, bonusWeaponTexture, bulletEnemyTexture);
				} break;
				case SDL_QUIT: gameState = 3; quit = 1; break;
				};
			};
			const Uint8* keystates = SDL_GetKeyboardState(0);
			if (keystates[SDL_SCANCODE_N]) {
				newGame(&car, &enemy, &notEnemy, &carBullet, &enemyBullet, &carBonus, &weaponBonus, &worldTime, &delta, &distance, &gameState);
			}
			endOfGame(renderer, car);
		}
	}

#pragma endregion



	SDL_FreeSurface(charset);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(Window);
	close(renderer, screen, Window, bgTexture, carTexture, enemyTexture, notEnemyTexture, bulletTexture, bonusCarTexture, bonusWeaponTexture, bulletEnemyTexture);
	SDL_Quit();

	return 0;
};
