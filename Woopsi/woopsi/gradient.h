/* Contributed by Jeff Laing
 * 20071207
 * - Updated by Ant to follow Woopsi code style
 */

#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include <cstdlib>
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Class providing a vertical gradient-filled background.
	 */
	class Gradient : public Gadget {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param fromColour The start colour for the gradient (top colour).
		 * @param toColour The end colour for the gradient (bottom colour).
		 */
		Gradient(s16 x, s16 y, u16 width, u16 height, u16 fromColour, u16 toColour);
		
		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the region of the gadget that falls within the clipping rectangle.  Should not
		 * be called.
		 * @param clipRect The clipping region to draw within.
		 */
		virtual void draw(Rect clipRect);
		
		/**
		 * Resize the gadget and recalculate the gradient.
		 * @param width The new gadget width.
		 * @param height The new gadget height.
		 */
		virtual bool resize(u16 width, u16 height);

	protected:
		u16 _fromColour;			/**< Initial gradient colour */
		u16 _toColour;				/**< Final gradient colour */
		s16 _numberR;				/**< Number of steps to get from start to finish colour in red component */
		s16 _numberG;				/**< Number of steps to get from start to finish colour in green component */
		s16 _numberB;				/**< Number of steps to get from start to finish colour in blue component */
		s16 _deltaR;				/**< Value that red component is incremented at each new gradient step */
		s16 _deltaG;				/**< Value that green component is incremented at each new gradient step */
		s16 _deltaB;				/**< Value that blue component is incremented at each new gradient step */
		s16* _rowColour;			/**< Array of precalculated gradient colours for each step */
		
		/**
		 * Precalculates the properties of the gradient to save processing time later.
		 */
		virtual void calculate();
		
		/**
		 * Precalculate the gradient colours to save processing time when drawing.
		 */
		virtual void cacheRowColours();

		/**
		 * Destructor.
		 */
		virtual inline ~Gradient() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Gradient(const Gradient& gradient) : Gadget(gradient) { };
	};
}

#endif
