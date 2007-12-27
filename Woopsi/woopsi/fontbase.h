#ifndef _FONT_BASE_H_
#define _FONT_BASE_H_

#include <nds.h>
#include <string.h>

#define GLYPH_MAP_SIZE 32

using namespace std;

class FontBase {

public:
	FontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour = 0);
	virtual ~FontBase();

	// Mandatory overrides
	virtual const u16 getPixel(const u32 position) const = 0;

	const u16 getPixel(const u16 x, const u16 y) const;
	const u16 getBitmapWidth() const;
	const u16 getBitmapHeight() const;
	const u8 getWidth() const;
	const u8 getHeight() const;
	
	const bool checkGlyphData(const char glyph) const;
	void setColour(const u16 colour);
	const u16 getColour() const;
	const bool isMonochrome() const;
	const u8 getTextPixelWidth(const char* text) const;
	const u16 getTransparentColour() const;
	void setTransparentColour(const u16 colour);

	void clearColour();

protected:
	void initGlyphMap();
	void createGlyphMap();

	// Mandatory overrides
	virtual const bool scanGlyph(const u16 x, const u16 y) const = 0;

private:
	u8 _width;
	u8 _height;
	u16 _bitmapWidth;
	u16 _bitmapHeight;
	u8 _glyphMap[GLYPH_MAP_SIZE];
	u16 _drawColour;
	bool _isMonochrome;
	u16 _transparentColour;
};

#endif
