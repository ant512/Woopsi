/* Contributed by Jeff Laing
 * 20071207
 * - Updated by Ant to follow Woopsi code style
 */

#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include "gadget.h"

// Gradient - draws colors bands down the screen
class Gradient : public Gadget {
public:
	Gradient(s16 x, s16 y, u16 width, u16 height, u16 fromColour, u16 toColour);
	virtual inline ~Gradient() {}
	virtual void draw(Rect clipRect);

protected:
	u16 _fromColour, _toColour;
	s16 _numberR, _deltaR;
	s16 _numberG, _deltaG;
	s16 _numberB, _deltaB;
	s16* _rowColour;
	
	virtual void calculate();
	virtual void cacheRowColours();
	virtual bool resize(u16 width, u16 height);
};

#endif
