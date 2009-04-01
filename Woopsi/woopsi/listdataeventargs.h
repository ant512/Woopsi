#include "eventargs.h"

#ifndef _LISTDATAEVENTARGS_H_
#define _LISTDATAEVENTARGS_H_

namespace WoopsiUI {

	class ListData;

	/**
	 * Event arguments passed to listeners when a ListData object raises an event.
	 */
	class ListDataEventArgs : public EventArgs<ListData*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the ListData object that raised the event.
		 */
		inline ListDataEventArgs(ListData* source) : EventArgs<ListData*>(source) { }
	};
}

#endif
