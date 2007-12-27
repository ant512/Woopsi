#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <nds.h>

#define DEBUG_ACTIVE true

class AmigaScreen;
class AmigaWindow;
class MultiLineTextBox;
class Gadget;
class Woopsi;
class Font;

class Debug {

public:
	static void busyWait();
	static void output(char* msg);
	static void printf(const char *format, ...);
	static void setWoopsi(Woopsi* woopsi);

private:
	static AmigaScreen* _screen;
	static AmigaWindow* _window;
	static MultiLineTextBox* _textBox;
	static Woopsi* _woopsi;
	static Font* _font;
	static void createGUI();
};

#endif
