#ifndef _SLIDER_HORIZONTAL_GRIP_H_
#define _SLIDER_HORIZONTAL_GRIP_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Class representing the grip, or draggable button, with a horizontal
	 * slider.
	 */
	class SliderHorizontalGrip : public Gadget {

	public:
		
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the grip, relative to its parent.
		 * @param y The y co-ordinate of the grip, relative to its parent.
		 * @param width The width of the grip.
		 * @param height The height of the grip.
		 */
		SliderHorizontalGrip(s16 x, s16 y, u16 width, u16 height);

	protected:

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Starts dragging the grip and redraws it.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);
		
		/**
		 * Redraws the grip.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onRelease(s16 x, s16 y);
		
		/**
		 * Redraws the grip.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onReleaseOutside(s16 x, s16 y);
		
		/**
		 * Moves the grip to follow the stylus.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal distance of the drag.
		 * @param vY The vertical distance of the drag.
		 */
		virtual void onDrag(s16 x, s16 y, s16 vX, s16 vY);

		/**
		 * Destructor.
		 */
		virtual inline ~SliderHorizontalGrip() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SliderHorizontalGrip(const SliderHorizontalGrip& sliderHorizontalGrip) : Gadget(sliderHorizontalGrip) { };
	};
}

#endif
