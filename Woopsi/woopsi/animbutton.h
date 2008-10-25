#ifndef _ANIM_BUTTON_H_
#define _ANIM_BUTTON_H_

#include <nds.h>
#include "gadget.h"
#include "animation.h"
#include "eventhandler.h"

namespace WoopsiUI {

	class WoopsiTimer;

	/**
	 * Button class that has an animation running in its clickable area.  Note that the
	 * bitmaps used in the animation should all be the same size.
	 */
	class AnimButton : public Gadget, public EventHandler {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the button.
		 * @param y The y co-ordinate of the button.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param animX The x co-ordinate at which the animation's bitmaps will be drawn.
		 * @param animY The y co-ordinate at which the animation's bitmaps will be drawn.
		 */
		AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the region of the button that falls within the clipRect.
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
		Animation* _animNormal;				/**< Animation played when button is not clicked */
		Animation* _animClicked;			/**< Animation played when button is clicked */
		u16 _animX;							/**< X co-ordinate of the animations */
		u16 _animY;							/**< Y co-ordinate of the animations */
		bool _initialised;					/**< Tracks if the animation has started or not */
		WoopsiTimer* _timer;				/**< Controls animation timing and playback */

		/**
		 * Destructor.
		 */
		virtual ~AnimButton();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline AnimButton(const AnimButton& animButton) : Gadget(animButton) { };

		/**
		 * Handle any child events.
		 * @return True if the event was processed correctly.
		 */
		bool handleEvent(const EventArgs& e);
	};
}

#endif
