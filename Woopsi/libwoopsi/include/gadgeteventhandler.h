#ifndef _GADGETEVENTHANDLER_H_
#define _GADGETEVENTHANDLER_H_

#include <nds.h>

namespace WoopsiUI {

	class Gadget;
	class WoopsiPoint;
	class ListDataItem;

	/**
	 * Enum listing all key codes.
	 */
	enum KeyCode {
		KEY_CODE_NONE = 0,			/**< No key.  Included for completeness. */
		KEY_CODE_UP = 1,			/**< D-pad up button. */
		KEY_CODE_DOWN = 2,			/**< D-pad down button. */
		KEY_CODE_LEFT = 3,			/**< D-pad left button. */
		KEY_CODE_RIGHT = 4,			/**< D-pad right button. */
		KEY_CODE_A = 5,				/**< A button. */
		KEY_CODE_B = 6,				/**< B button. */
		KEY_CODE_X = 7,				/**< X button. */
		KEY_CODE_Y = 8,				/**< Y button. */
		KEY_CODE_L = 9,				/**< L button. */
		KEY_CODE_R = 10,			/**< R button. */
		KEY_CODE_START = 11,		/**< Start button. */
		KEY_CODE_SELECT = 12		/**< Select button. */
	};

	/**
	 * Any class that needs to listen for gadget events should inherit from this
	 * class.
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
		virtual void handleClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus drag event.
		 * @param e The event data.
		 */
		virtual void handleDragEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta) { };

		/**
		 * Handle a drop event, triggered when the gadget has been dragged-and-dropped.
		 * @param e The event data.
		 */
		virtual void handleDropEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus release event that occurred within the bounds of
		 * the source gadget.
		 * @param e The event data.
		 */
		virtual void handleReleaseEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus release event that occurred outside the bounds of
		 * the source gadget.
		 * @param e The event data.
		 */
		virtual void handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a key press event.
		 * @param e The event data.
		 */
		virtual void handleKeyPressEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a key repeat event.
		 * @param e The event data.
		 */
		virtual void handleKeyRepeatEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a key release event.
		 * @param e The event data.
		 */
		virtual void handleKeyReleaseEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a DS lid open event.
		 * @param e The event data.
		 */
		virtual void handleLidOpenEvent(Gadget& source) { };

		/**
		 * Handle a DS lid close event.
		 * @param e The event data.
		 */
		virtual void handleLidCloseEvent(Gadget& source) { };

		/**
		 * Handle a gadget focus event.
		 * @param e The event data.
		 */
		virtual void handleFocusEvent(Gadget& source) { };

		/**
		 * Handle a gadget blur event.
		 * @param e The event data.
		 */
		virtual void handleBlurEvent(Gadget& source) { };

		/**
		 * Handle a gadget close event.
		 * @param e The event data.
		 */
		virtual void handleCloseEvent(Gadget& source) { };

		/**
		 * Handle a gadget hide event.
		 * @param e The event data.
		 */
		virtual void handleHideEvent(Gadget& source) { };

		/**
		 * Handle a gadget show event.
		 * @param e The event data.
		 */
		virtual void handleShowEvent(Gadget& source) { };

		/**
		 * Handle a gadget enable event.
		 * @param e The event data.
		 */
		virtual void handleEnableEvent(Gadget& source) { };

		/**
		 * Handle a gadget disable event.
		 * @param e The event data.
		 */
		virtual void handleDisableEvent(Gadget& source) { };

		/**
		 * Handle a gadget value change event.
		 * @param e The event data.
		 */
		virtual void handleValueChangeEvent(Gadget& source) { };

		/**
		 * Handle a gadget resize event.
		 * @param e The event data.
		 */
		virtual void handleResizeEvent(Gadget& source, const u16 width, const u16 height) { };

		/**
		 * Handle a gadget move event.
		 * @param e The event data.
		 */
		virtual void handleMoveEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta) { };

		/**
		 * Handle a gadget scroll event.
		 * @param e The event data.
		 */
		virtual void handleScrollEvent(Gadget& source, const WoopsiPoint& delta) { };

		/**
		 * Handle a gadget shift click event.
		 * @param e The event data.
		 */
		virtual void handleShiftClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a gadget context menu selection event.
		 * @param e The event data.
		 */
		virtual void handleContextMenuSelectionEvent(Gadget& source, const ListDataItem* item) { };

		/**
		 * Handle a gadget double-click event.
		 * @param e The event data.
		 */
		virtual void handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a gadget shelve event.
		 * @param e The event data.
		 */
		virtual void handleShelveEvent(Gadget& source) { };

		/**
		 * Handle a gadget unshelve event.
		 * @param e The event data.
		 */
		virtual void handleUnshelveEvent(Gadget& source) { };

		/**
		 * Handle a gadget action event.
		 * @param e The event data.
		 */
		virtual void handleActionEvent(Gadget& source) { };

		/**
		 * Handle a gadget move forward event.
		 * @param e The event data.
		 */
		virtual void handleMoveForwardEvent(Gadget& source) { };

		/**
		 * Handle a gadget move backward event.
		 * @param e The event data.
		 */
		virtual void handleMoveBackwardEvent(Gadget& source) { };
	};
}

#endif
