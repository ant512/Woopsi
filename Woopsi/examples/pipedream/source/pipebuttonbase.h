#ifndef _PIPE_BUTTON_BASE_H_
#define _PIPE_BUTTON_BASE_H_

#include "button.h"
#include "graphicsport.h"
#include "constants.h"

using namespace WoopsiUI;

class PipeButtonBase : public Button {
public:

	typedef struct {
		bool top : 1;
		bool right : 1;
		bool bottom : 1;
		bool left : 1;
	} Connectors;

	PipeButtonBase(s16 x, s16 y, u16 width, u16 height);
	
	virtual u8 getFlowLevel() const = 0;
	virtual void increaseFlowLevel(u8 increase) = 0;

	bool isRevealed() const { return _isRevealed; };
	void resetCoordinates();
	bool canSwap() const;
	
	bool hasAvailableTopConnector() const;
	bool hasAvailableRightConnector() const;
	bool hasAvailableBottomConnector() const;
	bool hasAvailableLeftConnector() const;
	
	bool hasTopConnector() const { return _connectors.top == true; };
	bool hasRightConnector() const { return _connectors.right == true; };
	bool hasBottomConnector() const { return _connectors.bottom == true; };
	bool hasLeftConnector() const { return _connectors.left == true; };
	
	void plugTopConnector() { _pluggedConnectors.top = true; };
	void plugRightConnector() { _pluggedConnectors.right = true; };
	void plugBottomConnector() { _pluggedConnectors.bottom = true; };
	void plugLeftConnector() { _pluggedConnectors.left = true; };
	
	virtual void reveal() { _isRevealed = true; };
	
	void setOriginalCoords(s16 x, s16 y);
	s16 getOriginalX() const { return _originalX; };
	s16 getOriginalY() const { return _originalY; };
	
	u8 getAvailableConnectionCount() const;
	u8 getConnectionCount() const;
	
protected:
	virtual ~PipeButtonBase() { };
	
	void onDrag(s16 x, s16 y, s16 vX, s16 vY);
	void onClick(s16 x, s16 y);
	
	void setConnectors(const Connectors& connectors) { _connectors = connectors; };
	
private:
	bool _isRevealed;
	s16 _originalX;
	s16 _originalY;
	Connectors _pluggedConnectors;
	Connectors _connectors;
};
	
#endif