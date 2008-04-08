#ifndef _SLIDER_HORIZONTAL_H_
#define _SLIDER_HORIZONTAL_H_

#include <nds.h>
#include "gadget.h"
#include "eventhandler.h"

using namespace std;

class SliderHorizontalGrip;

/**
 * Gadget providing a sliding "grip" that can be moved left and
 * right in the "gutter".  Essentially a scrollbar, but more
 * generic.
 */
class SliderHorizontal : public Gadget, public EventHandler {

public:

	/**
	 * Constructor.
	 * @param x The x co-ord of the slider, relative to its parent.
	 * @param y The y co-ord of the slider, relative to its parent.
	 * @param width The width of the slider.
	 * @param height The height of the slider.
	 */
	SliderHorizontal(s16 x, s16 y, u16 width, u16 height);

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
	const s16 getValue() const;

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
	inline void setMinimumValue(const s16 value) { _minimumValue = value; };

	/**
	 * Set the largest value that the slider can represent.
	 * @param value The largest value.
	 */
	inline void setMaximumValue(const s16 value) { _maximumValue = value; };

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
	inline void setPageSize(const s16 pageSize) { _pageSize = pageSize; };

	/**
	 * Resize and redraw the grip.
	 */
	void resizeGrip();

	/**
	 * Make the grip jump left or right along the gutter.
	 * @param direction 0 to jump left, 1 to jump right.
	 */
	void jumpGrip(u8 direction);

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
	 * Click the slider at the specified co-ordinates.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if the click was processed by the slider.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Drag the slider at the specified co-ordinates.
	 * @param x The x co-ordinate of the drag.
	 * @param y The y co-ordinate of the drag.
	 * @param vX The x distance of the drag.
	 * @param vY The y distance of the drag.
	 * @return True if the drag was processed by the slider.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	/**
	 * Process events fired by the grip.
	 * @param e The event details.
	 * @return True if the event was processed.
	 */
	virtual bool handleEvent(const EventArgs& e);

	/**
	 * Resize the slider to the new dimensions.
	 * @param width The new width.
	 * @param height The new height.
	 * @return True if the resize was successful.
	 */
	virtual bool resize(u16 width, u16 height);

protected:
	SliderHorizontalGrip* _grip;
	s16 _minimumValue;
	s16 _maximumValue;
	s16 _minimumGripWidth;
	s16 _pageSize;

	/**
	 * Destructor.
	 */
	virtual inline ~SliderHorizontal() { };
};

#endif
