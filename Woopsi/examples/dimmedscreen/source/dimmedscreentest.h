#ifndef _DIMMED_SCREEN_TEST_H_
#define _DIMMED_SCREEN_TEST_H_

#include "woopsi.h"
#include "eventhandler.h"

using namespace std;

class DimmedScreen;

class DimmedScreenTest : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
private:
	DimmedScreen* _dimmedScreen;
};

#endif
