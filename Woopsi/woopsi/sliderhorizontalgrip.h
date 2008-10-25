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

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the region of the slider that falls within the
		 * clipping rect.  Use draw() instead in most circumstances.
		 * @param clipRect The clipping region.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Click the grip at the specified co-ordinates.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 * @return True if the click was processed by the slider.
		 */
		virtual bool click(s16 x, s16 y);

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

	protected:

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
