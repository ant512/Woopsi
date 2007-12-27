#ifndef _SKINNED_WINDOW_BORDER_LEFT_H_
#define _SKINNED_WINDOW_BORDER_LEFT_H_

#include <nds.h>
#include "windowborderside.h"
#include "graphicsport.h"
#include "skin.h"
#include "gadget.h"

using namespace std;

class SkinnedWindowBorderLeft : public WindowBorderSide {
public:
	SkinnedWindowBorderLeft(s16 x, s16 y, u16 height, const WindowSkin* skin);
	virtual ~SkinnedWindowBorderLeft();

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;
};

#endif
