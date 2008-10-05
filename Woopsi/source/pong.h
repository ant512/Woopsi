#ifndef _PONG_H_
#define _PONG_H_

#include "woopsi.h"
#include "sysfont.h"

class AmigaScreen;
class AmigaWindow;
class WoopsiTimer;

class Pong : EventHandler {

public:
	Pong(AmigaScreen* screen);
	~Pong();

	void draw();
	void play();
	void moveObjects();

	bool handleEvent(const EventArgs& e);
	void handleKeyPress(const EventArgs& e);
	void handleKeyRelease(const EventArgs& e);

private:
	AmigaScreen* _screen;
	AmigaWindow* _window;
	WoopsiTimer* _timer;
	u8 _p1x;
	u8 _p1y;
	u8 _p2x;
	u8 _p2y;
	s16 _bx;
	s16 _by;
	s8 _bxSpeed;
	s8 _bySpeed;
	u8 _pWidth;
	u8 _pHeight;
	u8 _bRadius;
	bool _upHeld;
	bool _downHeld;
	FontBase* _font;
	u8 _fontRColour;
	u8 _fontGColour;
	u8 _fontBColour;

	void initGUI();
};

#endif
