#ifndef _LISTDATA_EVENTHANDLER_H_
#define _LISTDATA_EVENTHANDLER_H_

#include <nds.h>
#include "listdataeventargs.h"

namespace WoopsiUI {

	class ListData;

	/**
	 * Base ListDataEventHandler class, intended to be subclassed.  Any class that needs to listen for
	 * ListData events should inherit from this class.
	 */
	class ListDataEventHandler {
	public:
		/**
		 * Constructor.
		 */
		inline ListDataEventHandler() { }
		
		/**
		 * Destructor.
		 */
		virtual inline ~ListDataEventHandler() { }
		
		/**
		 * Handle data changes.
		 * @param e Event arguments.
		 */
		virtual void handleListDataChangedEvent(const ListDataEventArgs& e) = 0;

		/**
		 * Handle data changes.
		 * @param e Event arguments.
		 */
		virtual void handleListDataSelectionChangedEvent(const ListDataEventArgs& e) = 0;
	};
}

#endif
