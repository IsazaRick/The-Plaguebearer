#include "ajikugames.h"
#include "DxLib.h"
#include<math.h>
#include <time.h>


void logo::DrawLogo(int& index, int& framecount, int& scenestate, int frame, int lenght) {
	if (!CheckSoundMem(logosound)) PlaySoundMem(logosound, DX_PLAYTYPE_BACK);
	framecount++;
	if (framecount >= 5) {
		index++;
		if (index >= 4) index = 0;
		framecount = 0;
	}
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1.5, 0, logo[index], true);
	if (frame > lenght * 2) scenestate++;
}

int heldFlag(int key)
{
	int VALUE;
	if (key) VALUE = TRUE;
	else VALUE = FALSE;
	return VALUE;
}

int buttonFlag(int key, int oldkey) {
	int flag = FALSE;
	if (key && !oldkey) flag = TRUE;
	else if (key == oldkey) flag = FALSE;

	return flag;
}

// Fade Functions
void FadeFromBlack(int& pal, int duration) {
	int faderate = 255 / duration;
	if (pal < 0) pal = 0;
	pal += faderate;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	if (pal >= 255) pal = 255;
}

void FadeToBlack(int& pal, int duration)
{
	int faderate = 255 / duration;
	if (pal >= 255) pal = 255; 
	pal -= faderate;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	if (pal < 0) pal = 0;
}


// Draw Functions

void DrawCamStatic(int image, int x, int y, int camx, int camy, int shakevar)
{
	DrawGraph(x - camx + shakevar, y - camy + shakevar, image, true);
}

void DrawStatic(int xpos, int ypos, int image)
{
	DrawRotaGraph(xpos, ypos, 1, 0, image, true);
}

void DrawReferencePoint(int x, int y, int camx, int camy, int size, int shakevar) {
	DrawCircle(x - camx + shakevar, y - camy + shakevar, size, GetColor(255, 100, 200), true);
}

void DrawColorPoint(int x, int y, int camx, int camy, int size, int red, int green, int blue, int shakevar) {
	DrawCircle(x - camx + shakevar, y - camy + shakevar, size, GetColor(red, green, blue), true);
}

void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY, int shakevar) {
	int x1 = x - camx, y1 = y - camy;
	DrawBox(x1 + shakevar, y1 + shakevar, x1 + sizeX + shakevar, y1 + sizeY + shakevar, GetColor(255, 255, 255), true);
}

void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, int shakevar) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes && loopflag) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag && index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag && index >= firstframe + ammountofframes) index = firstframe + ammountofframes - 1;
	}
	DrawRotaGraph(x - camX + shakevar, y - camY + shakevar, 1, 0, image[index], true);
}

void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle, int shakevar, int bounceX) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag) if (index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag) if (index >= firstframe + ammountofframes) index = firstframe + ammountofframes;
	}
	DrawRotaGraph(x - camX + shakevar, y - camY + shakevar - bounceX, 1, angle, image[index], true);
}

void DrawReferenceLine(int x1, int y1, int x2, int y2, int camx, int camy, int R, int G, int B, int size, int shakevar) {
	DrawLine(x1 - camx + shakevar, y1 - camy + shakevar, x2 - camx + shakevar, y2 - camy + shakevar, GetColor(R, G, B), size);
}

void DrawPercentBar(int startX, int startY, int endX, int endY, int camX, int camY, int min, int max, int current, int R, int G, int B, bool horizontalflag)
{
	float maxvalue = max - min;
	float percent = (current - min) / maxvalue;
	int size = 0;

	if (horizontalflag) {
		size = endX - startX;
		DrawBox(startX - camX, startY - camY, startX + size - camX, endY - camY, GetColor(80, 20, 60), true);
		DrawBox(startX - camX, startY - camY, startX + size * percent - camX, endY - camY, GetColor(R, G, B), true);
		DrawBox(startX - camX, startY - camY, startX + size - camX, endY - camY, GetColor(0, 0, 0), false);
		
	}
	else {
		int size = endY - startY;
		DrawBox(startX - camX, startY - camY, endX - camX, startY + size * percent - camY, GetColor(R, G, B), true);
		DrawBox(startX - camX, startY - camY, endX- camX, startY + size - camY, GetColor(0, 0, 0), false);
	}

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%d %d %f", size, current, percent);
}


// Hitbox Related Functions

int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize) {
	int flag = FALSE;
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	if (c <= playerSize + objSize) flag = TRUE;
	else flag = FALSE;

	return flag;
}

int checkDist(int playerX, int playerY, int objX, int objY) {
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	round(c);

	return c;
}

void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize) {
	int dist = checkDist(playerX, playerY, objX, objY);
	if (dist < playerSize + objSize) {
		int disp = playerSize + objSize - dist;
		if (objX >= playerX) playerX -= disp;
		if (objX <= playerX) playerX += disp;
		if (objY >= playerY) playerY -= disp;
		if (objY <= playerY) playerY += disp;
		// NOT PERFECT YET
	}
}

void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int boxcenterX = x + sizeX / 2;
	int boxcenterY = y + sizeY / 2;

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int horflag = FALSE;
	int verflag = FALSE;

	if (right >= x && left <= x2) horflag = TRUE;
	if (down >= y && up <= y2) verflag = TRUE;

	// Prev
	int prevup = prevY - playerSize;
	int prevdown = prevY + playerSize;
	int prevleft = prevX - playerSize;
	int prevright = prevX + playerSize;

	int prevhorflag = FALSE;
	int prevverflag = FALSE;

	if (prevright >= x && prevleft <= x2) prevhorflag = TRUE;
	if (prevdown >= y && prevup <= y2) prevverflag = TRUE;

	// Calculations
	if (horflag && verflag) {
		if (prevhorflag) playerY = prevY;
		else if (prevverflag) playerX = prevX;
	}
}

int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int horflag = FALSE;
	int verflag = FALSE;

	if (right >= x && left <= x2) horflag = TRUE;
	if (down >= y && up <= y2) verflag = TRUE;

	int flag = FALSE;
	if (horflag && verflag) flag = TRUE;
	return flag;
}


// Scripts

void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed) {
	if (x < chaseX) x += speed;
	else if (x > chaseX) x -= speed;
	if (y < chaseY) y += speed;
	else if (y > chaseY) y -= speed;
}

void SimpleRunAwayScript(int& x, int& y, int runX, int runY, int speed) {
	if (x < runX) x -= speed;
	else if (x > runX) x += speed;
	if (y < runY) y -= speed;
	else if (y > runY) y += speed;
}

// Essentials
void MoveCamera(int& camX, int& camY, int followX, int followY, int MAXcamX, int MAXcamY) {

	camX = followX - WIN_WIDTH / 2;
	camY = followY - WIN_HEIGHT / 2;

	
	if (camX >= MAXcamX) camX = MAXcamX;
	else if (camX <= 0) camX = 0;

	if (camY >= MAXcamY) camY = MAXcamY;
	else if (camY <= 0) camY = 0;

}

void Shake(int deviation[], int& shakevar, int& downcount) {
	downcount--;
	if (downcount <= 0) downcount = 0;
	shakevar = deviation[downcount];
}


