#ifndef _GADGETEVENTHANDLER_H_
#define _GADGETEVENTHANDLER_H_

#include <nds.h>
#include "gadgeteventargs.h"
#include "contextmenueventargs.h"

namespace WoopsiUI {

	class Gadget;

	/**
	 * Base GadgetEventHandler class, intended to be subclassed.  Any class that needs to listen for
	 * gadget events should inherit from this class.
	 */
	class GadgetEventHandler {
	public:
		/**
		 * Constructor.
		 */
		inline GadgetEventHandler() { }
		
		/**
		 * Destructor.
		 */
		virtual inline ~GadgetEventHandler() { }
		
		/**
		 * Handle a stylus click event.
		 * @param e The event data.
		 */
		virtual void handleClickEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a stylus drag event.
		 * @param e The event data.
		 */
		virtual void handleDragEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a stylus release event that occurred within the bounds of
		 * the source gadget.
		 * @param e The event data.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a stylus release event that occurred outside the bounds of
		 * the source gadget.
		 * @param e The event data.
		 */
		virtual void handleReleaseOutsideEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a key press event.
		 * @param e The event data.
		 */
		virtual void handleKeyPressEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a key release event.
		 * @param e The event data.
		 */
		virtual void handleKeyReleaseEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a DS lid open event.
		 * @param e The event data.
		 */
		virtual void handleLidOpenEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a DS lid close event.
		 * @param e The event data.
		 */
		virtual void handleLidCloseEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget focus event.
		 * @param e The event data.
		 */
		virtual void handleFocusEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget blur event.
		 * @param e The event data.
		 */
		virtual void handleBlurEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget close event.
		 * @param e The event data.
		 */
		virtual void handleCloseEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget hide event.
		 * @param e The event data.
		 */
		virtual void handleHideEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget show event.
		 * @param e The event data.
		 */
		virtual void handleShowEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget enable event.
		 * @param e The event data.
		 */
		virtual void handleEnableEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget disable event.
		 * @param e The event data.
		 */
		virtual void handleDisableEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget value change event.
		 * @param e The event data.
		 */
		virtual void handleValueChangeEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget resize event.
		 * @param e The event data.
		 */
		virtual void handleResizeEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget move event.
		 * @param e The event data.
		 */
		virtual void handleMoveEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget scroll event.
		 * @param e The event data.
		 */
		virtual void handleScrollEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget shift click event.
		 * @param e The event data.
		 */
		virtual void handleShiftClickEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget context menu selection event.
		 * @param e The event data.
		 */
		virtual void handleContextMenuSelectionEvent(const ContextMenuEventArgs& e) { };

		/**
		 * Handle a gadget double-click event.
		 * @param e The event data.
		 */
		virtual void handleDoubleClickEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget shelve event.
		 * @param e The event data.
		 */
		virtual void handleShelveEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget unshelve event.
		 * @param e The event data.
		 */
		virtual void handleUnshelveEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget action event.
		 * @param e The event data.
		 */
		virtual void handleActionEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget move forward event.
		 * @param e The event data.
		 */
		virtual void handleMoveForwardEvent(const GadgetEventArgs& e) { };

		/**
		 * Handle a gadget move backward event.
		 * @param e The event data.
		 */
		virtual void handleMoveBackwardEvent(const GadgetEventArgs& e) { };
	};
}

#endif
