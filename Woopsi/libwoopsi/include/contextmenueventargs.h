#ifndef _CONTEXT_MENU_EVENT_ARGS_H_
#define _CONTEXT_MENU_EVENT_ARGS_H_

#include "eventargs.h"

namespace WoopsiUI {

	class Gadget;
	class ListDataItem;
	class ContextMenuEventArgs;

	/**
	 * Event arguments passed to listeners when a WoopsiKeyboard object raises an event.
	 */
	class ContextMenuEventArgs : public EventArgs<const Gadget*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the ContextMenu object that raised the event.
		 * @param item Pointer to the ContextMenuItem that caused the event.
		 */
		ContextMenuEventArgs(const Gadget* source, const ListDataItem* item) : EventArgs<const Gadget*>(source) {
			_item = item;
		};

		/**
		 * Get the item that caused the event.
		 * @return Pointer to the item that caused the event.
		 */
		inline const ListDataItem* getItem() const { return _item; };

	private:
		const ListDataItem* _item;			/**< Pointer to the item that caused the event */
	};
}

#endif
