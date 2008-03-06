#ifndef _SCROLLABLE_BASE_H_
#define _SCROLLABLE_BASE_H_

#include <nds.h>

class ScrollableBase {
public:
	ScrollableBase() { }
	virtual ~ScrollableBase() { }

	virtual const s32 getCanvasX() { return _canvasX; };
	virtual const s32 getCanvasY() { return _canvasY; };

	virtual const s32 getCanvasWidth() { return _canvasWidth; };
	virtual const s32 getCanvasHeight() { return _canvasHeight; };

	virtual void scroll(s32 dx, s32 dy) = 0;
	virtual void jump(s32 x, s32 y) = 0;

protected:
	s32 _canvasX;
	s32 _canvasY;
	s32 _canvasWidth;
	s32 _canvasHeight;
};

#endif
