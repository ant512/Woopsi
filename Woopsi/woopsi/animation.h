#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <nds.h>
#include "dynamicarray.h"

namespace WoopsiUI {

	/**
	 * Class for creating sequences of bitmaps.
	 */
	class Animation {
	public:

		/**
		 * Animation status enum.
		 */
		enum Status {
			ANIMATION_STATUS_STOPPED = 0,	/**< Animation has stopped */
			ANIMATION_STATUS_PLAYING = 1,	/**< Animation is playing */
			ANIMATION_STATUS_PAUSED = 2		/**< Animation has been paused */
		};

		/**
		 * Animation loop type enum.
		 */
		enum LoopType {
			ANIMATION_LOOPTYPE_NONE = 0,	/**< Animation will not loop */
			ANIMATION_LOOPTYPE_LOOP = 1,	/**< Animation will loop */
			ANIMATION_LOOPTYPE_PINGPONG = 2	/**< Animation will loop ping-pong style */
		};

		/**
		 * Struct defining a single animation frame.
		 */
		typedef struct {
			const u16* bitmap;			/**< Pointer to the bitmap image */
			u16 width;					/**< Width of the bitmap image */
			u16 height;					/**< Height of the bitmap image */
			u8 delay;					/**< Additional number of VBLs that this frame should be displayed for */
		} AnimFrame;

		/**
		 * Constructor.
		 * @param speed Default speed of the animation.
		 * @param loopType The loop type used by the animation.
		 * @param loops The number of loops the animation should play through.
		 */
		Animation(const u8 speed, const LoopType loopType, const u16 loops);
		
		/**
		 * Destructor.
		 */
		inline ~Animation() { };

		/**
		 * Get the current animation frame.
		 * @return The current animation frame.
		 */
		inline const AnimFrame* getCurrentFrame() const { return &_frames[_currentFrame]; };
		
		/**
		 * Get the bitmap from the current animation frame.
		 * @return The current frame's bitmap.
		 */
		inline const u16* getCurrentBitmap() const { return _frames[_currentFrame].bitmap; };
		
		/**
		 * Get the current status of the animation - stopped, playing or paused.
		 * @return The current animation status.
		 */
		inline const Status getStatus() const { return _status; };
		
		/**
		 * Get the speed of the animation.
		 * @return The animation speed.
		 */
		inline const u8 getSpeed() const { return _speed; };
		
		/**
		 * Get the animation's loop type.
		 * @return The animation's loop type.
		 */
		inline const LoopType getLoopType() const { return _loopType; };
		
		/**
		 * Get the number of VBLs until the next frame is displayed.  This includes
		 * any delay that the current frame specifies.
		 * @return The number of VBLs until the next frame.
		 */
		inline const u16 getTimeToNextFrame() const { return _frameTimer; };
		
		/**
		 * Gets the total number of frames in the animation.
		 * @return The total number of frames.
		 */
		inline const u16 getFrameCount() const { return _frames.size(); };

		/**
		 * Set the speed of the animation.
		 * @param speed The new speed of the animation.
		 */
		inline void setSpeed(const u8 speed) { _speed = speed; };
		
		/**
		 * Set the loop type of the animation.
		 * @param loopType The new loop type.
		 */
		inline void setLoopType(const LoopType loopType) { _loopType = loopType; };

		/**
		 * Add a new frame to the animation.
		 * @param bitmap Pointer to the bitmap to use in the new frame.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 * @param delay A frame-specific delay to make this frame appear longer than other
		 * frames (measured in VBLs).
		 */
		void addFrame(const u16* bitmap, const u16 width, const u16 height, const u8 delay);

		/**
		 * Run the animation.  Should be called every frame.
		 */
		void run();
		
		/**
		 * Start the animation.
		 */
		void play();
		
		/**
		 * Stop the animation - does not reset the current frame as doing so will cause the 
		 * animation to jump back to the first frame when it ends.
		 */
		void stop();
		
		/**
		 * Pause the animation.
		 */
		inline void pause() { _status = ANIMATION_STATUS_PAUSED; }
		
		/**
		 * Jump to the specified frame.
		 * @param frame The frame to jump to.
		 */
		void goToFrame(u16 frame);

	private:
		DynamicArray<AnimFrame> _frames;			/**< Array of animation frames */
		u8 _speed;									/**< Speed of the animation (smaller = faster) */
		u16 _frameTimer;							/**< Time that the current frame has been on screen */
		u16 _currentFrame;							/**< Index of currently displayed frame */
		s8 _frameInc;								/**< Value added to current frame index to move to next frame */
		u16 _requestedLoops;						/**< Number of times animation should loop */
		u16 _loopCount;								/**< Number of times animation has looped */

		LoopType _loopType;							/**< Loop type of the animation */
		Status _status;								/**< Current status (playing, stopped, etc) of the animation */

		/**
		 * Attempt to loop the animation once it reaches the end.
		 * @return True if the animation looped successfully.
		 */
		bool loop();
	};
}

#endif
