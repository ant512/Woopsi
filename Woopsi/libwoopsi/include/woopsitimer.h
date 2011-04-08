#ifndef _WOOPSI_TIMER_H_
#define _WOOPSI_TIMER_H_

#include <nds.h>
#include "gadget.h"
#include "woopsi.h"

namespace WoopsiUI {

	/**
	 * Timer gadget.  It can drive time-based events, animations, etc.  All actions that
	 * are time or vertical blank based should be driven by a WoopsiTimer.  Attempting to
	 * tie into libnds' low-level vertical blank interrupt can potentially cause all
	 * sorts of havoc.
	 *
	 * Using the timer is simple:
	 *  - Create an instance of the WoopsiTimer and add it as a child to a gadget. 
	 *  - Call the instance's "start()" method.
	 *  - Catch the timer's action event and call any code that should run.
	 *
	 * The maximum speed for timer-driven code is one iteration per vertical blank.  
	 */
	class WoopsiTimer : public Gadget {
	public:

		/**
		 * Constructor.
		 * @param timeout Time, in frames, before the timer fires an EVENT_ACTION event.
		 * @param repeat If true, the timer will fire multiple events.  If false, the
		 * timer will fire just once and stop.
		 */
		WoopsiTimer(u32 timeout, bool repeat);

		/**
		 * Return the timeout of this timer.
		 * @return The number of frames that this timer runs before firing an event.
		 */
		inline const u32 getTimeout() const { return _timeout; };

		/**
		 * Return the number of frames that this timer has been running for since the
		 * last event fired.
		 * @return Frame count since the last event fired.
		 */
		inline const u32 getFrameCount() const { return _frameCount; };

		/**
		 * Resets the frame count back to 0.
		 */
		inline void reset() { _frameCount = 0; };

		/**
		 * Starts the timer.
		 */
		inline void start() { _isRunning = true; };

		/**
		 * Stops the timer and resets the frame count.
		 */
		inline void stop() {
			_isRunning = false;
			_frameCount = 0;
		};

		/**
		 * Stops the timer but does not reset the frame count.
		 */
		inline void pause() { _isRunning = false; }

		/**
		 * Set the timeout of this timer.
		 * @param timeout The number of frames that this timer will run before firing an event.
		 */
		inline void setTimeout(u32 timeout) { _timeout = timeout; };

		/**
		 * Run any code that should execute every VBL.
		 * @return True if the function ran succesfully.
		 */
		virtual bool run();

		/**
		 * Check if the timer is running or not.
		 * @return True if the timer is running; false if not.
		 */
		inline bool isRunning() { return _isRunning; };

	protected:
		u32 _frameCount;			/**< Number of frames run so far */
		u32 _timeout;				/**< Number of frames to run before firing an event */
		bool _isRunning;			/**< Indicates whether or not the timer is running */
		bool _isRepeater;			/**< Indicates whether or not the timer repeats */

		/**
		 * Destructor.
		 */
		virtual inline ~WoopsiTimer() {
			if (woopsiApplication != NULL) {

				// Prevent gadget from receiving VBLs
				woopsiApplication->unregisterFromVBL(this);
			}
		};

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WoopsiTimer(const WoopsiTimer& woopsiTimer) : Gadget(woopsiTimer) { };
	};
}

#endif
