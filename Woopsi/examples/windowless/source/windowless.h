#ifndef _WINDOWLESS_H_
#define _WINDOWLESS_H_

#include "woopsi.h"

using namespace WoopsiUI;

class Windowless : public Woopsi {
private:
	void startup();
	void shutdown();
};

#endif
