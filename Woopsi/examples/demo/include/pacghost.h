#ifndef _PAC_GHOST_H_
#define _PAC_GHOST_H_

#include "woopsifuncs.h"
#include "pacsprite.h"

class PacMan;

class PacGhost : public PacSprite {

public:
    enum PacGhostType {
        PAC_GHOST_TYPE_RANDOM = 0,
        PAC_GHOST_TYPE_SMART = 1
    };

private:
	u8 _currentDirection;
	u8 _bufferedDirection;
	u8 _speed;
	bool _leftHome;
	bool _active;
	PacGhostType _type;

public:
	PacGhost(PacMan* game);

	inline const bool isActive() const { return _active; };

	inline void setType(PacGhostType type) { _type = type; };
	inline void setActive(const bool active) { _active = active; };
	
	inline void run() { move(); };
	void reset();
	void leaveHome();
	void move();
	
	void doDumbAI();
	void doSmartAI();
};

#endif
