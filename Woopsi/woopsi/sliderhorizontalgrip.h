#ifndef _SLIDER_HORIZONTAL_GRIP_H_
#define _SLIDER_HORIZONTAL_GRIP_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class SliderHorizontalGrip : public Gadget {

public:

	SliderHorizontalGrip(s16 x, s16 y, u16 width, u16 height);
	virtual ~SliderHorizontalGrip();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
};

#endif
