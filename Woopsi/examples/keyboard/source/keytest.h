#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "woopsikeyboard.h"
#include "multilinetextbox.h"

using namespace WoopsiUI;

class KeyTest : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	
private:
	WoopsiKeyboard* _keyboard;
	MultiLineTextBox* _output;
	WoopsiTimer* _timer;
};

#endif
