#ifndef _FILE_REQ_DEMO_H_
#define _FILE_REQ_DEMO_H_

#include "woopsi.h"
#include "eventhandler.h"

using namespace std;

class TextBox;

class FileReqDemo : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	
private:
	TextBox* _textbox;
};

#endif
