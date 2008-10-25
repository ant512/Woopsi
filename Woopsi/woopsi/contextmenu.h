#ifndef _CONTEXT_MENU_H_
#define _CONTEXT_MENU_H_

#include "gadget.h"
#include "eventhandler.h"

namespace WoopsiUI {

	class ContextMenuItem;

	/**
	 * The context menu is implemented as a member of the Woopsi class.
	 * It should not be instantiated elsewhere.  It offers a simple
	 * context-aware menu that opens when the user holds down a modifier
	 * key when tapping a gadget.
	 */
	class ContextMenu : public Gadget, public EventHandler {
	public:

		/**
		 * Constructor.
		 * @param font The font to use with the context menu.
		 */
		ContextMenu(FontBase* font = NULL);

		/**
		 * Handles events raised by the gadget's sub-gadgets.
		 * @param e The event arguments.
		 */
		virtual bool handleEvent(const EventArgs& e);

		/**
		 * Add a new menu item to the gadget.
		 * @param text Text to show in the item.
		 * @param value The value of the item.
		 */
		virtual ContextMenuItem* newMenuItem(const char* text, u32 value);

		/**
		 * Draw the region of the menu within the clipping rect.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Draw all visible regions of the menu.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

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
		 * Get the value of the context menu.  This will be
		 * the value of the last chosen item.
		 * @return The value of the last chosen menu item.
		 */
		inline const u32 getValue() const { return _value; };

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
		Gadget* _opener;				/**< Pointer to the gadget that opened the menu */
		u32 _value;						/**< The value of the last selected menu item */

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
