#ifndef _KEYBOARD_TEXTBOX_H_
#define _KEYBOARD_TEXTBOX_H_

#include "woopsi.h"
#include "woopsikeyboard.h"
#include "textbox.h"

using namespace WoopsiUI;

class KeyboardTextBox : public Woopsi {
public:
	void startup();
	void shutdown();
	
private:
	WoopsiKeyboard* _keyboard;
	TextBox* _output;
	WoopsiTimer* _timer;
};

#endif
