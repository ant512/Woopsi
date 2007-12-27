#ifndef _SKINNED_WINDOW_BORDER_RIGHT_H_
#define _SKINNED_WINDOW_BORDER_RIGHT_H_

#include <nds.h>
#include "windowborderside.h"
#include "graphicsport.h"
#include "skin.h"
#include "gadget.h"

using namespace std;

class SkinnedWindowBorderRight : public WindowBorderSide {
public:
	SkinnedWindowBorderRight(s16 x, s16 y, u16 height, const WindowSkin* skin);
	virtual ~SkinnedWindowBorderRight();

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;
};

#endif
