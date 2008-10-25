#ifndef _SCROLLBAR_VERTICAL_H_
#define _SCROLLBAR_VERTICAL_H_

#include <nds.h>
#include "gadget.h"
#include "eventhandler.h"

namespace WoopsiUI {

	class SliderVertical;
	class Button;
	class WoopsiTimer;

	/**
	 * Container class that holds a slider gadget and two arrow buttons.
	 * The interface is presents is virtually identical to the SliderVertical
	 * gadget, which means the two are easily interchangeable.  All events
	 * raised by the internal slider gadget are re-raised by this gadget
	 * to this gadget's event handler, meaning its events are also identical
	 * to the SliderVertical's.
	 */
	class ScrollbarVertical : public Gadget, public EventHandler  {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ord of the slider, relative to its parent.
		 * @param y The y co-ord of the slider, relative to its parent.
		 * @param width The width of the slider.
		 * @param height The height of the slider.
		 * @param font The font to use for this gadget.
		 */
		ScrollbarVertical(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

		/**
		 * Get the smallest value that the slider can represent.
		 * @return The smallest value.
		 */
		const s16 getMinimumValue() const;

		/**
		 * Get the largest value that the slider can represent.
		 * @return The largest value.
		 */
		const s16 getMaximumValue() const;

		/**
		 * Get the current value of the slider.
		 * return The current slider value.
		 */
		const s16 getValue() const;

		/**
		 * Get the value represented by the height of the grip.
		 * For sliders, this would typically be 1 (so each new
		 * grip position is worth 1).  For scrollbars, this
		 * would be the height of the scrolling gadget.
		 * @return The page size.
		 */
		const s16 getPageSize() const;

		/**
		 * Set the smallest value that the slider can represent.
		 * @param value The smallest value.
		 */
		void setMinimumValue(const s16 value);

		/**
		 * Set the largest value that the slider can represent.
		 * @param value The largest value.
		 */
		void setMaximumValue(const s16 value);

		/**
		 * Set the value that of the slider.  This will reposition
		 * and redraw the grip.
		 * @param value The new value.
		 */
		void setValue(const s16 value);

		/**
		 * Set the page size represented by the grip.
		 * @param pageSize The page size.
		 * @see getPageSize().
		 */
		void setPageSize(const s16 pageSize);

		/**
		 * Set the amount scrolled by the arrow buttons
		 * @param buttonScrollAmount The amount to scroll.
		 */
		void setButtonScrollAmount(const u16 buttonScrollAmount);

		/**
		 * Resize and redraw the grip.
		 */
		void resizeGrip();

		/**
		 * Make the grip jump up or down the gutter.
		 * @param direction 0 to jump up, 1 to jump down.
		 */
		void jumpGrip(u8 direction);

		/**
		 * Draw the slider.
		 */
		virtual void draw();

		/**
		 * Draw the region of the slider that falls within the
		 * clipping rect.  Use draw() instead in most circumstances.
		 * @param clipRect The clipping region.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Process events fired by the grip.
		 * @param e The event details.
		 * @return True if the event was processed.
		 */
		virtual bool handleEvent(const EventArgs& e);

		/**
		 * Resize the scrollbar to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

	protected:
		SliderVertical* _slider;					/**< Pointer to the slider gadget */
		Button* _upButton;							/**< Pointer to the up button */
		Button* _downButton;						/**< Pointer to the down button */
		u8 _buttonHeight;							/**< Height of the buttons */
		u16 _buttonScrollAmount;					/**< Amount that the grip moves when a button is clicked */
		u8 _scrollTimeout;							/**< VBLs needed until a button triggers another grip movement */
		WoopsiTimer* _timer;						/**< Controls slider button repeats */

		/**
		 * Destructor.
		 */
		virtual inline ~ScrollbarVertical() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollbarVertical(const ScrollbarVertical& scrollbarVertical) : Gadget(scrollbarVertical) { };
	};
}

#endif
