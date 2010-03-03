#ifndef _KEYBOARD_TEXTBOX_H_
#define _KEYBOARD_TEXTBOX_H_

#include "woopsi.h"
#include "woopsikeyboard.h"
#include "textbox.h"

using namespace WoopsiUI;

class KeyboardTextBox : public Woopsi {
private:
	WoopsiKeyboard* _keyboard;
	TextBox* _output;
	WoopsiTimer* _timer;
	
	void startup();
	void shutdown();
};

#endif
