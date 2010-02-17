#ifndef _WINDOW_BORDER_SIDE_H_
#define _WINDOW_BORDER_SIDE_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Class providing a gadget that mimics the appearance of the AmigaOS 3.x
	 * window side border.  Forms part of the AmigaWindow gadget.
	 */
	class WindowBorderSide : public Gadget {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 */
		WindowBorderSide(s16 x, s16 y, u16 width, u16 height);

	protected:
		/**
		 * Destructor.
		 */
		virtual inline ~WindowBorderSide() { };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WindowBorderSide(const WindowBorderSide& windowBorderSide) : Gadget(windowBorderSide) { };
	};
}

#endif
