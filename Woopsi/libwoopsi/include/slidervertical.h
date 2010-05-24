#ifndef _SLIDER_VERTICAL_H_
#define _SLIDER_VERTICAL_H_

#include <nds.h>
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "sliderbase.h"

namespace WoopsiUI {

	class SliderVerticalGrip;

	/**
	 * Gadget providing a sliding "grip" that can be moved up and
	 * down the "gutter".  Essentially a scrollbar, but more
	 * generic.
	 */
	class SliderVertical : public SliderBase, public Gadget, public GadgetEventHandler {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ord of the slider, relative to its parent.
		 * @param y The y co-ord of the slider, relative to its parent.
		 * @param width The width of the slider.
		 * @param height The height of the slider.
		 */
		SliderVertical(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Get the smallest value that the slider can represent.
		 * @return The smallest value.
		 */
		inline const s16 getMinimumValue() const { return _minimumValue; };

		/**
		 * Get the largest value that the slider can represent.
		 * @return The largest value.
		 */
		inline const s16 getMaximumValue() const { return _maximumValue; };

		/**
		 * Get the current value of the slider.
		 * return The current slider value.
		 */
		inline const s16 getValue() const { return _value; };

		/**
		 * Get the value represented by the height of the grip.
		 * For sliders, this would typically be 1 (so each new
		 * grip position is worth 1).  For scrollbars, this
		 * would be the height of the scrolling gadget.
		 * @return The page size.
		 */
		inline const s16 getPageSize() const { return _pageSize; };

		/**
		 * Set the smallest value that the slider can represent.
		 * @param value The smallest value.
		 */
		inline void setMinimumValue(const s16 value) {
			_minimumValue = value;
			_contentSize = _maximumValue - _minimumValue + 1;
			resizeGrip();
		};

		/**
		 * Set the largest value that the slider can represent.
		 * @param value The largest value.
		 */
		inline void setMaximumValue(const s16 value) {
			_maximumValue = value;
			_contentSize = _maximumValue - _minimumValue + 1;
			resizeGrip();
		};

		/**
		 * Set the value that of the slider.  This will reposition
		 * and redraw the grip.
		 * @param value The new value.
		 */
		void setValue(const s16 value);
		
		/**
		 * Set the value that of the slider.  This will reposition and redraw
		 * the grip.  The supplied value should be bitshifted left 16 places.
		 * This ensures greater accuracy than the standard setValue() method if
		 * the slider is being used as a scrollbar.
		 * @param value The new value.
		 */
		void setValueWithBitshift(const s32 value);

		/**
		 * Set the page size represented by the grip.
		 * @param pageSize The page size.
		 * @see getPageSize().
		 */
		inline void setPageSize(const s16 pageSize) {
			_pageSize = pageSize;
			resizeGrip();
		};

		/**
		 * Process events fired by the grip.
		 * @param e The event details.
		 */
		virtual void handleDragEvent(const GadgetEventArgs& e);

		/**
		 * Get the smallest value that the slider can move through when
		 * dragged.
		 * @return The smallest value that the slider can move through when
		 * dragged.
		 */
		s16 getMinimumStep() const;

	protected:
		SliderVerticalGrip* _grip;					/**< Pointer to the grip. */
		s16 _minimumValue;							/**< Minimum value that the grip can represent. */
		s16 _maximumValue;							/**< Maximum value that the grip can represent. */
		s16 _value;									/**< Current value of the slider. */
		s16 _minimumGripHeight;						/**< Smallest height that the grip can become. */
		s16 _pageSize;								/**< Value of a page of data, used when clicking the gutter */
		s32 _gutterHeight;							/**< Height of the gutter, taking into account
														 any adjustments made to the height of the grip. */
		u32 _contentSize;							/**< Number of values in the min/max range. */

		/**
		 * Get the maximum possible value that the slider can represent.  Useful when
		 * using the slider as a scrollbar, as the height of the grip prevents the full
		 * range of values being accessed (intentionally).
		 * The returned value is bitshfted left 16 places for more accuracy in fixed-point
		 * calculations.
		 * @return The maximum possible value that the slider can represent.
		 */
		s32 getPhysicalMaximumValueWithBitshift() const;

		/**
		 * Get the value represented by the top of the grip.
		 * return The value represented by the top of the grip.
		 */
		const s16 getGripValue() const;

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
		 * Resize the slider to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);

		/**
		 * Moves the grip towards the stylus.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Destructor.
		 */
		virtual inline ~SliderVertical() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SliderVertical(const SliderVertical& sliderVertical) : Gadget(sliderVertical) { };

		/**
		 * Resize and redraw the grip.
		 */
		void resizeGrip();
	};
}

#endif
