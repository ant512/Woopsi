#include "eventdata.h"

#ifndef _LISTDATA_H_
#define _LISTDATA_H_

namespace WoopsiUI {

	class ListData;

	/**
	 * Event arguments passed to listeners when a ListData object raises an event.
	 */
	class ListDataEventArgs : public EventData<const ListData*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the ListData object that raised the event.
		 */
		inline ListDataEventArgs(const ListData* source) : EventData<const ListData*>(source) { }
	};
}

#endif
