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
		 * Set the title of the window.
		 * @param title The new title.
		 */
		void setTitle(const WoopsiString& title);

		/**
		 * Get the title of the window.
		 * @return The title of the window.
		 */
		const WoopsiString& getTitle() { return _title; };

	protected:
		WoopsiString _title;							/**< Title of the window */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Draws a XOR rect around the window.
		 */
		virtual void onDragStart();
		
		/**
		 * Draws the dragging XOR rect at the new co-ordinates.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal distance dragged.
		 * @param vY The vertical distance dragged.
		 */
		virtual void onDrag(s16 x, s16 y, s16 vX, s16 vY);
		
		/**
		 * Erases the XOR rect and moves the window to the new co-ordinates.
		 */
		virtual void onDragStop();

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
