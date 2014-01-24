#include "pipebuttonbase.h"
#include "woopsifuncs.h"
#include "fontbase.h"

PipeButtonBase::PipeButtonBase(s16 x, s16 y, u16 width, u16 height) : Button(x, y, width, height, "") {
	_isRevealed = false;
	_flags.draggable = true;
	_originalX = x;
	_originalY = y;
	
	_borderSize.top = 1;
	_borderSize.right = 1;
	_borderSize.bottom = 1;
	_borderSize.left = 1;
	
	_connectors.top = 0;
	_connectors.right = 0;
	_connectors.bottom = 0;
	_connectors.left = 0;
	
	_pluggedConnectors.top = 0;
	_pluggedConnectors.right = 0;
	_pluggedConnectors.bottom = 0;
	_pluggedConnectors.left = 0;
}

bool PipeButtonBase::hasAvailableTopConnector() const {
	return (!_pluggedConnectors.top) && _connectors.top && _isRevealed;
}

bool PipeButtonBase::hasAvailableRightConnector() const {
	return (!_pluggedConnectors.right) && _connectors.right && _isRevealed;
}

bool PipeButtonBase::hasAvailableBottomConnector() const {
	return (!_pluggedConnectors.bottom) && _connectors.bottom && _isRevealed;
}

bool PipeButtonBase::hasAvailableLeftConnector() const {
	return (!_pluggedConnectors.left) && _connectors.left && _isRevealed;
}

void PipeButtonBase::resetCoordinates() {
	moveTo(_originalX, _originalY);
}

void PipeButtonBase::onDrag(s16 x, s16 y, s16 vX, s16 vY) {

	s16 destX = x - _parent->getX();
	s16 destY = y - _parent->getY();
	
	// Align to grid
	destX -= destX % getWidth();
	destY -= destY % getHeight();
	
	moveTo(destX, destY);
}

void PipeButtonBase::onClick(s16 x, s16 y) {

	if (!isRevealed()) reveal();
	raiseToTop();
	if (_flags.draggable) startDragging(x, y);
	markRectsDamaged();
}

bool PipeButtonBase::canSwap() const {
	if (!isEnabled()) return false;
	if (!isRevealed()) return false;
	return true;
}

void PipeButtonBase::setOriginalCoords(s16 x, s16 y) {
	_originalX = x;
	_originalY = y;
}

u8 PipeButtonBase::getAvailableConnectionCount() const {
	u8 count = 0;
	
	if (hasAvailableTopConnector()) count++;
	if (hasAvailableRightConnector()) count++;
	if (hasAvailableBottomConnector()) count++;
	if (hasAvailableLeftConnector()) count++;
	
	return count;
}

u8 PipeButtonBase::getConnectionCount() const {
	u8 count = 0;
	
	if (hasTopConnector()) count++;
	if (hasRightConnector()) count++;
	if (hasBottomConnector()) count++;
	if (hasLeftConnector()) count++;
	
	return count;
}

void PipeButtonBase::drawUnrevealedContents(GraphicsPort* port) {
	u16 width = _style.font->getStringWidth("?");
	u16 height = _style.font->getHeight();
	
	port->drawText((getWidth() - width) / 2, (getHeight() - height) / 2, _style.font, "?");
}

void PipeButtonBase::drawSpark(s16 x, s16 y, GraphicsPort* port) {
	u16 redRadius = rand() % 5;
	u16 yellowRadius = redRadius - rand() % 3;
	u16 whiteRadius = 1;
	port->drawFilledEllipse(x, y, redRadius, redRadius, woopsiRGB(31, 0, 0));
	port->drawFilledEllipse(x, y, yellowRadius, yellowRadius, woopsiRGB(31, 31, 0));
	port->drawFilledEllipse(x, y, whiteRadius, whiteRadius, woopsiRGB(31, 31, 31));
}
