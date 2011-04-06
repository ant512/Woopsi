#ifndef _PIPE_BUTTON_STANDARD_H_
#define _PIPE_BUTTON_STANDARD_H_

#include "pipebuttonbase.h"
#include "graphicsport.h"
#include "constants.h"

using namespace WoopsiUI;

class PipeButtonStandard : public PipeButtonBase {
public:
	PipeButtonStandard(s16 x, s16 y, u16 width, u16 height, PipeButtonBase::Connectors connectors);
	PipeButtonStandard(s16 x, s16 y, u16 width, u16 height, bool top, bool right, bool bottom, bool left);
	
	virtual u8 getFlowLevel() const { return _flowLevel; };
	virtual void increaseFlowLevel(u8 increase);
	
protected:
	virtual ~PipeButtonStandard() { };
	void drawContents(GraphicsPort* port);
	
private:
	u8 _flowLevel;
};
	
#endif