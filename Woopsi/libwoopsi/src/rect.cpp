#include "rect.h"

using namespace WoopsiUI;

Rect::Rect() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Rect::Rect(s16 x, s16 y, s32 width, s32 height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Rect::Rect(const Rect& rect) {
	this->x = rect.getX();
	this->y = rect.getY();
	this->width = rect.getWidth();
	this->height = rect.getHeight();
}

Rect* fromDimensions(s16 x, s16 y, s32 width, s32 height) {
	return new Rect(x, y, width, height);
}

Rect* fromCoordinates(s16 x1, s16 y1, s16 x2, s16 y2) {

	// Ensure x2 is the larger value
	if (x2 < x1) {
		s16 swap = x1;
		x1 = x2;
		x2 = swap;
	}

	// Ensure y2 is the larger value
	if (y2 < y1) {
		s16 swap = y1;
		y1 = y2;
		y2 = swap;
	}

	s32 width = (x2 - x1) + 1;
	s32 height = (y2 - y1) + 1;

	return new Rect(x1, y1, width, height);
}



void Rect::getIntersect(const Rect& rect, Rect& dest) const {
	s16 x1 = x > rect.getX() ? x : rect.getX();
	s16 y1 = y > rect.getY() ? y : rect.getY();

	s16 x2 = getX2() < rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() < rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setX2(x2);
	dest.setY2(y2);
}

void Rect::getAddition(const Rect& rect, Rect& dest) const {
	s16 x1 = x < rect.getX() ? x : rect.getX();
	s16 y1 = y < rect.getY() ? y : rect.getY();

	s16 x2 = getX2() > rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() > rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setX2(x2);
	dest.setY2(y2);
}

void Rect::clipToIntersect(const Rect& rect) {
	Rect clipped;
	getIntersect(rect, clipped);

	setX(clipped.getX());
	setY(clipped.getY());
	setWidth(clipped.getWidth());
	setHeight(clipped.getHeight());
}

void Rect::expandToInclude(const Rect& rect) {
	Rect addition;
	getAddition(rect, addition);

	setX(addition.getX());
	setY(addition.getY());
	setWidth(addition.getWidth());
	setHeight(addition.getHeight());
}

bool Rect::hasDimensions() const {
	if (!width) return false;
	if (!height) return false;
	return true;
}

Rect Rect::operator&(const Rect& rect) {
	Rect dest;
	getIntersect(rect, dest);
	return dest;
}

Rect Rect::operator+(const Rect& rect) {
	Rect dest;
	getAddition(rect, dest);
	return dest;
}

bool Rect::intersects(const Rect& rect) const {

	if ((x + width > rect.getX()) &&
		(y + height > rect.getY()) &&
		(x < rect.getX() + rect.getWidth()) &&
		(y < rect.getY() + rect.getHeight())) {
		return true;
	}

	return false;
}
