#ifndef GAME_H
#define GAME_H

#include "Barrel.h"
#include "Bullet.h"
#include "MathTools.h"
#include "Tank.h"
#include "Timer.h"
#include "Vector.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define GAME_WIDTH 320
#define GAME_HEIGHT 240

#define GAME_MAP_LEFT_BORDER 5
#define GAME_MAP_RIGHT_BORDER (GAME_WIDTH - GAME_MAP_LEFT_BORDER)

/** Terrain where tanks are on. */
typedef struct {
	/** Heights of the ground at different horizontal positions in m. */
	float ground[GAME_WIDTH];
} Map;
/** Game state. */
typedef struct {
	/** Remaining tanks. */
	Tanks tanks;
	/** Active bullets. */
	Bullets bullets;
	/** Map. */
	Map map;
	/** Whether the game is running. */
	bool playing;
	/** The index of the tank that can do actions. */
	size_t turn;
	/** Wheter the game is waiting for all the bullets to explode. */
	bool waitingBullets;
} Game;

/** The game which is currently running. Volatile because it is accessed by both
 * the main loop in rendering and the timer interrupt in updating. */
extern volatile Game game_instance;

/** Restarts the game. */
void game_restart();
/** Updates the game. */
void game_update();

#endif // GAME_H
