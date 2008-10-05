#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "woopsi.h"
#include "sysfont.h"
#include "dynamicarray.h"

class SuperBitmap;
class AmigaScreen;
class AmigaWindow;
class Button;
class WoopsiTimer;

class PacMap;
class PacGhost;
class PacGhosts;
class PacPlayer;

/**
 * PacMan game class.
 */
class PacMan : EventHandler {
	
public:
	/**
	 * Constructor.  Automatically creates the PacMan GUI.
	 * @param screen The screen to create the GUI on.
	 */
	PacMan(AmigaScreen* screen);

	/**
	 * Destructor.
	 */
	~PacMan();
	
	inline const u16 getWidth() const { return _width; };
	inline const u16 getHeight() const { return _height; };

	inline const bool isGameOver() const { return _gameOver; };
	
	PacMap* getMap();
	PacGhosts* getGhosts();
	PacPlayer* getPlayer();
	SuperBitmap* getSuperBitmap();

	void reset();
	void run();
	void endGame();

	bool handleEvent(const EventArgs& e);
	void handleKeyPress(const EventArgs& e);
	void handleRelease(const EventArgs& e);

private:
	AmigaScreen* _screen;
	AmigaWindow* _window;
	SuperBitmap* _superBitmap;
	Button* _resetButton;
	WoopsiTimer* _timer;

	u16 _width;
	u16 _height;
	bool _gameOver;
	PacMap* _map;
	PacGhosts* _ghosts;
	PacPlayer* _player;

	void initGUI();

};

#endif
