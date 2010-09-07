#include "pipebuttonbase.h"
#include "woopsifuncs.h"

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
	markRectsDirty();
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
