#ifndef _AMIGA_SCREEN_H_
#define _AMIGA_SCREEN_H_

#include <nds.h>
#include "eventhandler.h"
#include "screen.h"

using namespace std;

class DecorationGlyphButton;
class ScreenFlipButton;
class ScreenTitle;

/**
 * Class providing a screen with an Amiga-style title bar.  Unlike the AmigaWindow class,
 * the title bar in this class can be overlapped by child gadgets.  Clicking and dragging
 * the title bar causes the screen to drop down the DS' display, revealing any screens
 * below it.  The screen has depth swap and display flip buttons.
 *
 * All gadgets should be a child/grandchild/etc of a screen except for other screens
 * and the Woopsi instance.
 */
class AmigaScreen : public Screen, public EventHandler {

public:

	/**
	 * Constructor.
	 */
	AmigaScreen(char* title, FontBase* font = NULL);

	/**
	 * Sets this gadget's border state.  Setting to false hides the title bar.
	 * @param isBorderless The border state.
	 */
	virtual void setBorderless(bool isBorderless);

	/**
	 * Handles events fired by the decoration gadgets.
	 * @param e The event arguments.
	 */
	virtual bool handleEvent(const EventArgs& e);

protected:
	ScreenTitle* _screenTitle;						/**< Title of the screen */
	DecorationGlyphButton* _depthButton;			/**< Pointer to the screen's depth button */
	DecorationGlyphButton* _flipButton;				/**< Pointer to the screen's flip button */

	/**
	 * Destructor.
	 */
	virtual inline ~AmigaScreen() { };
};

#endif
