#ifndef _CONTEXT_MENU_H_
#define _CONTEXT_MENU_H_

#include "gadget.h"
#include "gadgetstyle.h"
#include "gadgeteventhandler.h"
#include "listbox.h"

namespace WoopsiUI {

	class ContextMenuItem;

	/**
	 * The context menu is implemented as a member of the Woopsi class.  It
	 * should not be instantiated elsewhere.  It offers a simple context-aware
	 * menu that opens when the user holds down a modifier key when tapping a
	 * gadget.
	 */
	class ContextMenu : public Gadget, public GadgetEventHandler {
	public:

		/**
		 * Constructor.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ContextMenu(GadgetStyle* style = NULL);

		/**
		 * Add a new menu item to the gadget.
		 * @param text Text to show in the item.
		 * @param value The value of the item.
		 */
		virtual void addOption(const WoopsiString& text, u32 value);

		/**
		 * Clear all items from the menu and reset it.
		 */
		virtual void reset();

		/**
		 * Set the gadget that opened the menu.
		 * @param gadget Pointer to the gadget that opened the menu.
		 */
		inline void setOpener(Gadget* gadget) { _opener = gadget; };

		/**
		 * Get the gadget that opened the menu.
		 * @return Pointer to the gadget that opened the menu.
		 */
		inline Gadget* getOpener() { return _opener; };

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.  Value is based on the length of the largest string
		 * in the set of options.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

		/**
		 * Handles events raised by the gadget's sub-gadgets.
		 * @param e The event arguments.
		 */
		virtual void handleReleaseEvent(Gadget& source, const WoopsiPoint& point);

		/**
		 * Handles events raised by the gadget's sub-gadgets.
		 * @param e The event arguments.
		 */
		virtual void handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point);

	private:
		Gadget* _opener;				/**< Pointer to the gadget that opened the menu */
		ListBox* _listbox;				/**< Pointer to the list box */

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
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Destructor.
		 */
		virtual inline ~ContextMenu() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ContextMenu(const ContextMenu& contextMenu) : Gadget(contextMenu) { };
	};
}

#endif
