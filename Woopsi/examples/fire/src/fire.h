#ifndef _FIRE_H_
#define _FIRE_H_

#include "woopsi.h"
#include "superbitmap.h"
#include "gadgeteventhandler.h"
#include "gadgeteventargs.h"
#include "graphics.h"

using namespace WoopsiUI;

class Fire : public Woopsi, public GadgetEventHandler {
public:
	void handleActionEvent(const GadgetEventArgs& e);

private:
	SuperBitmap* _superBitmap;
	Graphics* _graphics;
	u16* _palette;
	u16* _buffer1;
	u16* _buffer2;
	u16 _fireWidth;
	u16 _fireHeight;
	u16 _fireColours;
	u16 _intensity;
	u16 _scale;
	
	void startup();
	void shutdown();
	
	void handleTimer();
};

#endif
