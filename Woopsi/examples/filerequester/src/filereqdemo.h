#ifndef _FILE_REQ_DEMO_H_
#define _FILE_REQ_DEMO_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "textbox.h"

using namespace WoopsiUI;

class FileReqDemo : public Woopsi, public GadgetEventHandler {
public:
	void handleValueChangeEvent(Gadget& source);
	
private:
	TextBox* _textbox;
	
	void startup();
	void shutdown();
};

#endif
