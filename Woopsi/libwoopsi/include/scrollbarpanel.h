#ifndef _SCROLLBAR_PANEL_H_
#define _SCROLLBAR_PANEL_H_

#include "gadget.h"
#include "scrollingpanel.h"
#include "gadgetstyle.h"
#include "scrollbarvertical.h"
#include "scrollbarhorizontal.h"
#include "gadgeteventhandler.h"
#include "scrollablebase.h"
#include "graphicsport.h"

namespace WoopsiUI {

	/**
	 * Class containing a scrolling panel bordered by scrollbars.
	 */
	class ScrollbarPanel : public Gadget, public ScrollableBase, public GadgetEventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param flags The usual gadget flags.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ScrollbarPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style = NULL);

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
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsVerticalScroll(bool allow);

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsHorizontalScroll(bool allow);

		/**
		 * Sets the width of the virtual canvas.
		 * @param width The width of the virtual canvas.
		 */
		virtual void setCanvasWidth(const s32 width);
		
		/**
		 * Sets the height of the virtual canvas.
		 * @param height The height of the virtual canvas.
		 */
		virtual void setCanvasHeight(const s32 height);

		/**
		 * Returns true if vertical scrolling is allowed.
		 * @return True if vertical scrolling is allowed.
		 */
		virtual bool allowsVerticalScroll() const;

		/**
		 * Returns true if horizontal scrolling is allowed.
		 * @return True if horizontal scrolling is allowed.
		 */
		virtual bool allowsHorizontalScroll() const;

		/**
		 * Gets the x co-ordinate of the virtual canvas.
		 * @return The x co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasX() const;
		
		/**
		 * Gets the y co-ordinate of the virtual canvas.
		 * @return The y co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasY() const;

		/**
		 * Gets the width of the virtual canvas.
		 * @return The width of the virtual canvas.
		 */
		virtual const s32 getCanvasWidth() const;
		
		/**
		 * Gets the height of the virtual canvas.
		 * @return The height of the virtual canvas.
		 */
		virtual const s32 getCanvasHeight() const;

		void handleScrollEvent(const GadgetEventArgs& e);
		void handleValueChangeEvent(const GadgetEventArgs& e);

		inline ScrollingPanel* getPanel() { return _panel; };

	protected:
		ScrollingPanel* _panel;
		ScrollbarHorizontal* _scrollbarHorizontal;
		ScrollbarVertical* _scrollbarVertical;
		u8 _scrollbarWidth;									/**< Width of the vertical scrollbar. */
		u8 _scrollbarHeight;								/**< Height of the horizontal scrollbar. */
		bool _hasVerticalScrollbar;
		bool _hasHorizontalScrollbar;

		void buildUI();

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		virtual ~ScrollbarPanel() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollbarPanel(const ScrollbarPanel& scrollbarPanel) : Gadget(scrollbarPanel) { };
	};
}

#endif
