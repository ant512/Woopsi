#ifndef _WINDOW_BASE_H_
#define _WINDOW_BASE_H_

#include <nds.h>
#include "gadget.h"
#include "fontbase.h"
#include "graphicsport.h"
#include "gadgetstyle.h"

#define WINDOW_TITLE_HEIGHT 13
#define WINDOW_BORDER_SIZE 4
#define WINDOW_CLOSE_BUTTON_WIDTH 14
#define WINDOW_DEPTH_BUTTON_WIDTH 14

namespace WoopsiUI {

	/**
	 * Class representing a basic, empty window.  Intended to be subclassed, but can be used
	 * as-is if necessary.
	 */
	class Window : public Gadget {

	public:
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the window.
		 * @param y The y co-ordinate of the window.
		 * @param width The width of the window.
		 * @param height The height of the window.
		 * @param title The title of the window.
		 * @param flags Standard gadget flags bitmask.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		Window(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, u32 flags, GadgetStyle* style = NULL);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

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
		 * Set the title of the window.
		 * @param title The new title.
		 */
		void setTitle(const WoopsiString& title);

		/**
		 * Get the title of the window.
		 * @return The title of the window.
		 */
		const WoopsiString& getTitle() { return _title; };

		/**
		 * Notify this gadget that it is being dragged, and set its drag point.
		 * @param x The x co-ordinate of the drag position relative to this gadget.
		 * @param y The y co-ordinate of the drag position relative to this gadget.
		 */
		virtual void setDragging(u16 x, u16 y);

	protected:
		WoopsiString _title;							/**< Title of the window */

		/**
		 * Destructor.
		 */
		virtual inline ~Window() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Window(const Window& window) : Gadget(window) { };
	};
}

#endif
