#ifndef _PAC_PLAYER_H_
#define _PAC_PLAYER_H_

#include "woopsifuncs.h"
#include "pacsprite.h"

class PacMan;

class PacPlayer : public PacSprite {

private:
	u8 _bufferedDirection;
	u8 _currentDirection;
	u8 _speed;
	u8 _lives;

public:
	PacPlayer(PacMan* game);

	inline void setBufferedDirection(const u8 bufferedDirection) { _bufferedDirection = bufferedDirection; };
	inline void resetLives() { _lives = 3; };

	void reset();
	void loseLife();
	void move();
	inline void run() { move(); };
};

#endif
