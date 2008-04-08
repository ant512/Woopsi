#ifndef _PAC_GHOST_H_
#define _PAC_GHOST_H_

#include "woopsifuncs.h"
#include "pacsprite.h"

class PacMan;

class PacGhost : public PacSprite {

private:
	u8 _currentDirection;
	u8 _bufferedDirection;
	u8 _speed;
	bool _leftHome;
	bool _active;
	bool _type;	// 0 = random, 1 = smart

public:

	PacGhost(PacMan* game);

	bool isActive();

	void setType(u8 type);
	void setActive(bool active);
	
	void run();
	void reset();
	void leaveHome();
	void move();
	
	void doDumbAI();
	void doSmartAI();
};

#endif
