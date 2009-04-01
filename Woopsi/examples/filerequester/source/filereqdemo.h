#ifndef _FILE_REQ_DEMO_H_
#define _FILE_REQ_DEMO_H_

#include "woopsi.h"
#include "eventhandler.h"
#include "textbox.h"

using namespace WoopsiUI;

class FileReqDemo : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();
	void handleValueChangeEvent(const GadgetEventArgs& e);
	
private:
	TextBox* _textbox;
};

#endif
