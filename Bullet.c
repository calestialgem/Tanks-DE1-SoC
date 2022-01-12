#include "Bullet.h"

#include "MathTools.h"
#include "Timer.h"

void bullet_move(volatile Bullet *const bullet) {
	Vector const acceleration = {.x = 0.0F, .y = BULLET_GRAVITY};
	Vector const velocityEffect = vector_mul(bullet->velocity, TIMER_STEP);
	Vector const accelerationEffect =
		vector_mul(acceleration, math_square(TIMER_STEP) / 2.0F);
	Vector const positionChange =
		vector_add(velocityEffect, accelerationEffect);
	bullet->position = vector_add(bullet->position, positionChange);
	Vector const velocityChange = vector_mul(acceleration, TIMER_STEP);
	bullet->velocity = vector_add(bullet->velocity, velocityChange);
}
