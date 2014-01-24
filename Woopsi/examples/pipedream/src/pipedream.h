#ifndef _PIPE_DREAM_H_
#define _PIPE_DREAM_H_

#include <woopsi.h>
#include <gadgeteventhandler.h>
#include <woopsitimer.h>
#include <tabgroup.h>
#include <scrollingtextbox.h>

#include "pipebuttongrid.h"

using namespace WoopsiUI;

class PipeDream : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();

	void handleActionEvent(Gadget& source);
	void handleValueChangeEvent(Gadget& source);
	
private:
	PipeButtonGrid* _grid;
	WoopsiTimer* _flowTimer;
	WoopsiTimer* _redrawTimer;
	TabGroup* _tabs;
	ScrollingTextBox* _instructionsTextBox;
	ScrollingTextBox* _aboutTextBox;
	
	void createGameScreen();
	void createDocsScreen();
};

#endif
