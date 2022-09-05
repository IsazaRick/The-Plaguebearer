#pragma once

const int WIN_WIDTH = 1080;
const int WIN_HEIGHT = 620;

const int WORLD_WIDTH = 1728;
const int WORLD_HEIGHT = 896;

const int TILE_VERTICAL_AMMOUNT = WIN_HEIGHT / 32;
const int TILE_HORIZONTAL_AMMOUNT = WIN_WIDTH / 32;

const int TILE_SIZE = 32;

const enum directions {
	up,
	down,
	left,
	right,
	up_left,
	up_right,
	down_left,
	down_right,
};

class logo
{
public:
	int logo[4];
	int index;
	int framecount;
	int logosound;

	void DrawLogo(int& index, int& framecount, int& scenestate, int frame, int lenght);
};

int heldFlag(int key);
int buttonFlag(int key, int oldkey);

// Fade Functions
void FadeFromBlack(int& pal, int duration);
void FadeToBlack(int& pal, int duration);

// Draw Functions
void DrawCamStatic(int image, int x, int y, int camx, int camy, int shakevar);
void DrawStatic(int xpos, int ypos, int image);
void DrawReferencePoint(int x, int y, int camx, int camy, int size, int shakevar);
void DrawColorPoint(int x, int y, int camx, int camy, int size, int red, int green, int blue, int shakevar);
void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY, int shakevar);
void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, int shakevar);
void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle, int shakevar, int bounceX);
void DrawReferenceLine(int x1, int y1, int x2, int y2, int camx, int camy, int R, int G, int B, int size, int shakevar);
void DrawPercentBar(int startX, int startY, int endX, int endY, int camX, int camY, int min, int max, int current, int R, int G, int B, bool horizontalflag);

// Hitbox Related Functions
int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize);
int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize);
int checkDist(int playerX, int playerY, int objX, int objY);
void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize);
void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY);


// Scripts
void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed);
void SimpleRunAwayScript(int& x, int& y, int runX, int runY, int speed);

// Essentials
void MoveCamera(int& camX, int& camY, int followX, int followY, int MAXcamX, int MAXcamY);
void Shake(int deviation[], int& shakevar, int& downcount);