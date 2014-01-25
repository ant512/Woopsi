#ifndef _PIPE_BUTTON_GRID_H_
#define _PIPE_BUTTON_GRID_H_

#include "pipebuttonbase.h"
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "gadgeteventhandler.h"
#include "graphicsport.h"
#include "woopsiarray.h"

using namespace WoopsiUI;

class PipeButtonGrid : public Gadget, public GadgetEventHandler {
public:
	PipeButtonGrid(s16 x, s16 y, u8 rows, u8 columns);
	
	void handleDropEvent(Gadget& source, const WoopsiPoint& point);
	PipeButtonBase* getStartButton();
	PipeButtonBase* getEndButton();
	bool increaseFlow(u8 increase);
	
	void redrawActiveButton();

	void reset();
	
private:
	u8 _rows;
	u8 _columns;
	u16 _buttonWidth;
	u16 _buttonHeight;
	WoopsiArray<PipeButtonBase*> _activeButtons;
	PipeButtonBase* _startButton;
	PipeButtonBase* _endButton;
	
	~PipeButtonGrid() { };
	void drawContents(GraphicsPort* port);
	bool activateNextPipe(PipeButtonBase* activePipe);
	PipeButtonBase* getPipeButtonAt(s16 column, s16 row);
	void generateRandomLayout();
	bool isButtonActive(PipeButtonBase* button);
};
	
#endif