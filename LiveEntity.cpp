#include "LiveEntity.h"
#include<math.h>
#include <stdlib.h>
#include <time.h>
#include "ajikugames.h"

void LiveEntity::AccMov(int& x, int& y, int& accX, int& accY, int up, int down, int left, int right) {

	if (up && !down) accY -= accelerationSpeed;
	else if (down && !up) accY += accelerationSpeed;

	if (!up && !down) {
		if (accY < 0) accY += accelerationSpeed;
		else if (accY > 0) accY -= accelerationSpeed;
	}

	if (left && !right) accX -= accelerationSpeed;
	else if (right && !left) accX += accelerationSpeed;

	if (!left && !right) {
		if (accX < 0) accX += accelerationSpeed;
		else if (accX > 0) accX -= accelerationSpeed;
	}

	if (accX >= maxAccX) accX = maxAccX;
	if (accX <= -maxAccX) accX = -maxAccX;
	if (accY >= maxAccY) accY = maxAccY;
	if (accY <= -maxAccY) accY = -maxAccY;

	x += accX;
	y += accY;
}

// NPC Behavior
void LiveEntity::IdleWithPace(int& x, int& y, int& state, int& statetimer, int& dir) {
	statetimer++;
	enum states {
		idling,
		pacing,
	};

	enum direction {
		none,
		up,
		down,
		left,
		right,
	};

	switch (state) {
	case idling:
		if (statetimer >= rand() % (dir * 10) + 110) {
			statetimer = 0;
			state = pacing;
			dir = rand() % 4 + 1;
		}
		break;

	case pacing:
		switch (dir) {
		case up:
			y -= flatSpeed;
			break;
		case down:
			y += flatSpeed;
			break;
		case left:
			x -= flatSpeed;
			break;
		case right:
			x += flatSpeed;
			break;
		}

		if (statetimer >= rand() % (dir * 10) + 57) {
			statetimer = 0;
			state = idling;
		}
		break;

	}
}
void LiveEntity::PaceDiagonal(int& x, int& y, int& state, int& statetimer, int& dir) {
	statetimer++;
	enum states {
		idling,
		walking,
	};

	enum direction {
		none,
		upleft,
		downright,
		downleft,
		upright,
	};

	switch (state) {
	case idling:
		if (statetimer >= rand() % (dir * 10) + 33) {
			statetimer = 0;
			state = walking;
			dir = rand() % 4 + 1;
		}
		break;

	case walking:
		switch (dir) {
		case upleft:
			y -= flatSpeed;
			x -= flatSpeed;
			break;
		case downright:
			y += flatSpeed;
			x += flatSpeed;
			break;
		case downleft:
			y += flatSpeed;
			x -= flatSpeed;
			break;
		case upright:
			y -= flatSpeed;
			x += flatSpeed;
			break;
		}

		if (statetimer >= rand() % (dir * 10) + 125) {
			statetimer = 0;
			state = idling;
		}
		break;

	}
}
void LiveEntity::WalkDiagonal(int& x, int& y, int& state, int& statetimer, int& dir) {
	statetimer++;

	enum direction {
		none,
		upleft,
		downright,
		downleft,
		upright,
	};

	switch (dir) {
	case upleft:
		y -= flatSpeed;
		x -= flatSpeed;
		break;
	case downright:
		y += flatSpeed;
		x += flatSpeed;
		break;
	case downleft:
		y += flatSpeed;
		x -= flatSpeed;
		break;
	case upright:
		y -= flatSpeed;
		x += flatSpeed;
		break;
	}

		if (statetimer >= rand() % (dir * 10) + 114) {
			statetimer = 0;
			dir = rand() % 4 + 1;
		}
}
void LiveEntity::WalkMixed(int& x, int& y, int& state, int& statetimer, int& dir) {
	statetimer++;
	enum states {
		idling,
		walking,
	};

	enum direction {
		none,
		upleft,
		downright,
		downleft,
		upright,
		up,
		down,
		left,
		right,
	};

	switch (state) {
	case idling:
		if (statetimer >= rand() % (dir * 10) + 33) {
			statetimer = 0;
			state = walking;
			dir = rand() % 8 + 1;
		}
		break;

	case walking:
		switch (dir) {
		case upleft:
			y -= flatSpeed;
			x -= flatSpeed;
			break;
		case downright:
			y += flatSpeed;
			x += flatSpeed;
			break;
		case downleft:
			y += flatSpeed;
			x -= flatSpeed;
			break;
		case upright:
			y -= flatSpeed;
			x += flatSpeed;
			break;
		case up:
			y -= flatSpeed;
			break;
		case down:
			y += flatSpeed;
			break;
		case left:
			x -= flatSpeed;
			break;
		case right:
			x += flatSpeed;
			break;
		}

		if (statetimer >= rand() % (dir * 10) + 125) {
			statetimer = 0;
			state = idling;
		}

	}
}
void LiveEntity::WalkMixedPlus(int& x, int& y, int& state, int& statetimer, int& dir) {
	statetimer++;
	enum states {
		idling,
		walking,
	};

	enum direction {
		none,
		upleft,
		downright,
		downleft,
		upright,
		up,
		down,
		left,
		right,
	};

	switch (state) {
	case idling:
		if (statetimer >= rand() % (dir * 10) + 33) {
			statetimer = 0;
			state = walking;
			dir = rand() % 8 + 1;
		}
		break;

	case walking:
		switch (dir) {
		case upleft:
			y -= flatSpeed + 1;
			x -= flatSpeed;
			break;
		case downright:
			y += flatSpeed;
			x += flatSpeed + 1;
			break;
		case downleft:
			y += flatSpeed + 1;
			x -= flatSpeed;
			break;
		case upright:
			y -= flatSpeed;
			x += flatSpeed + 1;
			break;
		case up:
			y -= flatSpeed;
			x += flatSpeed / 2;
			break;
		case down:
			y += flatSpeed;
			x -= flatSpeed / 2;
			break;
		case left:
			x -= flatSpeed;
			y -= flatSpeed / 2;
			break;
		case right:
			x += flatSpeed;
			y += flatSpeed / 2;
			break;
		}

		if (statetimer >= rand() % (dir * 10) + 125) {
			statetimer = 0;
			state = idling;
		}

	}
}

void RandomizeSpawns(int& x, int& y) {
	x = rand() % WORLD_WIDTH;
	y = rand() % (WORLD_HEIGHT - 128) + 128;
}