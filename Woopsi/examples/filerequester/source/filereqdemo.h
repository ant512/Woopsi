#ifndef _FILE_REQ_DEMO_H_
#define _FILE_REQ_DEMO_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "textbox.h"

using namespace WoopsiUI;

class FileReqDemo : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	
private:
	TextBox* _textbox;
};

#endif
