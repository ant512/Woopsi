#ifndef _SKINNED_SCREEN_H_
#define _SKINNED_SCREEN_H_

#include <nds.h>
#include "screen.h"
#include "skin.h"

using namespace std;

class SkinnedScreenDepthButton;
class SkinnedScreenFlipButton;
class SkinnedScreenTitle;

class SkinnedScreen : public Screen, public EventHandler {

public:
	SkinnedScreen(char* title, const ScreenSkin* skin);

	virtual void setBorderless(bool isBorderless);
	virtual bool handleEvent(const EventArgs& e);

protected:
	SkinnedScreenTitle* _screenTitle;
	SkinnedScreenDepthButton* _screenDepthButton;
	SkinnedScreenFlipButton* _screenFlipButton;

	const ScreenSkin* _skin;

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedScreen() { };
};

#endif
