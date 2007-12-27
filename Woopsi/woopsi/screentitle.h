#ifndef _SCREEN_TITLE_H_
#define _SCREEN_TITLE_H_

#include <nds.h>
#include "gadget.h"
#include "fontbase.h"
#include "graphicsport.h"

using namespace std;

class ScreenTitle : public Gadget {

public:
	ScreenTitle(u16 height, char* text, FontBase* font = NULL);
	~ScreenTitle();

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);

protected:
	char* _text;
};

#endif
