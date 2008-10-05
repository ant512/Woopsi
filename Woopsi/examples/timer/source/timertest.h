#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "woopsitimer.h"
#include "textbox.h"

using namespace std;

class TimerTest : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	
private:
	TextBox* _output;
	WoopsiTimer* _timer;
	unsigned char _text;
};

#endif
