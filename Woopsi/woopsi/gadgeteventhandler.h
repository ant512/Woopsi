#ifndef _GADGETEVENTHANDLER_H_
#define _GADGETEVENTHANDLER_H_

#include <nds.h>
#include "GadgetEventArgs.h"

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
		 * Event handler function.  Each gadget that this class is the event handler for will
		 * call this function and pass it details of the event in the EventArgs parameter.
		 * @param e The event's argument data.
		 */
		virtual bool handleEvent(const GadgetEventArgs& e) { return true; };

		virtual void handleClickEvent(const GadgetEventArgs& e) { };

		virtual void handleDragEvent(const GadgetEventArgs& e) { };

		virtual void handleReleaseEvent(const GadgetEventArgs& e) { };

		virtual void handleReleaseOutsideEvent(const GadgetEventArgs& e) { };

		virtual void handleKeyPressEvent(const GadgetEventArgs& e) { };

		virtual void handleKeyReleaseEvent(const GadgetEventArgs& e) { };

		virtual void handleLidOpenEvent(const GadgetEventArgs& e) { };

		virtual void handleLidCloseEvent(const GadgetEventArgs& e) { };

		virtual void handleFocusEvent(const GadgetEventArgs& e) { };

		virtual void handleBlurEvent(const GadgetEventArgs& e) { };

		virtual void handleCloseEvent(const GadgetEventArgs& e) { };

		virtual void handleHideEvent(const GadgetEventArgs& e) { };

		virtual void handleShowEvent(const GadgetEventArgs& e) { };

		virtual void handleEnableEvent(const GadgetEventArgs& e) { };

		virtual void handleDisableEvent(const GadgetEventArgs& e) { };

		virtual void handleValueChangeEvent(const GadgetEventArgs& e) { };

		virtual void handleResizeEvent(const GadgetEventArgs& e) { };

		virtual void handleMoveEvent(const GadgetEventArgs& e) { };

		virtual void handleScrollEvent(const GadgetEventArgs& e) { };

		virtual void handleShiftClickEvent(const GadgetEventArgs& e) { };

		virtual void handleContextMenuSelectionEvent(const GadgetEventArgs& e) { };

		virtual void handleDoubleClickEvent(const GadgetEventArgs& e) { };

		virtual void handleShelveEvent(const GadgetEventArgs& e) { };

		virtual void handleUnshelveEvent(const GadgetEventArgs& e) { };

		virtual void handleActionEvent(const GadgetEventArgs& e) { };

		virtual void handleMoveForwardEvent(const GadgetEventArgs& e) { };

		virtual void handleMoveBackwardEvent(const GadgetEventArgs& e) { };
	};
}

#endif
