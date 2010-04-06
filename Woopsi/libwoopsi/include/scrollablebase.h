#ifndef _SCROLLABLE_BASE_H_
#define _SCROLLABLE_BASE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Abstract class defining basic functionality of scrolling gadgets.
	 * Scrolling regions are modelled as a virtual "canvas", or rectangular region, with
	 * height/width dimensions and x/y co-ordinates.
	 */
	class ScrollableBase {
	public:
		/**
		 * Constructor.
		 */
		inline ScrollableBase() { };
		
		/**
		 * Gets the x co-ordinate of the virtual canvas.
		 * @return The x co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasX() const = 0;
		
		/**
		 * Gets the y co-ordinate of the virtual canvas.
		 * @return The y co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasY() const = 0;

		/**
		 * Gets the width of the virtual canvas.
		 * @return The width of the virtual canvas.
		 */
		virtual const s32 getCanvasWidth() const = 0;
		
		/**
		 * Gets the height of the virtual canvas.
		 * @return The height of the virtual canvas.
		 */
		virtual const s32 getCanvasHeight() const = 0;

		/**
		 * Scrolls the virtual canvas by the specified amounts.
		 * @param dx Distance to scroll horizontally.
		 * @param dy Distance to scroll vertically.
		 */
		virtual void scroll(s32 dx, s32 dy) = 0;
		
		/**
		 * Repositions the virtual canvas to the specified co-ordinates.
		 * @param x New x co-ordinate of the virtual canvas.
		 * @param y New y co-ordinate of the virtual canvas.
		 */
		virtual void jump(s32 x, s32 y) = 0;

		/**
		 * Returns true if vertical scrolling is allowed.
		 * @return True if vertical scrolling is allowed.
		 */
		virtual bool allowsVerticalScroll() const = 0;

		/**
		 * Returns true if horizontal scrolling is allowed.
		 * @return True if horizontal scrolling is allowed.
		 */
		virtual bool allowsHorizontalScroll() const = 0;

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsVerticalScroll(bool allow) = 0;

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsHorizontalScroll(bool allow) = 0;

		/**
		 * Sets the width of the virtual canvas.
		 * @param width The width of the virtual canvas.
		 */
		virtual void setCanvasWidth(const s32 width) = 0;
		
		/**
		 * Sets the height of the virtual canvas.
		 * @param height The height of the virtual canvas.
		 */
		virtual void setCanvasHeight(const s32 height) = 0;

		/**
		 * Destructor.
		 */
		virtual inline ~ScrollableBase() { };
	};
}

#endif
