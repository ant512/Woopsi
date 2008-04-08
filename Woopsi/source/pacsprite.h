#ifndef _PACSPRITE_H_
#define _PACSPRITE_H_

#include "woopsifuncs.h"

class PacMan;

class PacSprite {
public:
	PacSprite(PacMan* game);

	s16 getX();
	s16 getY();
	u8 getWidth();
	u8 getHeight();

	void setColour(u16 colour);

	void draw();
	bool checkCollision(PacSprite* sprite);
	bool checkMove(u8 newDirection, u8 speed, bool isGhost);
	bool move(u8 newDirection, u8 speed, bool isGhost);

protected:
	PacMan* _game;
	s16 _x;
	s16 _y;
	u8 _width;
	u8 _height;
	u16 _colour;
};

#endif
