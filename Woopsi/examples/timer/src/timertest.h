#ifndef _KEY_TEST_H_
#define _KEY_TEST_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "woopsitimer.h"
#include "textbox.h"

using namespace WoopsiUI;

class TimerTest : public Woopsi, public GadgetEventHandler {
public:
	void handleActionEvent(const GadgetEventArgs& e);
	
private:
	TextBox* _output;
	WoopsiTimer* _timer;
	unsigned char _text;
	
	void startup();
	void shutdown();
};

#endif
