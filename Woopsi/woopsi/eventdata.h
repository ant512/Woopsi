namespace WoopsiUI {

	/**
	 * Base template class for all events within the Woopsi system.
	 * All event types should inherit from this class.
	 */
	template <class T>
	class EventData {

	public:
		
		/**
		 * Constructor.
		 * @param source The object that raised the event.
		 */
		EventData(const T& source);

		/**
		 * Destructor.
		 */
		virtual inline ~EventData() { }
		
		/**
		 * Get the source object that raised the event.
		 */
		inline T& getSource();

	private:
		T _source;					/**< The object that raised the event */
	};

	template <class T>
	EventData<T>::EventData(const T& source) {
		_source = source;
	}

	template <class T>
	T& EventData<T>::getSource() {
		return _source;
	}
}
