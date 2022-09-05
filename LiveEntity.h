#pragma once
#include <stdlib.h>
class LiveEntity
{
public:

	int x = 500, y = 500;
	int startX = 0, startY = 0, endX = 0, endY = 0;
	int px, py;
	int accX = 0, accY = 0;
	int maxAccX = 6, maxAccY = 6;
	int accelerationSpeed = 1;
	int state = 0, statetimer = 0;
	int substate = 0;
	int dir = 1;
	int direction = 1;
	
	int behavior = 0;
	int type = rand() % 5;

	int flatSpeed = rand() % 2 + 1;

	void AccMov(int& x, int& y, int& accX, int& accY, int up, int down, int left, int right);
	
	// NPC Behavior
	void IdleWithPace(int& x, int& y, int& state, int& statetimer, int& dir);
	void PaceDiagonal(int& x, int& y, int& state, int& statetimer, int& dir);
	void WalkDiagonal(int& x, int& y, int& state, int& statetimer, int& dir);
	void WalkMixed(int& x, int& y, int& state, int& statetimer, int& dir);
	void WalkMixedPlus(int& x, int& y, int& state, int& statetimer, int& dir);
};

void RandomizeSpawns(int& x, int& y);