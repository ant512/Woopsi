#ifndef _SCREEN_BASE_H_
#define _SCREEN_BASE_H_

#include <nds.h>
#include "gadget.h"
#include "fontbase.h"
#include "dynamicarray.h"

using namespace std;

class Screen : public Gadget {

public:
	Screen(char* title, FontBase* font = NULL);
	virtual ~Screen();
	
	virtual void getClientRect(Rect& rect) const;

	virtual void setBorderless(bool isBorderless);

	virtual u8 getTitleHeight();
	virtual char* getTitle();
	virtual void drag();
	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual void drawChildren(Rect clipRect);

	virtual void setActiveGadget(Gadget* gadget);

	virtual bool swapGadgetDepth(Gadget* gadget);

	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool keyPress(KeyCode keyCode);
	virtual bool keyRelease(KeyCode keyCode);
	virtual void lidClosed();
	virtual void lidOpened();
	virtual void flipToTopScreen();
	virtual void flipToBottomScreen();
	virtual bool flipScreens();

protected:
	u8 _titleHeight;
	char* _title;

	void setActive(bool active);
};

#endif
