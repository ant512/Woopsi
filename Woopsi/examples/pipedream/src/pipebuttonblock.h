#ifndef _PIPE_BUTTON_BLOCK_H_
#define _PIPE_BUTTON_BLOCK_H_

#include "pipebuttonbase.h"
#include "graphicsport.h"
#include "constants.h"

using namespace WoopsiUI;

class PipeButtonBlock : public PipeButtonBase {
public:
	PipeButtonBlock(s16 x, s16 y, u16 width, u16 height);
	
	virtual u8 getFlowLevel() const { return 0; };
	virtual void increaseFlowLevel(u8 increase) { };
	
	virtual void reveal();
	
protected:
	virtual ~PipeButtonBlock() { };
	
	void drawContents(GraphicsPort* port);
};
	
#endif
