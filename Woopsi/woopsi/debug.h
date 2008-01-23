#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <nds.h>
#include "eventhandler.h"

#define DEBUG_ACTIVE true

class AmigaScreen;
class AmigaWindow;
class MultiLineTextBox;
class Gadget;
class Woopsi;
class Font;
class SliderVertical;

class Debug : public EventHandler {

public:
	static void busyWait();
	static void output(char* msg);
	static void printf(const char *format, ...);
	static void setWoopsi(Woopsi* woopsi);

	bool handleEvent(const EventArgs& e);

private:
	Debug();
	~Debug();

	static Debug* _debug;
	static Woopsi* _woopsi;
	AmigaScreen* _screen;
	AmigaWindow* _window;
	MultiLineTextBox* _textBox;
	SliderVertical* _slider;
	Font* _font;
	void createGUI();

	static void createDebug();
};

#endif
