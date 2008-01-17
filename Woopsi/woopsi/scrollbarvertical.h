#ifndef _SCROLLBAR_VERTICAL_H_
#define _SCROLLBAR_VERTICAL_H_

#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"
#include "scrollbarverticalgrip.h"
#include "eventhandler.h"

using namespace std;

class ScrollbarVertical : public Gadget, public EventHandler {

public:

	ScrollbarVertical(s16 x, s16 y, u16 width, u16 height);
	virtual ~ScrollbarVertical();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	virtual bool handleEvent(const EventArgs& e);

protected:
	ScrollbarVerticalGrip* _grip;
	s16 _gripY;

	void raiseScrollEvent(s16 x, s16 y);
};

#endif
