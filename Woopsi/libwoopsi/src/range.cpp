#include "range.h"

using namespace WoopsiUI;

Range::Range(s32 minimumValue, s32 maximumValue, s32 span) {
	_minimumValue = minimumValue;
	_maximumValue = maximumValue;
	_span = span;
}

const s32 Range::convertValueToScaled(s32 value) const {

	if (_span <= 0) return 0;
	if (value < _minimumValue) return 0;
	if (_maximumValue < _minimumValue) return 0;
	if (value > _maximumValue) return _span;
	
	s32 range = _maximumValue - _minimumValue;
	s32 p = value - _minimumValue;
	
	if (range > _span) {
		return (2 * p * _span + range) / (2 * range);
	} else {
		u32 div = _span / range;
		u32 mod = _span % range;
		return p * div + (2 * p * mod + range) / (2 * range);
	}
}

const s32 Range::convertScaledToValue(s32 scaledValue) const {

	if (_span <= 0) return _minimumValue;
	if (scaledValue <= 0) return _minimumValue;
	if (scaledValue >= _span) return _maximumValue;
	
	s32 range = _maximumValue - _minimumValue;
	
	if (_span > range) {
		return _minimumValue + (2 * scaledValue * range + _span) / (2 * _span);
	} else {
		u32 div = range / _span;
		u32 mod = range % _span;
		return _minimumValue + scaledValue * div + (2 * scaledValue * mod + _span) / (2 * _span);
	}
}
