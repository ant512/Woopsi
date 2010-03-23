#ifndef _PIPE_DREAM_H_
#define _PIPE_DREAM_H_

#include "woopsi.h"
#include "pipebuttongrid.h"
#include "gadgeteventhandler.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

class PipeDream : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();

	void handleActionEvent(const GadgetEventArgs& e);
	
private:
	PipeButtonGrid* _grid;
	WoopsiTimer* _timer;
};

#endif
