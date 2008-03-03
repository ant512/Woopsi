#ifndef _SKINNED_WINDOW_H_
#define _SKINNED_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "skinnedwindowbordertop.h"
#include "skinnedwindowborderleft.h"
#include "skinnedwindowborderright.h"
#include "skinnedwindowborderbottom.h"
#include "skinnedwindowclosebutton.h"
#include "skinnedwindowdepthbutton.h"
#include "fontbase.h"
#include "graphicsport.h"
#include "skin.h"
#include "gadget.h"

using namespace std;

class SkinnedWindow : public Window {

public:
	SkinnedWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, WindowSkin* skin);
	virtual ~SkinnedWindow();

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
	const WindowSkin* _skin;

	SkinnedWindowBorderTop* _windowBorderTop;
	SkinnedWindowBorderLeft* _windowBorderLeft;
	SkinnedWindowBorderRight* _windowBorderRight;
	SkinnedWindowBorderBottom* _windowBorderBottom;
	SkinnedWindowCloseButton* _windowCloseButton;
	SkinnedWindowDepthButton* _windowDepthButton;

	virtual void createBorder();
};

#endif
