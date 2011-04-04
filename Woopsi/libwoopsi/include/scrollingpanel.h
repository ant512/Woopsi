#ifndef _SCROLLING_PANEL_H_
#define _SCROLLING_PANEL_H_

#include <cstdlib>
#include <nds.h>
#include "gadget.h"
#include "scrollablebase.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Class containing a scrollable region.  Responds to stylus movement.  Uses hardware-
	 * accelerated copying to reduce the amount of drawing required, where possible.  Can
	 * contain sub-gadgets which will also be scrolled.
	 */
	class ScrollingPanel : public Gadget, public ScrollableBase {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ScrollingPanel(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style = NULL);

		/**
		 * Scroll the panel by the specified amounts.
		 * @param dx The horizontal distance to scroll.
		 * @param dy The vertical distance to scroll.
		 */
		virtual void scroll(s32 dx, s32 dy);
		
		/**
		 * Reposition the panel's scrolling region to the specified co-ordinates.
		 * @param x The new x co-ordinate of the scrolling region.
		 * @param y The new y co-ordinate of the scrolling region.
		 */
		virtual void jump(s32 x, s32 y);

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
		 * Set whether or not the content of the panel is scrolled.
		 * Content is anything drawn to the panel in the draw() method.
		 * This property is disabled by default, which will result in
		 * faster scrolling of child objects.
		 * If the panel contains no child objects, just draw() method
		 * content, consider using a SuperBitmap class instead.
		 * @param scrolled True to enable content scrolling; false to disable it.
		 */
		inline void setContentScrolled(bool scrolled) { _isContentScrolled = scrolled; };

		/**
		 * Check if the content of the panel, drawn via the draw() method,
		 * is scrolled.
		 * @return True if the content is scrolled; false if not.
		 */
		inline bool IsContentScrolled() { return _isContentScrolled; };

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

	protected:
		s32 _canvasX;						/**< X co-ordinate of the vitual canvas. */
		s32 _canvasY;						/**< Y co-ordinate of the virtual canvas. */
		s32 _canvasWidth;					/**< Width of the virtual canvas. */
		s32 _canvasHeight;					/**< Height of the virtual canvas. */
		bool _allowVerticalScroll;			/**< True if vertical scrolling is allowed. */
		bool _allowHorizontalScroll;		/**< True if horizontal scrolling is allowed. */
		bool _isContentScrolled;			/**< True if the content drawn to the panel is scrolled
											     (ie. everything drawn in the draw() method);
												 false if just child objects are scrolled. */

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
		 * Scrolls the panel to match the drag.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal drag distance.
		 * @param vY The vertical drag distance.
		 */
		virtual void onDrag(s16 x, s16 y, s16 vX, s16 vY);
		
		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Scroll all child gadgets by the specified amounts.  Actually uses the gadget's
		 * moveTo() function to reposition them.
		 * @param dx The horizontal distance to scroll.
		 * @param dy The vertical distance to scroll.
		 */
		void scrollChildren(s32 dx, s32 dy);
		
		/**
		 * Draws all child gadgets within the supplied clipping region.
		 * @param clipRect The clipping region to draw within.
		 */
		void drawChildren(Rect clipRect);

		/**
		 * Destructor.
		 */
		virtual ~ScrollingPanel() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollingPanel(const ScrollingPanel& scrollingPanel) : Gadget(scrollingPanel) { };
	};
}

#endif
