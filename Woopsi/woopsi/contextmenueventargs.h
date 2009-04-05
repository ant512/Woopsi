#ifndef _CONTEXTMENUEVENTARGS_H_
#define _CONTEXTMENUEVENTARGS_H_

#include "eventargs.h"

namespace WoopsiUI {

	class ContextMenu;
	class ContextMenuItem;
	class ContextMenuEventArgs;

	/**
	 * Event arguments passed to listeners when a WoopsiKeyboard object raises an event.
	 */
	class ContextMenuEventArgs : public EventArgs<const ContextMenu*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the ContextMenu object that raised the event.
		 */
		ContextMenuEventArgs(const ContextMenu* source, const ContextMenuItem* item) : EventArgs<const ContextMenu*>(source) {
			_item = item;
		};

		/**
		 * Get the item that caused the event.
		 * @return Pointer to the item that caused the event.
		 */
		inline const ContextMenuItem* getItem() const { return _item; };

	private:
		const ContextMenuItem* _item;
	};
}

#endif
