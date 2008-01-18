#ifndef _SCROLLBAR_VERTICAL_COMPLEX_H_
#define _SCROLLBAR_VERTICAL_COMPLEX_H_

#include <nds.h>
#include "gadget.h"
#include "scrollbarvertical.h"

using namespace std;

class ScrollbarVerticalComplex : public Gadget {

public:

	ScrollbarVerticalComplex(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);
	virtual ~ScrollbarVerticalComplex();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool resize(u16 width, u16 height);

protected:
	ScrollbarVertical* _scrollbar;
};

#endif
