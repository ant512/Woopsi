#include "gadgeteventhandlerlist.h"
#include "gadget.h"
#include "gadgeteventargs.h"

using namespace WoopsiUI;

GadgetEventHandlerList::GadgetEventHandlerList(Gadget* gadget) {
	_gadget = gadget;
	_isEnabled = true;
}

const bool GadgetEventHandlerList::isEnabled() const {
	return _isEnabled && !_gadget->isDeleted();
}

void GadgetEventHandlerList::raiseClickEvent(s16 x, s16 y) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleClickEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseDoubleClickEvent(s16 x, s16 y) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDoubleClickEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseShiftClickEvent(s16 x, s16 y) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShiftClickEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseReleaseEvent(s16 x, s16 y) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleReleaseEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseReleaseOutsideEvent(s16 x, s16 y) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleReleaseOutsideEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, vX, vY, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDragEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseMoveForwardEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveForwardEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseMoveBackwardEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveBackwardEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseKeyPressEvent(KeyCode keyCode) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleKeyPressEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseKeyRepeatEvent(KeyCode keyCode) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleKeyRepeatEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseKeyReleaseEvent(KeyCode keyCode) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleKeyReleaseEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseLidCloseEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleLidCloseEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseLidOpenEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleLidOpenEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseFocusEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleFocusEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseBlurEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleBlurEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseCloseEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleCloseEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseHideEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleHideEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseShowEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShowEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseShelveEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShelveEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseUnshelveEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleUnshelveEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseEnableEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleEnableEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseDisableEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDisableEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseValueChangeEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleValueChangeEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseResizeEvent(u16 width, u16 height) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleResizeEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseMoveEvent(s16 x, s16 y, s16 vX, s16 vY) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, x, y, vX, vY, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseActionEvent() {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleActionEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseContextMenuSelectionEvent(const ListDataItem* contextMenuItem) {
	if (isEnabled()) {
		ContextMenuEventArgs e(_gadget, contextMenuItem);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleContextMenuSelectionEvent(e);
		}
	}
}

void GadgetEventHandlerList::raiseScrollEvent(s16 vX, s16 vY) {
	if (isEnabled()) {
		GadgetEventArgs e(_gadget, 0, 0, vX, vY, KEY_CODE_NONE);
	
		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleScrollEvent(e);
		}
	}
}

void GadgetEventHandlerList::addGadgetEventHandler(GadgetEventHandler* eventHandler) {

	// Prevent insertion if the handler already exists
	for (s32 i = 0; i < _gadgetEventHandlers.size(); ++i) {
		if (_gadgetEventHandlers.at(i) == eventHandler) {
			return;
		}
	}

	// Add the new handler
	_gadgetEventHandlers.push_back(eventHandler);
}

void GadgetEventHandlerList::removeGadgetEventHandler(GadgetEventHandler* eventHandler) {
	for (s32 i = 0; i < _gadgetEventHandlers.size(); ++i) {
		if (_gadgetEventHandlers.at(i) == eventHandler) {
			_gadgetEventHandlers.erase(i);
			return;
		}
	}
}