#ifndef _DIMMED_SCREEN_TEST_H_
#define _DIMMED_SCREEN_TEST_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "dimmedscreen.h"

using namespace WoopsiUI;

class DimmedScreenTest : public Woopsi, public GadgetEventHandler {
public:
	void handleCloseEvent(const GadgetEventArgs& e);
private:
	DimmedScreen* _dimmedScreen;
	
	void startup();
	void shutdown();
};

#endif
