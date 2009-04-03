#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "woopsi.h"
#include "keyboardeventhandler.h"
#include "keyboardeventargs.h"
#include "woopsikeyboard.h"
#include "multilinetextbox.h"
#include "woopsikey.h"

using namespace WoopsiUI;

class KeyTest : public Woopsi, public KeyboardEventHandler {
public:
	void startup();
	void shutdown();
	void handleKeyboardPressEvent(const KeyboardEventArgs& e);
	void handleKeyboardRepeatEvent(const KeyboardEventArgs& e);
	
private:
	WoopsiKeyboard* _keyboard;
	MultiLineTextBox* _output;
	WoopsiTimer* _timer;
	
	void processKey(const WoopsiKey* key);
};

#endif
