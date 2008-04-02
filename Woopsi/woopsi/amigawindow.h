#ifndef _AMIGA_WINDOW_H_
#define _AMIGA_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "eventhandler.h"

using namespace std;

class WindowBorderTop;
class WindowBorderSide;
class WindowBorderBottom;
class WindowBorderButton;

class AmigaWindow : public Window, public EventHandler {

public:
	AmigaWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font = NULL);

	virtual void setBorderless(bool isBorderless);

	virtual u8 getBorderSize();
	virtual u8 getTitleHeight();

	virtual void getClientRect(Rect& rect) const;

	virtual void setClickedGadget(Gadget* gadget);

	virtual bool click(s16 x, s16 y);
	virtual bool focus();
	virtual bool blur();

	virtual bool resize(u16 width, u16 height);
	virtual bool handleEvent(const EventArgs& e);

protected:
	char* _title;

	WindowBorderTop* _windowBorderTop;
	WindowBorderSide* _windowBorderLeft;
	WindowBorderSide* _windowBorderRight;
	WindowBorderBottom* _windowBorderBottom;
	WindowBorderButton* _closeButton;
	WindowBorderButton* _depthButton;

	virtual void createBorder();

	/**
	 * Destructor.
	 */
	virtual inline ~AmigaWindow() { }
};

#endif
