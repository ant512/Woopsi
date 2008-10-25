#ifndef _CONTEXT_MENU_ITEM_H_
#define _CONTEXT_MENU_ITEM_H_

#include "label.h"

namespace WoopsiUI {

	/**
	 * Class representing an item in the ContextMenu class.  Woopsi
	 * handles the creation of these items automatically based on a
	 * gadget's context menu item array, so these objects should
	 * not be created by any other code.
	 */
	class ContextMenuItem : public Label {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the menu item, relative to its parent.
		 * @param y The y co-ordinate of the menu item, relative to its parent.
		 * @param width The width of the menu item.
		 * @param height The height of the menu item.
		 * @param text Pointer to a string to display in the menu item.
		 * @param value The value of the item.
		 * @param font The font to use in this menu item.
		 */
		ContextMenuItem(s16 x, s16 y, u16 width, u16 height, const char* text, u32 value, FontBase* font = NULL);

		/**
		 * Draw the region of the menu item within the clipping rect.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Draw all visible regions of the menu.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Get the menu item's value.
		 * @return The menu item's value.
		 */
		inline u32 getValue() { return _value; };

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool click(s16 x, s16 y);

		/**
		 * Release this gadget at the supplied co-ordinates
		 * @param x X co-ordinate of the release.
		 * @param y Y co-ordinate of the release.
		 * @return True if the release was successful.
		 */
		bool release(s16 x, s16 y);

		/**
		 * Give the gadget focus.
		 * @return True if the gadget received focus correctly.
		 */
		virtual inline bool focus() { return false; };

		/**
		 * Remove focus from the gadget.
		 * @return True if the gadget lost focus correctly.
		 */
		virtual inline bool blur() { return false; };

	private:
		u32 _value;								/**< The value of the menu item */

		/**
		 * Destructor.
		 */
		virtual inline ~ContextMenuItem() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ContextMenuItem(const ContextMenuItem& contextMenuItem) : Label(contextMenuItem) { };
	};
}

#endif
