#include "DxLib.h"
#include "ajikugames.h"
#include "LiveEntity.h"
#include <stdlib.h>
#include <time.h>

const char TITLE[] = "AjikuGames: Template";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32); // 画面サイズを設定(解像度との比率で設定)

	SetWindowSizeExtendRate(1.5);

	SetBackgroundColor(0x00, 0x00, 0x00);	// 画面の背景色を設定するx
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);
	// ESSENTIAL VARIABLES
	int frame = 0, pal = 0;
	int fadefromblackflag = FALSE, fadetoblackflag = FALSE;
	int shakedev[8] = { 4, 4, -4, -4, 2, 2, -2, -2 };
	int shakevar = 0, shakecount = 0, shakeflag = FALSE;
	srand(time(NULL));
	// AJKGames Variables
	logo AJKGames;
	AJKGames.framecount = 0, AJKGames.index = 0, AJKGames.logosound = LoadSoundMem("ajikugamessound.wav");
	LoadDivGraph("ajikugameslogo.png", 4, 2, 2, 640, 256, AJKGames.logo, true);

	// Player Related
	const int HumanLimit = 20;
	LiveEntity Human[HumanLimit];
	bool isPlayerFlag[HumanLimit] = { FALSE };
	bool isInfectedFlag[HumanLimit] = { FALSE };
	isPlayerFlag[0] = TRUE;
	isInfectedFlag[0] = TRUE;
	int currPlayer = 0;
	int currDoc = HumanLimit - 1;
	int infectedCount = 1;
	int surroundCount[HumanLimit] = { 0 };

	// DOC
	int docHealth = 1000;

	// Images
	int playerIMG[6][60];
	LoadDivGraph("Resource\\Characters\\girl.png", 59, 8, 8, 64, 64, playerIMG[0], true);
	LoadDivGraph("Resource\\Characters\\boy.png", 59, 8, 8, 64, 64, playerIMG[1], true);
	LoadDivGraph("Resource\\Characters\\woman.png", 59, 8, 8, 64, 64, playerIMG[2], true);
	LoadDivGraph("Resource\\Characters\\man.png", 59, 8, 7, 64, 64, playerIMG[3], true);
	LoadDivGraph("Resource\\Characters\\woman.png", 59, 8, 8, 64, 64, playerIMG[4], true);
	LoadDivGraph("Resource\\Characters\\doctor_sheet.png", 36, 8, 4, 64, 64, playerIMG[5], true);
	int virus = LoadGraph("Resource\\virus.png", true);
	int characterIndex[HumanLimit] = { 0 }, characterFramecount[HumanLimit] = { 0 };
	int characterFramerate = 8;

	int engineLOGO = LoadGraph("Dandy-OEngine.png", true);

	int arrow = LoadGraph("Resource\\arrow.png");
	int arrowSmall[4];
	int arrowIndex[HumanLimit] = { 0 }, arrowFramecount[HumanLimit] = { 0 };
	LoadDivGraph("Resource\\arrow_small.png", 4, 4, 1, 64, 64, arrowSmall, true);
	int vialIMG = LoadGraph("Resource\\vial.png");
	int vialexplodedIMG = LoadGraph("Resource\\vial_exploded.png");
	int mapchip[32];
	LoadDivGraph("Resource\\tilesheet.png", 12, 4, 3, 64, 64, mapchip, true);


	// Sneeze Variables
	bool sneezeflag[HumanLimit] = { FALSE };
	int sneezeDuration = 11;
	int sneezeCounter[HumanLimit] = { 0 };
	int sneezeRadius = 60;
	bool sneezingflag[HumanLimit] = { FALSE };
	bool sneezedflag[HumanLimit] = { FALSE };

	bool attackflag = FALSE;

	// Vials
	LiveEntity Vial;
	int vialtimer = 0;
	const int vialtimerMAX = 60;
	float vialangle = 0;

	// Follow
	bool followFlag = FALSE;

	// Cam
	int camX = 0, camY = 0;

	// Ayashii Counter
	int susCount = 0;
	int susMAX = 350;
	int susRadius = sneezeRadius * 3;
	int susCountSubCount = 0;

	enum behaviorstates {
		idle,
		pacediagonal,
		walkdiagonal,
		mixed,
		mixedplus,
	};
	const enum scenstates {
		openinglogo,
		enginelogo,
		reset,
		load,
		gameplay,
		win,
		outofpeople,
		overused,
	};
	enum vialstates {
		inactive,
		thrown,
		exploded,
	};
	int vialstate = inactive;

	int scenestate = 0; // CHANGE THIS FOR DEBUGGING

	// Spider Virus
	int spiderX[HumanLimit],
		spiderY[HumanLimit],
		spiderIMG = LoadGraph("Resource\\spider.png", true);

	int controls = LoadGraph("Resource\\controls.png", true);

	// Keybind Flags
	int upflag = FALSE, downflag = FALSE, leftflag = FALSE, rightflag = FALSE;

	char key[256] = { 0 };
	char prevkey[256] = { 0 };

	int floormap[19][27] = {
		{ 9, 10, 10, 10, 10, 3, 10, 10, 10, 10, 10, 3, 10, 10, 10, 10, 10, 10, 3, 10, 10, 10, 10, 3, 10, 10, 11, },
		{ 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },
	};

	int floordets[19][27] = {
		{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,15, 15, 15, 15, 15, 15, 15,15, 15, 15, 15, 15, 15, },
		{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,15, 15, 15, 15, 15, 15, 15,15, 15, 15, 15, 15, 15, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 7, 8, 8, 8, 7, 8, 8, 8, 8, 7, 8, 8, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 7, 8, 8, 8, 6, 0, 0, 0, 0, 5, 8, 8, 8, 6, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, },
		{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 8, 8, 7, 8, 8, 7, 8, 8, 8, 8, 6, },
		{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 5, 8, 8, 8, 7, 8, 8, 7, 8, 8, 8, 8, 6, },
	};

	int Key = 0;
	int oldKey = 0;

	// Sounds
	int glass = LoadSoundMem("Resource\\glass.wav");
	int throwSound = LoadSoundMem("Resource\\throw.wav");
	int music = LoadSoundMem("Resource\\.wav");
	int sneezeSound = LoadSoundMem("Resource\\sneeze.wav");
	int bgm = LoadSoundMem("Resource\\bgm.mp3");

	// Bounce
	int bounce[HumanLimit] = { 0 };
	bool bouncestate[HumanLimit] = { FALSE };

	float bounceangle[HumanLimit] = { 0 };
	bool bounceanglestate[HumanLimit] = { FALSE };

	// ゲームループ
	while (true) {
		GetHitKeyStateAll(key);
		ClearDrawScreen();

		Key = GetJoypadInputState(DX_INPUT_PAD1);//使用するパッドを指定する

		//---------  MATH & LOGIC & KEYBINDS  ----------//
		sneezeflag[currPlayer] = buttonFlag(Key & PAD_INPUT_2, oldKey & PAD_INPUT_2);

		if (Key & PAD_INPUT_1) followFlag = TRUE;
		else followFlag = FALSE;

		if (Key & PAD_INPUT_3) attackflag = TRUE;
		else attackflag = FALSE;

		if (Key & PAD_INPUT_UP) upflag = TRUE;
		else upflag = FALSE;

		if (Key & PAD_INPUT_DOWN) downflag = TRUE;
		else downflag = FALSE;

		if (Key & PAD_INPUT_LEFT) leftflag = TRUE;
		else leftflag = FALSE;

		if (Key & PAD_INPUT_RIGHT) rightflag = TRUE;
		else rightflag = FALSE;

		//---------  SCENESTATE SWITCH  ----------//

		switch (scenestate) {
		case openinglogo:
			if (frame < 170) FadeFromBlack(pal, 60);
			if (frame >= 170) FadeToBlack(pal, 60);
			AJKGames.DrawLogo(AJKGames.index, AJKGames.framecount, scenestate, frame, 110);
			break; // This break the opening logo

		case enginelogo:
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawCamStatic(engineLOGO, WIN_WIDTH / 2 - 540 / 2, WIN_HEIGHT / 2 - 252 / 2, 0, 0, shakevar);
			if (frame >= 400) scenestate++;
			break; // This break the opening logo

		case reset:
			scenestate = load;
			break;

		case load:
			for (int amm = 0; amm < HumanLimit; amm++) {
				Human[amm].behavior = rand() % 5;
				RandomizeSpawns(Human[amm].x, Human[amm].y);
				for (int j = 0; j < 28; j++) {
					for (int k = 0; k < 54; k++) {
						if (floordets[j][k] && checkSquareHit(k * 64, j * 64, 64, 64, Human[amm].x, Human[amm].y, 32)) {
							Human[amm].x = WORLD_WIDTH / 2, Human[amm].y = WORLD_HEIGHT / 2;
						}
					}
				}
				camX = Human[amm].x - WIN_WIDTH / 2;
				camY = Human[amm].y - WIN_HEIGHT / 2;
			}
			docHealth = 7000;
			susCount = 0;
			infectedCount = 0;
			isInfectedFlag[0] = TRUE;
			Human[HumanLimit - 1].type = 5; // Doctor
			Human[HumanLimit - 1].y = WORLD_HEIGHT / 2, Human[HumanLimit - 1].x = WORLD_WIDTH / 2;
			Human[0].y = 64 * 4 + 32, Human[0].x = 64 * 2 + 32;
			scenestate = gameplay;
			break;

		case gameplay:
			pal++;
			if (pal >= 255) pal = 255;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);

			if (!CheckSoundMem(bgm)) PlaySoundMem(bgm, DX_PLAYTYPE_BACK);
			// Reset Shake
			if (!shakeflag) shakevar = 0, shakecount = 8;
			if (shakeflag) {
				Shake(shakedev, shakevar, shakecount);
				if (shakecount <= 0) shakeflag = FALSE;
			}

			// Draw
			for (int j = 0; j < 19; j++) {
				for (int k = 0; k < 27; k++) {
					// Just the floor
					DrawCamStatic(mapchip[floormap[j][k]], k * 64, j * 64, camX, camY, shakevar);

					// Details
					if (floordets[j][k] != 0) DrawCamStatic(mapchip[floordets[j][k]], k * 64, j * 64 - 16, camX, camY, shakevar);
				}
			}

			
			// Basic Necessities (Movement, AI)
			for (int num = 0; num < HumanLimit; num++) {
				Human[num].px = Human[num].x, Human[num].py = Human[num].y;
				if (num != currPlayer && isInfectedFlag[num] && !isPlayerFlag[currPlayer]) {
					isPlayerFlag[num] = TRUE;
					currPlayer = num;
				}
				// Will only run when the player is controlling that character.
				if (isPlayerFlag[num]) {

					// Vial Exploded
					if (checkHit(Vial.x, Vial.y, Human[num].x, Human[num].y, 32, 32) && vialstate == exploded) {
						isInfectedFlag[num] = FALSE;
						isPlayerFlag[num] = FALSE;
					}

					

					//isInfectedFlag[num] = TRUE;
					if (!sneezingflag[num]) Human[num].AccMov(Human[num].x, Human[num].y, Human[num].accX, Human[num].accY, upflag, downflag, leftflag, rightflag);
					// Camera
					MoveCamera(camX, camY, Human[num].x, Human[num].y, WORLD_WIDTH - WIN_WIDTH, WORLD_HEIGHT - WIN_HEIGHT);

					// Sneezing
					if (sneezeflag[currPlayer]) sneezingflag[currPlayer] = TRUE, shakeflag = TRUE, PlaySoundMem(sneezeSound, DX_PLAYTYPE_BACK);;

					if (sneezingflag[currPlayer]) {
						sneezeCounter[currPlayer]++;
						//DrawReferencePoint(Human[num].x, Human[num].y, camX, camY, sneezeRadius);
						if (sneezeCounter[currPlayer] >= sneezeDuration) {
							sneezeCounter[currPlayer] = 0;
							sneezingflag[currPlayer] = FALSE;
						}
					}
					
				}
				// Will run for every other character.
				else {
					// Behavior Switch
					if (!isInfectedFlag[num] || !followFlag) switch (Human[num].behavior) {
					case idle:
						Human[num].IdleWithPace(Human[num].x, Human[num].y, Human[num].state, Human[num].statetimer, Human[num].dir);
						break;

					case pacediagonal:
						Human[num].PaceDiagonal(Human[num].x, Human[num].y, Human[num].state, Human[num].statetimer, Human[num].dir);
						break;

					case walkdiagonal:
						Human[num].WalkDiagonal(Human[num].x, Human[num].y, Human[num].state, Human[num].statetimer, Human[num].dir);
						break;

					case mixed:
						Human[num].WalkMixed(Human[num].x, Human[num].y, Human[num].state, Human[num].statetimer, Human[num].dir);
						break;

					case mixedplus:
						Human[num].WalkMixedPlus(Human[num].x, Human[num].y, Human[num].state, Human[num].statetimer, Human[num].dir);
						break;
					}
					// else if the character is infected. FOLLOWFLAG
					else if (isInfectedFlag[num] && followFlag) {
						circleHitBox(Human[num].x, Human[num].y, Human[currPlayer].x, Human[currPlayer].y, 32, 32);
						SimpleFollowScript(Human[num].x, Human[num].y, Human[currPlayer].x, Human[currPlayer].y, 2);
					}


					// Doctor Info Here
					if (Human[num].type == 5) {
						// If Player is inside sus radius
						if (checkHit(Human[num].x, Human[num].y, Human[currPlayer].x, Human[currPlayer].y, 32, susRadius)) {
							if (sneezeflag[currPlayer]) susCount += 10;
							susCountSubCount++;
							if (susCountSubCount >= 20) {
								susCount++;
								susCountSubCount = 0;
							}
						}
						
					}


					// Hitbox with other Characters
					for (int amm = 0; amm < HumanLimit; amm++) {
						if (num != amm) circleHitBox(Human[num].x, Human[num].y, Human[amm].x, Human[amm].y, 32, 32);

						// Doctor Infection Check
						if (Human[num].type == 5 && isInfectedFlag[amm]) {
							if (checkHit(Human[num].x, Human[num].y, Human[amm].x, Human[amm].y, 32, susRadius)) {
								SimpleFollowScript(Human[num].x, Human[num].y, Human[amm].x, Human[amm].y, 1);

								susCountSubCount++;
								if (susCountSubCount >= 30) {
									susCount++;
									susCountSubCount = 0;
								}
							}

							// Sneezeradius
							if (checkHit(Human[num].x, Human[num].y, Human[amm].x, Human[amm].y, 32, sneezeRadius + 10)) {
								if (vialstate == inactive) {
									Vial.endX = Human[amm].x;
									Vial.endY = Human[amm].y;
									if (vialtimer >= vialtimerMAX) {
										vialtimer = 0;
										vialstate = thrown;
										PlaySoundMem(throwSound, DX_PLAYTYPE_BACK);
									}
								}
							}
						}

						// Surround Count Check
						if (checkHit(Human[num].x, Human[num].y, Human[amm].x, Human[amm].y, 32, susRadius)) {
							surroundCount[num]++;
						}
					}

					// Sneeze Check
					if (sneezeflag[currPlayer] && checkHit(Human[num].x, Human[num].y, Human[currPlayer].x, Human[currPlayer].y, 32, sneezeRadius) && !isInfectedFlag[num]) {
						if (Human[num].type != 5) isInfectedFlag[num] = TRUE;
					}
				}

				// Vial Exploded
				if (checkHit(Vial.x, Vial.y, Human[num].x, Human[num].y, 32, 32) && vialstate == exploded) {
					isInfectedFlag[num] = FALSE;
					isPlayerFlag[num] = FALSE;
				}

				if (isInfectedFlag[num] && !isPlayerFlag[currPlayer]) {
					isPlayerFlag[num] = TRUE;
				}

				// Infection Count
				if (isInfectedFlag[num]) infectedCount++; // Adds to the infected count
				

				// Hitboxes
				if (Human[num].x >= WORLD_WIDTH) Human[num].x = WORLD_WIDTH, Human[num].dir = rand() % 4 + 1;
				else if (Human[num].x <= 0) Human[num].x = 0, Human[num].dir = rand() % 4 + 1;
				if (Human[num].y >= WORLD_HEIGHT) Human[num].y = WORLD_HEIGHT, Human[num].dir = rand() % 4 + 1;
				else if (Human[num].y <= 0) Human[num].y = 0, Human[num].dir = rand() % 4 + 1;

				for (int j = 0; j < 28; j++) {
					for (int k = 0; k < 54; k++) {
						if (floordets[j][k]) {
							squareHitBox(k * 64, j * 64, 64, 64, Human[num].x, Human[num].y, 28, Human[num].px, Human[num].py);
						}
					}
				}

				// Character
				bool moveflag[HumanLimit] = { FALSE };
				if (Human[num].x != Human[num].px || Human[num].y != Human[num].py) moveflag[num] = TRUE;

				if (Human[num].x > Human[num].px) Human[num].direction = right;
				if (Human[num].x < Human[num].px) Human[num].direction = left;
				if (Human[num].y > Human[num].py) Human[num].direction = down;
				if (Human[num].y < Human[num].py) Human[num].direction = up;

				// Bounce
				int bounceMAX = 12;
				if (moveflag[num]) switch (bouncestate[num]) {
				case 0:
					bounce[num] += 2;
					if (bounce[num] >= bounceMAX) bouncestate[num] = TRUE;
					break;
				case 1:
					bounce[num] -= 2;
					if (bounce[num] <= 0) bouncestate[num] = FALSE;
					break;
				}
				else if (!moveflag[num]) {
					bounce[num] = 0;
				}
				int bounceangleMAX = 0.5;
				if (moveflag[num]) switch (bounceanglestate[num]) {
				case 0:
					bounceangle[num] += 0.05;
					if (bounceangle[num] >= bounceangleMAX) bounceanglestate[num] = TRUE;
					break;
				case 1:
					bounceangle[num] -= 0.05;
					if (bounceangle[num] <= -bounceangleMAX) bounceanglestate[num] = FALSE;
					break;
				}
				else if (!moveflag[num]) {
					bounceangle[num] = 0;
				}

				if (!moveflag[num] && !sneezingflag[num]) switch (Human[num].direction) {
				case up:																																		
				case down:																																		
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 0, 0, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;										 																									
				case left:										 																									
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 4, 0, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;										 
				case right:										 
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 12, 0, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;
				}
				else if (moveflag[num] && !sneezeflag[num]) switch (Human[num].direction) {
				case up:
				case down:
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 0, 4, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;										
				case left:										
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 4, 4, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;										
				case right:										
					DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], characterFramerate, 12, 4, TRUE, camX, camY, bounceangle[num], shakevar, bounce[num]);
					break;
				}
				else if (sneezingflag[num]) DrawRotaAnimation(Human[num].x, Human[num].y - 16, playerIMG[Human[num].type], characterIndex[num], characterFramecount[num], 1, 16, 11, FALSE, camX, camY, bounceangle[num], shakevar, bounce[num]);
			
				
				if (isInfectedFlag[num]) DrawRotaAnimation(Human[num].x, Human[num].y -20 - 16, arrowSmall, arrowIndex[num], arrowFramecount[num], 10, 0, 4, 1, camX, camY, bounceangle[num], shakevar, bounce[num]);
				if (isPlayerFlag[num]) DrawCamStatic(arrow, Human[num].x - 32, Human[num].y - 64 - 24, camX, camY, shakevar);
				
				// Spider X Y
				if (!attackflag) spiderX[num] = Human[num].x, spiderY[num] = Human[num].y;
				else if (attackflag && isInfectedFlag[num] && checkHit(Human[num].x, Human[num].y - 16, Human[currDoc].x, Human[currDoc].y, 32, susRadius)) {
					DrawReferenceLine(Human[num].x, Human[num].y-16, spiderX[num], spiderY[num], camX, camY, 118, 66, 138, 4, shakevar);
					DrawCamStatic(spiderIMG, spiderX[num] - 16, spiderY[num] - 16, camX, camY, shakevar);
					SimpleFollowScript(spiderX[num], spiderY[num], Human[currDoc].x, Human[currDoc].y, 3);
					if (checkHit(spiderX[num], spiderY[num], Human[currDoc].x, Human[currDoc].y, 12, 32)) docHealth--;
				}

				if (Human[num].type == 5) DrawPercentBar(Human[num].x - 40, Human[num].y - 32 - 10 - 16, Human[num].x + 40, Human[num].y - 32 - 16, camX, camY, 0, 7000, docHealth, 0, 255, 0, true);
				DrawPercentBar(20, 10, WIN_WIDTH - 20, 10 + 20, 0, 0, 0, susMAX, susCount, 118, 64, 132, true);
				float percent = susCount / susMAX;
				float place = WIN_WIDTH - 20 - 20;
				DrawStatic(20 + place * percent, 32, virus);
				// Resets Surround count every frame.
				surroundCount[num] = 0;
			}



			// Vials
			switch (vialstate) {
			case inactive:
				vialtimer++;
				Vial.x = Human[currDoc].x;
				Vial.y = Human[currDoc].y;
				break;

			case thrown:
				vialtimer++;
				vialangle += 0.2;
				SimpleFollowScript(Vial.x, Vial.y, Vial.endX, Vial.endY, 5);
				DrawRotaGraph(Vial.x - camX, Vial.y - camY, 1, vialangle, vialIMG, true);
				if (vialtimer >= vialtimerMAX / 4) {
					vialstate = exploded;
					PlaySoundMem(glass, DX_PLAYTYPE_BACK);
					vialtimer = 0;
				}
				break;

			case exploded:
				shakeflag = TRUE;
				vialtimer++;
				DrawRotaGraph(Vial.x - camX, Vial.y - camY, 1, 0, vialexplodedIMG, true);
				if (vialtimer >= vialtimerMAX * 3) {
					vialtimer = 0;
					vialstate = inactive;
				}
				break;
			}

			if (frame >= 10) susCount--, frame = 0;
			if (susCount <= 0) susCount = 0;

			DrawStatic(32 * 3, WIN_HEIGHT - 32 * 2, controls);

			// LOSE&WIN GAME HERE ------------------------------------------------------------------------
			if (docHealth <= 0) {
				scenestate = win;
				frame = 0;
				pal = 0;
			}

			if (susCount >= susMAX) {
				scenestate = overused;
				frame = 0;
				pal = 0;
			}

			if (infectedCount == 0) {
				scenestate = outofpeople;
				frame = 0;
				pal = 0;
				
			}

		
			break; // This breaks gameplay

			case win:
				pal++;
				if (pal >= 255) pal = 255;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
				SetFontSize(28);
				DrawString(WIN_WIDTH / 2, (WIN_HEIGHT / 3) * 2, "There are no healthy people left.\n健常者が残っていません。", GetColor(168, 86, 132), GetColor(255, 255, 255));

				break;
			case overused:
				pal++;
				if (pal >= 255) pal = 255;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
				SetFontSize(28);
				DrawString(WIN_WIDTH / 2, (WIN_HEIGHT / 3) * 2, "You used your powers too much\nand everyone infected died.\n\n力入れすぎて感染者が死にました。", GetColor(168, 86, 132), GetColor(255, 255, 255));

				break;
			case outofpeople:
				pal++;
				if (pal >= 255) pal = 255;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
				SetFontSize(28);
				DrawString(WIN_WIDTH / 2, (WIN_HEIGHT / 3) * 2, "There are no infected people left.\n\n感染者が残っていません。", GetColor(168, 86, 132), GetColor(255, 255, 255));

				break;
		}

		// DEBUG GOES HERE
		//DrawFormatString(20, 20, GetColor(255, 255, 255), "%04hx	%04hx	%04hx	%04hx", isPlayerFlag[0], isPlayerFlag[1], isInfectedFlag[0], isInfectedFlag[1]);
		infectedCount = 0;
		
		//---------  ESSENTIALS  ---------//
		oldKey = GetJoypadInputState(DX_INPUT_PAD1);//使用するパッドを指定する
		GetHitKeyStateAll(prevkey);
		frame++;
		ScreenFlip();
		WaitTimer(20); // 20 miliseconds = 60 FPS
		if (ProcessMessage() == -1) {
			break;
		}

		// PRESS B TO BREAK. DELETE THIS AT THE END. ---------------------------------------------------------------------------------------------------
		if (CheckHitKey(KEY_INPUT_B) == 1) {
			break;
		}
	}
	DxLib_End(); // If this gives an error, restart Visual Studio.
	return 0;
}
