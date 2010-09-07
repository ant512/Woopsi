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

Rect fromCoordinates(s16 x1, s16 y1, s16 x2, s16 y2) {

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

	return Rect(x1, y1, width, height);
}

void Rect::getIntersect(const Rect& rect, Rect& dest) const {
	s16 x1 = x > rect.getX() ? x : rect.getX();
	s16 y1 = y > rect.getY() ? y : rect.getY();

	s16 x2 = getX2() < rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() < rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setWidth(x2 - x1 + 1);
	dest.setHeight(y2 - y1 + 1);
}

void Rect::setX2(s16 x2) {

	// Ensure that x contains the smaller value
	if (x2 < x) {
		s16 swap = x;
		x = x2;
		x2 = swap;
	}

	width = (x2 - x) + 1;
}

void Rect::setY2(s16 y2) {

	// Ensure that y contains the smaller value
	if (y2 < y) {
		s16 swap = y;
		y = y2;
		y2 = swap;
	}

	height = (y2 - y) + 1;
}

void Rect::getAddition(const Rect& rect, Rect& dest) const {
	s16 x1 = x < rect.getX() ? x : rect.getX();
	s16 y1 = y < rect.getY() ? y : rect.getY();

	s16 x2 = getX2() > rect.getX2() ? getX2() : rect.getX2();
	s16 y2 = getY2() > rect.getY2() ? getY2() : rect.getY2();

	dest.setX(x1);
	dest.setY(y1);
	dest.setWidth(x2 - x1 + 1);
	dest.setHeight(y2 - x1 + 1);
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
	if (width < 1) return false;
	if (height < 1) return false;
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

bool Rect::operator==(const Rect& rect) {
	return (x == rect.x &&
			y == rect.y &&
			width == rect.width &&
			height == rect.height);
}

bool Rect::operator!=(const Rect& rect) {
	return !(this->operator== (rect));
}

bool Rect::intersects(const Rect& rect) const {
	return ((x + width > rect.getX()) &&
			(y + height > rect.getY()) &&
			(x < rect.getX() + rect.getWidth()) &&
			(y < rect.getY() + rect.getHeight()));
}

bool Rect::contains(s16 x, s16 y) const {
	return ((x >= this->x) &&
			(y >= this->y) &&
			(x < this->x + this->width) &&
			(y < this->y + this->height));
}

void Rect::copyTo(Rect& rect) const {
	rect.setX(x);
	rect.setY(y);
	rect.setWidth(width);
	rect.setHeight(height);
}

bool Rect::splitIntersection(const Rect& rect, Rect& intersection, WoopsiArray<Rect>* remainderRects) const {

	if (intersects(rect)) {

		// Copy the properties of rect into intersection; we trim this to size later
		intersection.x = rect.x;
		intersection.y = rect.y;
		intersection.width = rect.width;
		intersection.height = rect.height;

		// Check for a non-overlapped rect on the left
		if (intersection.x < x) {
			Rect left;
			left.x = intersection.x;
			left.y = intersection.y;
			left.width = x - intersection.x;
			left.height = intersection.height;
			
			// Insert the rect
			remainderRects->insert(0, left);
			
			// Adjust the dimensions of the intersection
			intersection.x = x;
			intersection.width -= left.width;
		}
		
		// Check for a non-overlapped rect on the right
		if (intersection.x + intersection.width > x + width) {
			Rect right;
			right.x = x + width;
			right.y = intersection.y;
			right.width = intersection.width - (x + width - intersection.x);
			right.height = intersection.height;
			
			// Insert the rect
			remainderRects->insert(0, right);
			
			// Adjust dimensions of the intersection
			intersection.width -= right.width;
		}
		
		// Check for a non-overlapped rect above
		if (intersection.y < y) {
			Rect top;
			top.x = intersection.x;
			top.y = intersection.y;
			top.width = intersection.width;
			top.height = y - intersection.y;
			
			// Insert the rect
			remainderRects->insert(0, top);
			
			// Adjust the dimensions of the intersection
			intersection.y = y;
			intersection.height -= top.height;
		}
		
		// Check for a non-overlapped rect below
		if (intersection.y + intersection.height > y + height) {
			Rect bottom;
			bottom.x = intersection.x;
			bottom.y = y + height;
			bottom.width = intersection.width;
			bottom.height = intersection.height - (y + height - intersection.y);
			
			// Insert the rect
			remainderRects->insert(0, bottom);
			
			// Adjust dimensions of the intersection
			intersection.height -= bottom.height;
		}
		
		return true;
	}

	return false;
}