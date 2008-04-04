#ifndef _ANIM_BUTTON_H_
#define _ANIM_BUTTON_H_

#include <nds.h>
#include "textbox.h"
#include "animation.h"

using namespace std;

/**
 * Button class that has an animation running in its clickable area.  Note that the
 * bitmaps used in the animation should all be the same size.
 */
class AnimButton : public Textbox {

public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the button.
	 * @param y The y co-ordinate of the button.
	 * @param width The width of the button.
	 * @param height The height of the button.
	 * @param bitmapX The x co-ordinate at which the animation's bitmaps will be drawn.
	 * @param bitmapY The y co-ordinate at which the animation's bitmaps will be drawn.
	 */
	AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY);

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Draw the region of the button that falls within the clipRect.  Should not be
	 * called.
	 * @param clipRect The clipping region to draw within.
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Get a pointer to the animation that plays when the button is not clicked.
	 * @return Pointer to the normal animation.
	 */
	virtual Animation* const getNormalAnimation();
	
	/**
	 * Get a pointer to the animation that plays when the button is clicked.
	 * @return Pointer to the clicked animation.
	 */
	virtual Animation* const getClickedAnimation();
	
	/**
	 * Handle a VBL.
	 * @return True if the VBL was processed correctly.
	 */
	bool vbl();
	
	/**
	 * Click the button at the specified co-ordinates.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	bool click(s16 x, s16 y);
	
	/**
	 * Click the button at the specified co-ordinates.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	bool release(s16 x, s16 y);

protected:
	Animation* _animNormal;
	Animation* _animClicked;
	u16 _animX;
	u16 _animY;
	bool _initialised;

	/**
	 * Destructor.
	 */
	virtual ~AnimButton();
};

#endif
