#ifndef _WOOPSIE_H_
#define _WOOPSIE_H_

#include <nds.h>
#include <vector>
#include "sysfont.h"
#include "bitmap.h"
#include "fontbase.h"
#include "font.h"
#include "monofont.h"
#include "gadget.h"
#include "button.h"
#include "window.h"
#include "amigawindow.h"
#include "simplewindow.h"
#include "screen.h"
#include "amigascreen.h"
#include "simplescreen.h"
#include "textwriter.h"
#include "textbox.h"
#include "eventhandler.h"
#include "superbitmap.h"
#include "graphicsport.h"
#include "gradient.h"
#include "skin.h"
#include "skinnedscreen.h"
#include "skinnedwindow.h"
#include "debug.h"
#include "animation.h"
#include "bitmapbutton.h"
#include "animbutton.h"
#include "woopsifuncs.h"
#include "scrollingpanel.h"
#include "scrollbarvertical.h"

using namespace std;

class Woopsi : public Gadget {

public:
	Woopsi(FontBase* font = NULL);
	virtual ~Woopsi();
	virtual Screen* newScreen(char* title);
	virtual void play();
	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual void handleStylus();
	virtual void handleKeys();
	virtual void handleLid();
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool vbl();
	virtual void setActiveGadget(Gadget* gadget);
	virtual bool swapGadgetDepth(Gadget* gadget);
	virtual bool flipScreens(Gadget* gadget);
	virtual void eraseRect(Rect rect);

	static FontBase* getSystemFont();
	static void deleteSystemFont();

	static void registerForVBL(Gadget* gadget);
	static void unregisterFromVBL(Gadget* gadget);
	static void addToDeleteQueue(Gadget* gadget);
	static u32 getVBLCount();

protected:
	bool _lidClosed;
	
	static vector<Gadget*> _vblListeners;
	static vector<Gadget*> _deleteQueue;
	static FontBase* _systemFont;
	static u32 _vblCount;

	virtual void closeChild(Gadget* gadget);
	virtual void hideChild(Gadget* gadget);

	void processDeleteQueue();
};

#endif
