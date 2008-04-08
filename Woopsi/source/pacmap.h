#ifndef _PACMAP_H_
#define _PACMAP_H_

#include "woopsifuncs.h"

class PacMan;

class PacMap {
private:
	PacMan* _game;
	u8 _blockWidth;
	u8 _blockHeight;
	u8 _pillOffset;
	u8 _pillWidth;
	u8 _pillHeight;
	u8 _emptyBlock;
	u8 _pillBlock;
	u8 _eatenPillBlock;
	u8 _powerBlock;
	u8 _eatenPowerBlock;
	u8 _wallBlock;
	u8 _spaceBlock;
	u8 _ghostBlock;
	u8 _remainingPills;
	static u8 _layout[22][19];
	u8 _width;
	u8 _height;
	u16 _colour;

public:
	PacMap(PacMan* game);					
	void draw();
	void clearBlock(u8 x, u8 y);
	void reset();
	bool checkBlock(s16 x, s16 y, u8 isGhost);
	void drawPills();
};

#endif
