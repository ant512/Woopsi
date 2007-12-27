#ifndef _SKINNED_WINDOW_BORDER_TOP_H_
#define _SKINNED_WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "windowbordertop.h"
#include "fontbase.h"
#include "graphicsport.h"
#include "skin.h"
#include "gadget.h"

using namespace std;

class SkinnedWindowBorderTop : public WindowBorderTop {

public:
	SkinnedWindowBorderTop(s16 x, u16 width, char* text, const WindowSkin* skin);
	virtual ~SkinnedWindowBorderTop();

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;
};

#endif
