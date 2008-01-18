#ifndef _SLIDER_VERTICAL_GRIP_H_
#define _SLIDER_VERTICAL_GRIP_H_

#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"

using namespace std;

class SliderVerticalGrip : public Gadget {

public:

	SliderVerticalGrip(s16 x, s16 y, u16 width, u16 height);
	virtual ~SliderVerticalGrip();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
};

#endif
