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
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 */
		virtual void handleClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus drag event.
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 * @param delta The distance dragged.
		 */
		virtual void handleDragEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta) { };

		/**
		 * Handle a drop event, triggered when the gadget has been
		 * dragged-and-dropped.
		 * @param source The gadget that was dropped.
		 * @param point The co-ordinates of the drop.
		 */
		virtual void handleDropEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus release event that occurred within the bounds of
		 * the source gadget.
		 * @param source The gadget that was released.
		 * @param point The co-ordinates of the release.
		 */
		virtual void handleReleaseEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a stylus release event that occurred outside the bounds of
		 * the source gadget.
		 * @param source The gadget that was released.
		 * @param point The co-ordinates of the release.
		 */
		virtual void handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a key press event.
		 * @param source The gadget that was focused when the key was pressed.
		 * @param keyCode The key that was pressed.
		 */
		virtual void handleKeyPressEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a key repeat event.
		 * @param source The gadget that was focused when the key was pressed.
		 * @param keyCode The key that was pressed.
		 */
		virtual void handleKeyRepeatEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a key release event.
		 * @param source The gadget that was focused when the key was released.
		 * @param keyCode The key that was released.
		 */
		virtual void handleKeyReleaseEvent(Gadget& source, const KeyCode keyCode) { };

		/**
		 * Handle a DS lid open event.
		 * @param source The gadget that raised the event.
		 */
		virtual void handleLidOpenEvent(Gadget& source) { };

		/**
		 * Handle a DS lid close event.
		 * @param source The gadget that raised the event.
		 */
		virtual void handleLidCloseEvent(Gadget& source) { };

		/**
		 * Handle a gadget focus event.
		 * @param source The gadget that received focus.
		 */
		virtual void handleFocusEvent(Gadget& source) { };

		/**
		 * Handle a gadget blur event.
		 * @param source The gadget that lost focus.
		 */
		virtual void handleBlurEvent(Gadget& source) { };

		/**
		 * Handle a gadget close event.
		 * @param source The gadget that was closed.
		 */
		virtual void handleCloseEvent(Gadget& source) { };

		/**
		 * Handle a gadget hide event.
		 * @param source The gadget that was hidden.
		 */
		virtual void handleHideEvent(Gadget& source) { };

		/**
		 * Handle a gadget show event.
		 * @param source The gadget that was shown.
		 */
		virtual void handleShowEvent(Gadget& source) { };

		/**
		 * Handle a gadget enable event.
		 * @param source The gadget that was enabled.
		 */
		virtual void handleEnableEvent(Gadget& source) { };

		/**
		 * Handle a gadget disable event.
		 * @param source The gadget that was disabled.
		 */
		virtual void handleDisableEvent(Gadget& source) { };

		/**
		 * Handle a gadget value change event.
		 * @param source The gadget that raised the event.
		 */
		virtual void handleValueChangeEvent(Gadget& source) { };

		/**
		 * Handle a gadget resize event.
		 * @param source The gadget that was resized.
		 * @param width The new gadget width.
		 * @param height The new gadget height.
		 */
		virtual void handleResizeEvent(Gadget& source, const u16 width, const u16 height) { };

		/**
		 * Handle a gadget move event.
		 * @param source The gadget that was moved.
		 * @param point The new co-ordinates of the gadget.
		 * @param delta The distance moved.
		 */
		virtual void handleMoveEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta) { };

		/**
		 * Handle a gadget scroll event.
		 * @param source The gadget that was scrolled.
		 * @param delta The distance scrolled.
		 */
		virtual void handleScrollEvent(Gadget& source, const WoopsiPoint& delta) { };

		/**
		 * Handle a gadget shift click event.
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 */
		virtual void handleShiftClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a gadget context menu selection event.
		 * @param source The gadget that opened the menu.
		 * @param item The selected context menu item.
		 */
		virtual void handleContextMenuSelectionEvent(Gadget& source, const ListDataItem* item) { };

		/**
		 * Handle a gadget double-click event.
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 */
		virtual void handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) { };

		/**
		 * Handle a gadget shelve event.
		 * @param source The gadget that was shelved.
		 */
		virtual void handleShelveEvent(Gadget& source) { };

		/**
		 * Handle a gadget unshelve event.
		 * @param source The gadget that was unshelved.
		 */
		virtual void handleUnshelveEvent(Gadget& source) { };

		/**
		 * Handle a gadget action event.
		 * @param source The gadget that performed the action.
		 */
		virtual void handleActionEvent(Gadget& source) { };

		/**
		 * Handle a gadget move forward event.
		 * @param source The gadget that was moved forwards in its parent's
		 * child list.
		 */
		virtual void handleMoveForwardEvent(Gadget& source) { };

		/**
		 * Handle a gadget move backward event.
		 * @param source The gadget that was moved backwards in its parent's
		 * child list.
		 */
		virtual void handleMoveBackwardEvent(Gadget& source) { };
	};
}

#endif
