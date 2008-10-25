#ifndef _WOOPSI_TIMER_H_
#define _WOOPSI_TIMER_H_

#include <nds.h>
#include "gadget.h"
#include "woopsi.h"

namespace WoopsiUI {

	/**
	 * Class providing a timer.
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

	protected:
		u32 _frameCount;
		u32 _timeout;
		bool _isRunning;
		bool _isRepeater;

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
