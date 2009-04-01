#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "textbox.h"
#include "gadgeteventargs.h"

using namespace WoopsiUI;

class Calculator : GadgetEventHandler {

public:
	Calculator(AmigaScreen* screen);
	void buttonClicked(Gadget* gadget);

	bool handleEvent(const GadgetEventArgs& e);
	void handleClick(const GadgetEventArgs& e);

private:
	AmigaScreen* _screen;
	AmigaWindow* _window;
	TextBox* _output;
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
	void appendText(const char* text);
	void doFunction();
	void initGUI();

	u8 countDigits(s32 number);

	void reverse(char s[]);
	void itoa(s32 n, char s[]);
};

#endif
