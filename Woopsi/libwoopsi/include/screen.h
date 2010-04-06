#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"
#include "woopsistring.h"

#define SCREEN_TITLE_HEIGHT 13
#define SCREEN_FLIP_BUTTON_WIDTH 16
#define SCREEN_DEPTH_BUTTON_WIDTH 16

namespace WoopsiUI {

	/**
	 * Class representing a basic, empty screen.  All gadgets, except for the Woopsi object
	 * itself, must ultimately have a screen at the top of their hierarchies (below Woopsi).
	 * Screen gadgets can only be children of the Woopsi object.
	 */
	class Screen : public Gadget {

	public:
		/**
		 * Constructor.
		 * @param title The title of the screen; not displayed by default.
		 * @param flags Bitmask specifying some set-up values for the screen.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		Screen(const WoopsiString& title, u32 flags, GadgetStyle* style = NULL);

		/**
		 * Get the screen's title.
		 * @return The screen's title.
		 */
		virtual inline const WoopsiString& getTitle() const { return _title; };

		/**
		 * Swaps the depth of the supplied child gadget.  The child
		 * gadget is assumed to be a window, and although this is not a requirement,
		 * it does mean that child gadgets are depth-swapped as windows would
		 * be expected to.  If the gadget is not at the top of the z-order of the
		 * gadgets is collides with, it is raised above those gadgets.  If it is
		 * at the top of the z-order of the gadgets it collides with, it is
		 * dropped down to the bottom of that z-order.
		 * @param gadget A pointer to the child gadget that needs to swap depths.
		 * @return True if the swap was successful.
		 */
		virtual bool swapGadgetDepth(Gadget* gadget);

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
		 * Set the title of the screen.
		 * @param title The new title.
		 */
		void setTitle(const WoopsiString& title);

		/**
		 * Get the number of the screen that this gadget is currently displayed on.
		 * @return 0 for the touch screen, 1 for the top screen.
		 */
		virtual inline const u8 getPhysicalScreenNumber() const { return calculatePhysicalScreenNumber(getY()); };

	protected:
		u8 _titleHeight;						/**< Height of the title bar */
		WoopsiString _title;					/**< Title of the screen */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Moves the screen vertically to follow the stylus.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal distance of the drag.
		 * @param vY The vertical distance of the drag.
		 */
		virtual void onDrag(s16 x, s16 y, s16 vX, s16 vY);
		
		/**
		 * Raises the screen to the top of the screen stack.
		 */
		virtual void onFocus();

		/**
		 * Destructor.
		 */
		virtual inline ~Screen() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Screen(const Screen& screen) : Gadget(screen) { };
	};
}

#endif
