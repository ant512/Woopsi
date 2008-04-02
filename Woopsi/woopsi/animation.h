#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <nds.h>
#include "dynamicarray.h"

using namespace std;

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
		const u16* bitmap;
		u16 width;
		u16 height;
		u8 delay;
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
	const AnimFrame* getCurrentFrame() const;
	
	/**
	 * Get the bitmap from the current animation frame.
	 * @return The current frame's bitmap.
	 */
	const u16* getCurrentBitmap() const;
	
	/**
	 * Get the current status of the animation - stopped, playing or paused.
	 * @return The current animation status.
	 */
	const Status getStatus() const;
	
	/**
	 * Get the speed of the animation.
	 * @return The animation speed.
	 */
	const u8 getSpeed() const;
	
	/**
	 * Get the animation's loop type.
	 * @return The animation's loop type.
	 */
	const LoopType getLoopType() const;
	
	/**
	 * Get the number of VBLs until the next frame is displayed.  This includes
	 * any delay that the current frame specifies.
	 * @return The number of VBLs until the next frame.
	 */
	const u16 getTimeToNextFrame() const;
	
	/**
	 * Gets the total number of frames in the animation.
	 * @return The total number of frames.
	 */
	const u16 getFrameCount() const;

	/**
	 * Set the speed of the animation.
	 * @param speed The new speed of the animation.
	 */
	void setSpeed(const u8 speed);
	
	/**
	 * Set the loop type of the animation.
	 * @param loopType The new loop type.
	 */
	void setLoopType(const LoopType loopType);

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
	void pause();
	
	/**
	 * Jump to the specified frame.
	 * @param frame The frame to jump to.
	 */
	void goToFrame(u16 frame);

private:
	DynamicArray<AnimFrame> _frames;
	u8 _speed;
	u16 _frameTimer;
	u16 _currentFrame;
	s8 _frameInc;
	u16 _requestedLoops;
	u16 _loopCount;

	LoopType _loopType;
	Status _status;

	/**
	 * Attempt to loop the animation once it reaches the end.
	 * @return True if the animation looped successfully.
	 */
	bool loop();
};

#endif
