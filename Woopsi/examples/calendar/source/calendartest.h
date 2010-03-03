#ifndef _CALENDAR_TEST_H_
#define _CALENDAR_TEST_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "calendar.h"

using namespace WoopsiUI;

class CalendarTest : public Woopsi, public GadgetEventHandler {
public:
	void handleActionEvent(const GadgetEventArgs& e);
private:
	Calendar* _calendar;
	
	void startup();
	void shutdown();
};

#endif
