#ifndef _BITMAP_LOADER_H_
#define _BITMAP_LOADER_H_

#include <nds.h>
#include "woopsi.h"

using namespace WoopsiUI;

class BitmapLoader : public Woopsi {
public:
	void startup();
	void shutdown();
};

#endif
