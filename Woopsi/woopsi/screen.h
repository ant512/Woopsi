#ifndef _SCREEN_BASE_H_
#define _SCREEN_BASE_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

/**
 * Class representing a basic, empty screen.  All gadgets, except for the Woopsi object
 * itself, must ultimately have a screen at the top of their hierarchies (below Woopsi).
 */
class Screen : public Gadget {

public:
	/**
	 * Constructor.
	 * @param title The title of the screen; not displayed by default.
	 * @param font The font to use with the screen.
	 */
	Screen(char* title, FontBase* font = NULL);
	
	/**
	 * Insert the properties of the space within this gadget that is available
	 * for children into the rect passed in as a parameter.
	 * All co-ordinates are relative to this gadget.
	 * @param rect Reference to a rect to populate with data.
	 */
	virtual void getClientRect(Rect& rect) const;

	/**
	 * Gets the height of the title bar.  Mainly available for subclassing,
	 * as this class does not include a title bar.
	 * @return The height of the title bar.
	 */
	virtual inline const u8 getTitleHeight() const { return _titleHeight; };

	/**
	 * Get the screen's title.
	 * @return The screen's title.
	 */
	virtual inline const char* getTitle() const { return _title; };

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Draw the area of this gadget that falls within the clipping region.
	 * Called by the draw() function to draw all visible regions.
	 * @param clipRect The clipping region to draw.
	 * @see draw()
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Sets the supplied gadget as the focused child.  The gadget must
	 * be a child of this gadget.
	 * @param gadget A pointer to the child gadget.
	 * @see getFocusedGadget()
	 */
	virtual void setFocusedGadget(Gadget* gadget);

	/**
	 * Swaps the depth of the supplied child gadget.
	 * @param gadget A pointer to the child gadget that needs to swap depths.
	 * @return True if the swap was successful.
	 */
	virtual bool swapGadgetDepth(Gadget* gadget);

	/**
	 * Release this gadget at the supplied co-ordinates
	 * @param x X co-ordinate of the release.
	 * @param y Y co-ordinate of the release.
	 * @return True if the release was successful.
	 */
	virtual bool release(s16 x, s16 y);

	/**
	 * Drag the gadget to the supplied co-ordinates.
	 * @param x The x co-ordinate of the stylus.
	 * @param y The y co-ordinate of the stylus.
	 * @param vX The horizontal distance that the stylus was dragged.
	 * @param vY The vertical distance that the stylus was dragged.
	 * @return True if the drag was successful.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	/**
	 * Flip the screen from the bottom physical display to the top display.
	 */
	virtual void flipToTopScreen();

	/**
	 * Flip the screen from the top physical display to the bottom display.
	 */
	virtual void flipToBottomScreen();

	/**
	 * Flip the screen from the current physical display to the other.
	 */
	virtual bool flipScreens();

	/**
	 * Give the gadget focus.
	 * @return True if the gadget received focus correctly.
	 */
	virtual bool focus();

protected:
	u8 _titleHeight;						/**< Height of the title bar */
	char* _title;							/**< Title of the screen */

	/**
	 * Destructor.
	 */
	virtual inline ~Screen() { };
};

#endif
