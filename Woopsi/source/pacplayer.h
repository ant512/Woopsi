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

	void setBufferedDirection(u8 bufferedDirection);
	void resetLives();

	void reset();
	void loseLife();
	void move();
	void run();
};

#endif
