#ifndef _CONTEXT_MENU_TEST_H_
#define _CONTEXT_MENU_TEST_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "multilinetextbox.h"

using namespace WoopsiUI;

class ContextMenuTest : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	
private:
	MultiLineTextBox* _output;
};

#endif
