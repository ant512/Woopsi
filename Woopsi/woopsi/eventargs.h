#ifndef _EVENTARGS_H_
#define _EVENTARGS_H_

namespace WoopsiUI {

	/**
	 * Base template class for all events within the Woopsi system.
	 * All event types should inherit from this class.
	 */
	template <class T>
	class EventArgs {

	public:
		
		/**
		 * Constructor.
		 * @param source The object that raised the event.
		 */
		inline EventArgs(const T& source) { _source = source; };

		/**
		 * Destructor.
		 */
		virtual inline ~EventArgs() { };
		
		/**
		 * Get the source object that raised the event.
		 */
		inline const T& getSource() const { return _source; };

	private:
		T _source;					/**< The object that raised the event */
	};
}

#endif
