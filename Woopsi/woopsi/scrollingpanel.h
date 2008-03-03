#ifndef _SCROLLING_PANEL_H_
#define _SCROLLING_PANEL_H_

#include <cstdlib>
#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"
#include "scrollablebase.h"

using namespace std;

class ScrollingPanel : public Gadget, public ScrollableBase {
public:
	ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font = NULL);
	virtual ~ScrollingPanel();

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
	virtual void jump(s32 x, s32 y);

protected:
	void scrollLeft(s32 dx);
	void scrollRight(s32 dx);
	void scrollUp(s32 dy);
	void scrollDown(s32 dy);

	void scrollChildren(s32 dx, s32 dy);
	void drawChildren(Rect clipRect);

	bool clipToClientRect(Rect& clipRect);

	void raiseScrollEvent();
};

#endif
