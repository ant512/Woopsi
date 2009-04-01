#ifndef _PROGRESS_BAR_TEST_H_
#define _PROGRESS_BAR_TEST_H_

#include "woopsi.h"
#include "gadgeteventhandler.h"
#include "progressbar.h"

using namespace WoopsiUI;

class ProgressBarTest : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();
	void handleActionEvent(const GadgetEventArgs& e);
private:
	ProgressBar* _progressBar;
	WoopsiTimer* _timer;
};

#endif
