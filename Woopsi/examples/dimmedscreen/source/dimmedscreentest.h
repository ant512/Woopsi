#ifndef _DIMMED_SCREEN_TEST_H_
#define _DIMMED_SCREEN_TEST_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "dimmedscreen.h"

using namespace WoopsiUI;

class DimmedScreenTest : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
private:
	DimmedScreen* _dimmedScreen;
};

#endif
