#ifndef _SCROLLING_PANEL_H_
#define _SCROLLING_PANEL_H_

#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"

using namespace std;

class ScrollingPanel : public Gadget {
public:
	ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font = NULL);
	virtual ~ScrollingPanel();

	virtual u32 getScrollX();
	virtual u32 getScrollY();

	virtual u32 getMinScrollX();
	virtual u32 getMinScrollY();
	virtual u32 getMaxScrollX();
	virtual u32 getMaxScrollY();

	virtual void setMinScrollX(u32 minScrollX);
	virtual void setMinScrollY(u32 minScrollY);
	virtual void setMaxScrollX(u32 maxScrollX);
	virtual void setMaxScrollY(u32 maxScrollY);

	virtual u32 getScrollWidth();
	virtual u32 getScrollHeight();

	virtual void draw();
	virtual void draw(Rect clipRect);

	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool keyPress(KeyCode keyCode);
	virtual bool keyRelease(KeyCode keyCode);
	virtual void lidClosed();
	virtual void lidOpened();
	virtual bool focus();
	virtual bool blur();

	virtual void scroll(s32 dx, s32 dy);

protected:
	s32 _scrollX;
	s32 _scrollY;
	s32 _minScrollX;
	s32 _minScrollY;
	s32 _maxScrollX;
	s32 _maxScrollY;

	void scrollLeft(s32 dx);
	void scrollRight(s32 dx);
	void scrollUp(s32 dy);
	void scrollDown(s32 dy);

	void scrollChildren(s32 dx, s32 dy);
	void drawChildren(Rect clipRect);

	bool clipToClientRect(Rect& clipRect);

};

#endif
