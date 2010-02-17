#ifndef _GADGET_EVENT_HANDLER_LIST_H_
#define _GADGET_EVENT_HANDLER_LIST_H_

#include <nds.h>
#include "gadgeteventhandler.h"
#include "woopsiarray.h"

namespace WoopsiUI {

	class Gadget;

	/**
	 * List of gadget event handlers.
	 */
	class GadgetEventHandlerList {
	public:

		/**
		 * Constructor.
		 * @param gadget The owning gadget.
		 */
		GadgetEventHandlerList(Gadget* gadget);

		/**
		 * Destructor.
		 */
		~GadgetEventHandlerList() { };
		
		/**
		 * Check if the object raises events or not.
		 * @return True if events are enabled.
		 */
		const bool isEnabled() const;

		/**
		 * Get the event handler at the specified index.
		 * @param index The index of the event handler.
		 * @return The event handler at the specified index.
		 */
		inline GadgetEventHandler* at(const s32 index) const { return _gadgetEventHandlers.at(index); };

		/**
		 * Get the size of the array.
		 * @return The size of the array.
		 */
		inline const s32 size() const { return _gadgetEventHandlers.size(); };

		/**
		 * Adds a gadget event handler.  The event handler will receive
		 * all events raised by this object.
		 * @param eventHandler A pointer to the event handler.
		 */
		void addGadgetEventHandler(GadgetEventHandler* eventHandler);

		/**
		 * Remove a gadget event handler.
		 * @param eventHandler A pointer to the event handler to remove.
		 */
		void removeGadgetEventHandler(GadgetEventHandler* eventHandler);

		/**
		 * Enables event raising.
		 */
		inline void enable() { _isEnabled = true; };

		/**
		 * Disables event raising.
		 */
		inline void disable() { _isEnabled = false; };

		/**
		 * Raise a click event to the event handler.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		void raiseClickEvent(s16 x, s16 y);

		/**
		 * Raise a double-click event to the event handler.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		void raiseDoubleClickEvent(s16 x, s16 y);

		/**
		 * Raise a shift click event to the event handler.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		void raiseShiftClickEvent(s16 x, s16 y);

		/**
		 * Raise a stylus release event to the event handler.
		 * @param x The x co-ordinate of the release.
		 * @param y The y co-ordinate of the release.
		 */
		void raiseReleaseEvent(s16 x, s16 y);

		/**
		 * Raise a stylus release-outside event to the event handler.
		 * @param x The x co-ordinate of the release.
		 * @param y The y co-ordinate of the release.
		 */
		void raiseReleaseOutsideEvent(s16 x, s16 y);

		/**
		 * Raise a stylus drag event to the event handler.
		 * @param x The x co-ordinate of the stylus when the drag started.
		 * @param y The y co-ordinate of the stylus when the drag started.
		 * @param vX The horizontal distance dragged.
		 * @param vY The vertical distance dragged.
		 */
		void raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY);

		/**
		 * Raise a move forward event to the event handler.
		 */
		void raiseMoveForwardEvent();

		/**
		 * Raise a move backward event to the event handler.
		 */
		void raiseMoveBackwardEvent();

		/**
		 * Raise a key press event to the event handler.
		 * @param keyCode The code of the key that caused the event.
		 */
		void raiseKeyPressEvent(KeyCode keyCode);
		
		/**
		 * Raise a key repeat event to the event handler.
		 * @param keyCode The code of the key that caused the event.
		 */
		void raiseKeyRepeatEvent(KeyCode keyCode);

		/**
		 * Raise a key release event to the event handler.
		 * @param keyCode The code of the key that caused the event.
		 */
		void raiseKeyReleaseEvent(KeyCode keyCode);

		/**
		 * Raise a lid closed event to the event handler.
		 */
		void raiseLidCloseEvent();

		/**
		 * Raise a lid opened event to the event handler.
		 */
		void raiseLidOpenEvent();

		/**
		 * Raise a focus event to the event handler.
		 */
		void raiseFocusEvent();

		/**
		 * Raise a blur event to the event handler.
		 */
		void raiseBlurEvent();

		/**
		 * Raise a close event to the event handler.
		 */
		void raiseCloseEvent();

		/**
		 * Raise a hide event to the event handler.
		 */
		void raiseHideEvent();

		/**
		 * Raise a show event to the event handler.
		 */
		void raiseShowEvent();

		/**
		 * Raise a shelve event to the event handler.
		 */
		void raiseShelveEvent();

		/**
		 * Raise a unshelve event to the event handler.
		 */
		void raiseUnshelveEvent();

		/**
		 * Raise an enable event to the event handler.
		 */
		void raiseEnableEvent();

		/**
		 * Raise a disable event to the event handler.
		 */
		void raiseDisableEvent();

		/**
		 * Raise a value change event to the event handler.
		 */
		void raiseValueChangeEvent();

		/**
		 * Raise a resize event to the event handler.
		 * @param width The new width of the gadget.
		 * @param height The new height of the gadget.
		 */
		void raiseResizeEvent(u16 width, u16 height);

		/**
		 * Raise a move event to the event handler.
		 * @param x The new x co-ordinate of the gadget.
		 * @param y The new y co-ordinate of the gadget.
		 * @param vX The horizontal distance moved.
		 * @param vY The vertical distance moved.
		 */
		void raiseMoveEvent(s16 x, s16 y, s16 vX, s16 vY);

		/**
		 * Raise a context menu selection event to the event handler.
		 * @param contextMenuItem Pointer to the menu item selected.
		 */
		void raiseContextMenuSelectionEvent(const ListDataItem* contextMenuItem);

		/**
		 * Raise an action event to the event handler.  This should be called
		 * when a gadget's purpose has been fulfilled.  For example, in the case
		 * of a button, this event is raised when the button is released within
		 * its boundaries.  The button has produced a valid click, and thus
		 * fulfilled its purpose, so it needs to raise an "action" event.
		 */
		void raiseActionEvent();

		/**
		 * Raises a scroll event.  Fired when the panel scrolls.
		 * @param vX Horizontal distance scrolled.
		 * @param vY Vertical distance scrolled.
		 */
		void raiseScrollEvent(s16 vX, s16 vY);

	protected:
		WoopsiArray<GadgetEventHandler*> _gadgetEventHandlers;		/**< List of event handlers */
		Gadget* _gadget;											/**< Owning gadget */
		bool _isEnabled;											/**< Indicates if events are active */
	};
}

#endif
