#ifndef _LISTDATA_EVENTHANDLER_H_
#define _LISTDATA_EVENTHANDLER_H_

#include <nds.h>

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
		 */
		virtual bool handleDataChangedEvent(const ListData* listDataSource) = 0;

		/**
		 * Handle data changes.
		 */
		virtual bool handleSelectionChangedEvent(const ListData* listDataSource) = 0;
	};
}

#endif
