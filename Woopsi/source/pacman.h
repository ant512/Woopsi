#ifndef _PACMAN_H_
#define _PACMAN_H_

#include <vector>
#include "woopsi.h"
#include "sysfont.h"

class PacSprite;
class PacMan;
class PacMap;
class PacGhost;
class PacGhosts;
class PacPlayer;

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

class PacMan : EventHandler {
	
public:
	PacMan(SimpleScreen* screen);
	~PacMan();
	
	u16 getWidth();
	u16 getHeight();

	bool isGameOver();
	
	PacMap* getMap();
	PacGhosts* getGhosts();
	PacPlayer* getPlayer();
	SuperBitmap* getSuperBitmap();

	void reset();
	void run();
	void endGame();

	bool handleEvent(const EventArgs& e);
	void handleVBL(const EventArgs& e);
	void handleKeyPress(const EventArgs& e);
	void handleRelease(const EventArgs& e);

private:
	SimpleScreen* _screen;
	SimpleWindow* _window;
	SuperBitmap* _superBitmap;
	Button* _resetButton;

	u16 _width;
	u16 _height;
	bool _gameOver;
	PacMap* _map;
	PacGhosts* _ghosts;
	PacPlayer* _player;

	void initGUI();

};

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

class PacGhosts {

private:
	PacMan* _game;
	vector<PacGhost*> _ghosts;
	u8 _ghostCount;
	u8 _ghostsInHome;
	
public:
	PacGhosts(PacMan* game);
	~PacGhosts();

	void reset();
	void run();
	void draw();
};


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
