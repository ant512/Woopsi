#ifndef _SCROLLBAR_VERTICAL_H_
#define _SCROLLBAR_VERTICAL_H_

#include <nds.h>
#include "gadget.h"
#include "slidervertical.h"

using namespace std;

class ScrollbarVertical : public Gadget {

public:

	ScrollbarVertical(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);
	virtual ~ScrollbarVertical();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool resize(u16 width, u16 height);

protected:
	SliderVertical* _slider;
};

#endif
