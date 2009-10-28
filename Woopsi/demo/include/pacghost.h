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

	inline const bool isActive() const { return _active; };

	inline void setType(const u8 type) { _type = type; };
	inline void setActive(const bool active) { _active = active; };
	
	inline void run() { move(); };
	void reset();
	void leaveHome();
	void move();
	
	void doDumbAI();
	void doSmartAI();
};

#endif
