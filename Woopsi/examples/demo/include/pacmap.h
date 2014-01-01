#ifndef _PACMAP_H_
#define _PACMAP_H_

#include "woopsifuncs.h"

class PacMan;

class PacMap {
private:
	static const u8 BLOCK_WIDTH = 5;
	static const u8 BLOCK_HEIGHT = 5;

	static const u8 PILL_OFFSET = 1;
	static const u8 PILL_WIDTH = 2;
	static const u8 PILL_HEIGHT = 2;

	static const u8 BLOCK_EMPTY = 0;
	static const u8 BLOCK_PILL = 1;
	static const u8 BLOCK_PILL_EATEN = 2;
	static const u8 BLOCK_POWER = 3;
	static const u8 BLOCK_POWER_EATEN = 4;
	static const u8 BLOCK_WALL = 5;
	static const u8 BLOCK_SPACE = 6;
	static const u8 BLOCK_GHOST = 7;

	static const u8 MAP_WIDTH = 19;
	static const u8 MAP_HEIGHT = 22;

	PacMan* _game;
	u8 _remainingPills;
	static u8 _layout[MAP_HEIGHT][MAP_WIDTH];
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
