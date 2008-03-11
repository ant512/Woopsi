/* Contributed by Jeff Laing
 * 20071207
 * - Updated by Ant to follow Woopsi code style
 */

#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include <cstdlib>
#include "gadget.h"

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
	 * Destructor.
	 */
	virtual inline ~Gradient() {}
	
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
	u16 _fromColour, _toColour;
	s16 _numberR, _deltaR;
	s16 _numberG, _deltaG;
	s16 _numberB, _deltaB;
	s16* _rowColour;
	
	/**
	 * Precalculates the properties of the gradient to save processing time later.
	 */
	virtual void calculate();
	
	/**
	 * Precalculate the gradient colours to save processing time when drawing.
	 */
	virtual void cacheRowColours();
};

#endif
