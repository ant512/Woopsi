#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <nds.h>
#include "button.h"

using namespace std;

class CheckBox : public Button {

public:
	enum CheckBoxState {
		CHECK_BOX_STATE_OFF = 0,
		CHECK_BOX_STATE_ON = 1,
		CHECK_BOX_STATE_MU = 2
	};

	CheckBox(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

	virtual void draw(Rect clipRect);
	virtual void draw();
	virtual bool click(s16 x, s16 y);
	virtual CheckBoxState getState();
	virtual void setState(CheckBoxState state);

protected:
	CheckBoxState _state;

	/**
	 * Destructor.
	 */
	virtual inline ~CheckBox() { };
};

#endif
