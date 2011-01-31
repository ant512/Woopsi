#ifndef _PACSPRITE_H_
#define _PACSPRITE_H_

#include "woopsifuncs.h"

class PacMan;

class PacSprite {
public:
	PacSprite(PacMan* game);

	inline const s16 getX() const { return _x; };
	inline const s16 getY() const { return _y; };
	inline const u8 getWidth() const { return _width; };
	inline const u8 getHeight() const { return _height; };

	inline void setColour(const u16 colour) { _colour = colour; };

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
