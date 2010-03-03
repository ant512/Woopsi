#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "woopsi.h"
#include "woopsikeyboard.h"
#include "multilinetextbox.h"
#include "woopsikey.h"

using namespace WoopsiUI;

class KeyTest : public Woopsi {
private:
	WoopsiKeyboard* _keyboard;
	MultiLineTextBox* _output;
	WoopsiTimer* _timer;
	
	void startup();
	void shutdown();
};

#endif
