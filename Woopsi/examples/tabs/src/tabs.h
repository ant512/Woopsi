#ifndef _TABS_H_
#define _TABS_H_

#include <woopsi.h>
#include <gadgeteventhandler.h>
#include <scrollingtextbox.h>

using namespace WoopsiUI;

class Tabs : public Woopsi, GadgetEventHandler {
public:
	virtual void handleValueChangeEvent(Gadget& source);

protected:
	void startup();
	void shutdown();

private:
	ScrollingTextBox* _text1;
	ScrollingTextBox* _text2;
	ScrollingTextBox* _text3;
};

#endif
