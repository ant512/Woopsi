#ifndef _SKINNED_SCREEN_H_
#define _SKINNED_SCREEN_H_

#include <nds.h>
#include "gadget.h"
#include "skinnedscreentitle.h"
#include "fontbase.h"
#include "screen.h"
#include "skin.h"

using namespace std;

class SkinnedScreenDepthButton;
class SkinnedScreenFlipButton;

class SkinnedScreen : public Screen, public EventHandler {

public:
	SkinnedScreen(char* title, const ScreenSkin* skin);
	virtual ~SkinnedScreen();

	virtual void setBorderless(bool isBorderless);
	virtual bool handleEvent(const EventArgs& e);

protected:
	SkinnedScreenTitle* _screenTitle;
	SkinnedScreenDepthButton* _screenDepthButton;
	SkinnedScreenFlipButton* _screenFlipButton;

	const ScreenSkin* _skin;
};

#endif
