#ifndef _WINDOW_BASE_H_
#define _WINDOW_BASE_H_

#include <nds.h>
#include "gadget.h"
#include "fontbase.h"
#include "graphicsport.h"

#define WINDOW_TITLE_HEIGHT 13
#define WINDOW_BORDER_SIZE 4
#define WINDOW_CLOSE_BUTTON_WIDTH 14
#define WINDOW_DEPTH_BUTTON_WIDTH 14

using namespace std;

class Window : public Gadget {

public:
	Window(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font = NULL);

	virtual void draw();
	virtual void draw(Rect clipRect);

	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

protected:
	char* _title;

	/**
	 * Destructor.
	 */
	virtual inline ~Window() { };
};

#endif
