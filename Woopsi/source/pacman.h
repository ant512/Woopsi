#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "woopsi.h"
#include "sysfont.h"
#include "dynamicarray.h"

class SuperBitmap;
class AmigaScreen;
class AmigaWindow;
class Button;

class PacMap;
class PacGhost;
class PacGhosts;
class PacPlayer;

class PacMan : EventHandler {
	
public:
	PacMan(AmigaScreen* screen);
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
	AmigaScreen* _screen;
	AmigaWindow* _window;
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

#endif
