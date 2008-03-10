#ifndef _SCROLLING_PANEL_H_
#define _SCROLLING_PANEL_H_

#include <cstdlib>
#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"
#include "scrollablebase.h"

using namespace std;

/**
 * Class containing a scrollable region.  Responds to stylus movement.  Uses hardware-
 * accelerated copying to reduce the amount of drawing required, where possible.  Can
 * contain sub-gadgets which will also be scrolled.
 */
class ScrollingPanel : public Gadget, public ScrollableBase {
public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the gadget.
	 * @param y The y co-ordinate of the gadget.
	 * @param width The width of the gadget.
	 * @param height The height of the gadget.
	 * @param flags The usual gadget flags.
	 * @param font Pointer to a font object to use for text output.
	 */
	ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font = NULL);
	
	/**
	 * Destructor.
	 */
	virtual ~ScrollingPanel();

	/**
	 * Draw the visible portions of the gadget and its children.
	 */
	virtual void draw();
	
	/**
	 * Draw a single region of the gadget.  Should not be called.
	 * @param clipRect The region to draw.
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Click the gadget at the specified co-ordinates.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);
	
	/**
	 * Release the gadget at the specified co-ordinates.
	 * @param x The x co-ordinate of the release.
	 * @param y The y co-ordinate of the release.
	 * @return True if the release was successful.
	 */
	virtual bool release(s16 x, s16 y);
	
	/**
	 * Drag the gadget at the specified co-ordinates.
	 * @param x The x co-ordinate of the drag.
	 * @param y The y co-ordinate of the drag.
	 * @param vX The horizontal distance that the gadget was dragged.
	 * @param vY The vertical distance that the gadget was dragged.
	 * @return True if the drag was successful.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	
	/**
	 * Send a keypress to the gadget.
	 * @param keyCode The code of the keypress to send.
	 * @return True if the keypress was handled by the gadget.
	 */
	virtual bool keyPress(KeyCode keyCode);
	
	/**
	 * Send a key release to the gadget.
	 * @param keyCode The code of the key release to send.
	 * @return True if the key release was handled by the gadget.
	 */
	virtual bool keyRelease(KeyCode keyCode);
	
	/**
	 * Tell the gadget that the lid has closed.
	 */
	virtual void lidClosed();
	
	/**
	 * Tell the gadget that the lid has opened.
	 */
	virtual void lidOpened();
	
	/**
	 * Send the focus to the gadget.
	 * @return True if the gadget took the focus.
	 */
	virtual bool focus();
	
	/**
	 * Remove the focus from the gadget.
	 * @return True if the gadget lost the focus.
	 */
	virtual bool blur();

	/**
	 * Scroll the panel by the specified amounts.
	 * @param dx The horizontal distance to scroll.
	 * @param dy The vertical distance to scroll.
	 */
	virtual void scroll(s32 dx, s32 dy);
	
	/**
	 * Reposition the panel's scrolling region to the specified co-ordinates.
	 * @param x The new x co-ordinate of the scrolling region.
	 * @param y The new y co-ordinate of the scrolling region.
	 */
	virtual void jump(s32 x, s32 y);

protected:

	/**
	 * Scroll the panel left by the specified amount.
	 * @param dx The amount to scroll.
	 */
	void scrollLeft(s32 dx);
	
	/**
	 * Scroll the panel right by the specified amount.
	 * @param dx The amount to scroll.
	 */
	void scrollRight(s32 dx);
	
	/**
	 * Scroll the panel up by the specified amount.
	 * @param dy The amount to scroll.
	 */
	void scrollUp(s32 dy);
	
	/**
	 * Scroll the panel dpwn by the specified amount.
	 * @param dy The amount to scroll.
	 */
	void scrollDown(s32 dy);

	/**
	 * Scroll all child gadgets by the specified amounts.  Actually uses the gadget's
	 * moveTo() function to reposition them.
	 * @param dx The horizontal distance to scroll.
	 * @param dy The vertical distance to scroll.
	 */
	void scrollChildren(s32 dx, s32 dy);
	
	/**
	 * Draws all child gadgets within the supplied clipping region.
	 * @param clipRect The clipping region to draw within.
	 */
	void drawChildren(Rect clipRect);

	/**
	 * Clips the properties of the supplied rect so that it fits within the panel's
	 * client region.
	 * @param clipRect The rect to clip.
	 */
	bool clipToClientRect(Rect& clipRect);

	/**
	 * Raises a scroll event.  Fired when the panel scrolls.
	 */
	void raiseScrollEvent();
};

#endif
