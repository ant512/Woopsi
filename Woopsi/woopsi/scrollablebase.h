#ifndef _SCROLLABLE_BASE_H_
#define _SCROLLABLE_BASE_H_

#include <nds.h>

class ScrollableBase {
public:
	ScrollableBase() { }
	virtual ~ScrollableBase() { }

	virtual const s32 getScrollX() { return _scrollX; };
	virtual const s32 getScrollY() { return _scrollY; };

	virtual const s32 getMinScrollX() { return _minScrollX; };
	virtual const s32 getMinScrollY() { return _minScrollY; };
	virtual const s32 getMaxScrollX() { return _maxScrollX; };
	virtual const s32 getMaxScrollY() { return _maxScrollY; };

	virtual void setMinScrollX(const s32 minScrollX) { _minScrollX = minScrollX; };
	virtual void setMinScrollY(const s32 minScrollY) { _minScrollY = minScrollY; };
	virtual void setMaxScrollX(const s32 maxScrollX) { _maxScrollX = maxScrollX; };
	virtual void setMaxScrollY(const s32 maxScrollY) { _maxScrollY = maxScrollY; };

	virtual const s32 getScrollWidth() { return (_maxScrollX - _minScrollX) + 1; };
	virtual const s32 getScrollHeight() { return (_maxScrollY - _minScrollY) + 1; };

	virtual void scroll(s32 dx, s32 dy) = 0;
	virtual void jump(s32 x, s32 y) = 0;

protected:
	s32 _scrollX;
	s32 _scrollY;
	s32 _minScrollX;
	s32 _minScrollY;
	s32 _maxScrollX;
	s32 _maxScrollY;
};

#endif
