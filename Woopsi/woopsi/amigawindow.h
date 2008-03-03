#ifndef _AMIGA_WINDOW_H_
#define _AMIGA_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "windowbordertop.h"
#include "windowborderside.h"
#include "windowborderbottom.h"
#include "windowclosebutton.h"
#include "windowdepthbutton.h"
#include "fontbase.h"
#include "graphicsport.h"

using namespace std;

class AmigaWindow : public Window {

public:
	AmigaWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font = NULL);
	virtual ~AmigaWindow();

	virtual void setBorderless(bool isBorderless);

	virtual u8 getBorderSize();
	virtual u8 getTitleHeight();

	virtual void getClientRect(Rect& rect) const;

	virtual void setClickedGadget(Gadget* gadget);

	virtual bool click(s16 x, s16 y);
	virtual bool focus();
	virtual bool blur();

	virtual bool resize(u16 width, u16 height);

protected:
	char* _title;

	WindowBorderTop* _windowBorderTop;
	WindowBorderSide* _windowBorderLeft;
	WindowBorderSide* _windowBorderRight;
	WindowBorderBottom* _windowBorderBottom;
	WindowCloseButton* _windowCloseButton;
	WindowDepthButton* _windowDepthButton;

	virtual void createBorder();
};

#endif
