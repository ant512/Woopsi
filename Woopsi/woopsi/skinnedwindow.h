#ifndef _SKINNED_WINDOW_H_
#define _SKINNED_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "skin.h"
#include "eventhandler.h"

using namespace std;

class SkinnedWindowDepthButton;
class SkinnedWindowCloseButton;
class SkinnedWindowBorderBottom;
class SkinnedWindowBorderLeft;
class SkinnedWindowBorderRight;
class SkinnedWindowBorderTop;

class SkinnedWindow : public Window, public EventHandler  {

public:

	/**
	 * Enum listing flags that can be set in the constructor's "windowFlags" parameter.
	 */
	enum SkinnedWindowFlagType {
		SKINNED_WINDOW_SHOW_CLOSE = 0x0001,					/**< Window shows close button */
		SKINNED_WINDOW_SHOW_DEPTH = 0x0002					/**< Window shows depth button */
	};

	/**
	 * Struct describing some basic properties of an AmigaWindow.
	 */
	typedef struct {
		u8 showClose : 1;					/**< True if the close button is visible. */
		u8 showDepth : 1;					/**< True if the depth button is visible. */
	} SkinnedWindowFlags;

	SkinnedWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, u32 windowFlags, WindowSkin* skin);

	virtual void setBorderless(bool isBorderless);

	virtual const u8 getBorderSize() const;
	virtual const u8 getTitleHeight() const;

	virtual void getClientRect(Rect& rect) const;

	virtual void setClickedGadget(Gadget* gadget);

	virtual bool click(s16 x, s16 y);
	virtual bool focus();
	virtual bool blur();

	virtual bool resize(u16 width, u16 height);
	virtual bool handleEvent(const EventArgs& e);

protected:
	char* _title;
	const WindowSkin* _skin;
	SkinnedWindowFlags _windowFlags;					/**< AmigaWindow-specific flags */

	SkinnedWindowBorderTop* _windowBorderTop;
	SkinnedWindowBorderLeft* _windowBorderLeft;
	SkinnedWindowBorderRight* _windowBorderRight;
	SkinnedWindowBorderBottom* _windowBorderBottom;
	SkinnedWindowCloseButton* _closeButton;
	SkinnedWindowDepthButton* _depthButton;

	virtual void createBorder();

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedWindow() { };
};

#endif
