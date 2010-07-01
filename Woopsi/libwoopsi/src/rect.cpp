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

void Rect::intersect(const Rect& rect, Rect& dest) const {
	s16 x1 = x > rect.getX() ? x : rect.getX();
	s16 y1 = y > rect.getY() ? y : rect.getY();

	s16 x2 = getX2() < rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() < rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setX2(x2);
	dest.setY2(y2);
}

void Rect::add(const Rect& rect, Rect& dest) const {
	s16 x1 = x < rect.getX() ? x : rect.getX();
	s16 y1 = y < rect.getY() ? y : rect.getY();

	s16 x2 = getX2() > rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() > rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setX2(x2);
	dest.setY2(y2);
}

Rect Rect::operator&(const Rect& rect) {
	Rect dest;
	intersect(rect, dest);
	return dest;
}

Rect Rect::operator+(const Rect& rect) {
	Rect dest;
	add(rect, dest);
	return dest;
}
