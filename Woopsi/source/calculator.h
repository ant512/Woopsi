#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "woopsi.h"

class Calculator : EventHandler {

public:
	Calculator(AmigaScreen* screen);
	void buttonClicked(Gadget* gadget);

	bool handleEvent(const EventArgs& e);
	void handleClick(const EventArgs& e);

private:
	AmigaScreen* _screen;
	AmigaWindow* _window;
	Textbox* _output;
	s32 _val1;
	s32 _val2;
	u8 _opCode;
	char* _text;
	bool _wipeNeeded;

	void doAdd();
	void doSubtract();
	void doMultiply();
	void doDivide();
	void doEquals();
	void updateDisplay();
	void addText(const char* text);
	void doFunction();
	void initGUI();

	u8 countDigits(s32 number);

	void reverse(char s[]);
	void itoa(s32 n, char s[]);
};

#endif
