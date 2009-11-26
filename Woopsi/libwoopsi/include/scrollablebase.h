#ifndef _SCROLLABLE_BASE_H_
#define _SCROLLABLE_BASE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Abstract class providing some basic functionality required by scrolling gadgets.
	 * Scrolling regions are modelled as a virtual "canvas", or rectangular region, with
	 * height/width dimensions and x/y co-ordinates.
	 */
	class ScrollableBase {
	public:
		/**
		 * Constructor.
		 */
		inline ScrollableBase() { }
		
		/**
		 * Gets the x co-ordinate of the virtual canvas.
		 * @return The x co-ordinate of the virtual canvas.
		 */
		virtual inline const s32 getCanvasX() const { return _canvasX; };
		
		/**
		 * Gets the y co-ordinate of the virtual canvas.
		 * @return The y co-ordinate of the virtual canvas.
		 */
		virtual inline const s32 getCanvasY() const { return _canvasY; };

		/**
		 * Gets the width of the virtual canvas.
		 * @return The width of the virtual canvas.
		 */
		virtual inline const s32 getCanvasWidth() const { return _canvasWidth; };
		
		/**
		 * Gets the height of the virtual canvas.
		 * @return The height of the virtual canvas.
		 */
		virtual inline const s32 getCanvasHeight() const { return _canvasHeight; };

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
		inline bool allowsVerticalScroll() const { return _allowVerticalScroll; };

		/**
		 * Returns true if horizontal scrolling is allowed.
		 * @return True if horizontal scrolling is allowed.
		 */
		inline bool allowsHorizontalScroll() const { return _allowHorizontalScroll; };

	protected:
		s32 _canvasX;						/**< X co-ordinate of the vitual canvas. */
		s32 _canvasY;						/**< Y co-ordinate of the virtual canvas. */
		s32 _canvasWidth;					/**< Width of the virtual canvas. */
		s32 _canvasHeight;					/**< Height of the virtual canvas. */

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		inline void setAllowsVerticalScroll(bool allow) { _allowVerticalScroll = allow; };

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		inline void setAllowsHorizontalScroll(bool allow) { _allowHorizontalScroll = allow; };

		/**
		 * Sets the width of the virtual canvas.
		 * @param width The width of the virtual canvas.
		 */
		virtual inline void setCanvasWidth(const s32 width) { _canvasWidth = width; };
		
		/**
		 * Sets the height of the virtual canvas.
		 * @param height The height of the virtual canvas.
		 */
		virtual inline void setCanvasHeight(const s32 height) { _canvasHeight = height; };

		/**
		 * Destructor.
		 */
		virtual inline ~ScrollableBase() { }

	private:
		bool _allowVerticalScroll;			/**< True if vertical scrolling is allowed. */
		bool _allowHorizontalScroll;		/**< True if horizontal scrolling is allowed. */
	};
}

#endif
