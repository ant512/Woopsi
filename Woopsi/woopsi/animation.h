#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <NDS.h>
#include <vector>

using namespace std;

class Animation {
public:
	enum Status {
		ANIMATION_STATUS_STOPPED = 0,
		ANIMATION_STATUS_PLAYING = 1,
		ANIMATION_STATUS_PAUSED = 2
	};

	enum LoopType {
		ANIMATION_LOOPTYPE_NONE = 0,
		ANIMATION_LOOPTYPE_LOOP = 1,
		ANIMATION_LOOPTYPE_PINGPONG = 2
	};

	typedef struct {
		const u16* bitmap;
		u16 width;
		u16 height;
		u8 delay;
	} AnimFrame;

	Animation(const u8 speed, const LoopType loopType, const u16 loops);
	~Animation();

	// Getters
	const AnimFrame* getCurrentFrame() const;
	const u16* getCurrentBitmap() const;
	const Status getStatus() const;
	const u8 getSpeed() const;
	const LoopType getLoopType() const;
	const u16 getTimeToNextFrame() const;
	const u16 getFrameCount() const;

	// Setters
	void setSpeed(const u8 speed);
	void setLoopType(const LoopType loopType);

	void addFrame(const u16* bitmap, const u16 width, const u16 height, const u8 delay);

	// Playback control
	void run();
	void play();
	void stop();
	void pause();
	void goToFrame(u16 frame);

private:
	vector<AnimFrame> _frames;
	u8 _speed;
	u16 _frameTimer;
	u16 _currentFrame;
	s8 _frameInc;
	u16 _requestedLoops;
	u16 _loopCount;


	LoopType _loopType;
	Status _status;

	bool loop();
};

#endif
