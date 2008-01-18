#ifndef _SLIDER_VERTICAL_H_
#define _SLIDER_VERTICAL_H_

#include <nds.h>
#include "gadget.h"
#include "graphicsport.h"
#include "sliderverticalgrip.h"
#include "eventhandler.h"

using namespace std;

class SliderVertical : public Gadget, public EventHandler {

public:

	SliderVertical(s16 x, s16 y, u16 width, u16 height);
	virtual ~SliderVertical();

	const s16 getMinimumValue() const;
	const s16 getMaximumValue() const;
	const s16 getValue() const;

	void setMinimumValue(const s16 value);
	void setMaximumValue(const s16 value);
	void setValue(const s16 value);

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	virtual bool handleEvent(const EventArgs& e);

protected:
	SliderVerticalGrip* _grip;
	s16 _minimumValue;
	s16 _maximumValue;
	s16 _minimumGripHeight;

	void resizeGrip();
};

#endif
