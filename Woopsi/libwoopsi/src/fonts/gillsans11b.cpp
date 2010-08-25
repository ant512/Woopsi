#include "gillsans11b.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 GillSans11b_glyphdata[332] = {
/* '!' */	0xFFEF,0x0000,
/* '"' */	0xFC00,0x0000,
/* '#' */	0x297E,0xA57E,0x9400,0x0000,
/* '$' */	0x77ED,0x3C3D,0x37EE,0x0000,0x0000,
/* '%' */	0xE4A8,0xA8C8,0x1615,0x1526,0x0000,0x0000,
/* '&' */	0x30D1,0xA3A7,0xDF37,0x7B00,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x364C,0xCCCC,0x6300,
/* ')' */	0x9936,0xDAD0,0x0000,
/* '*' */	0x757E,0xEFD5,0xC000,0x0000,
/* '+' */	0x0109,0xFF90,0x8000,0x0000,
/* ',' */	0x003D,0x8000,
/* '-' */	0x000F,0xC000,0x0000,
/* '.' */	0x003F,0x0000,
/* '/' */	0x55AA,0x0000,
/* '0' */	0x39EF,0xF3CF,0xF78E,0x0000,0x0000,
/* '1' */	0x7FFF,0x0000,
/* '2' */	0x6FF3,0x366F,0x0000,
/* '3' */	0xEF3E,0x33FE,0x0000,
/* '4' */	0x339D,0x6FFC,0xC600,0x0000,
/* '5' */	0x76EF,0x333E,0x0000,
/* '6' */	0x7BBD,0xFDFF,0xEE00,0x0000,
/* '7' */	0xF98C,0x6631,0x9800,0x0000,
/* '8' */	0x77F7,0xFFEF,0xEE00,0x0000,
/* '9' */	0x77FF,0xBFBD,0xCE00,0x0000,
/* ':' */	0x0F0F,0x0000,
/* ';' */	0x0FCF,0x6000,
/* '<' */	0x013E,0xCE31,0x0000,
/* '=' */	0x0FF0,0xFF00,0x0000,
/* '>' */	0x08C7,0x37C8,0x0000,
/* '?' */	0x67F3,0x2266,0x0000,
/* '@' */	0x3C89,0x6D5A,0xB56B,0x5E40,0x7800,0x0000,
/* 'A' */	0x3871,0xB367,0xCFB1,0xE300,0x0000,0x0000,
/* 'B' */	0xF7F7,0xEFEF,0xFE00,0x0000,
/* 'C' */	0x7FFD,0x8C7B,0xEF00,0x0000,
/* 'D' */	0xF7FF,0xBDFF,0xFE00,0x0000,
/* 'E' */	0xFF7F,0xBF00,0x0000,
/* 'F' */	0xFF7F,0xB600,0x0000,
/* 'G' */	0x6FFC,0xDFF7,0x0000,
/* 'H' */	0xDEF7,0xFFEF,0x7B00,0x0000,
/* 'I' */	0xFFFF,0x0000,
/* 'J' */	0x6DB7,0xFB00,0x0000,
/* 'K' */	0xDEFF,0xEFEF,0x7B00,0x0000,
/* 'L' */	0xCCCC,0xCFFF,0x0000,
/* 'M' */	0xC7BF,0xFFD7,0x5C71,0x0000,0x0000,
/* 'N' */	0x8E7B,0xFFDE,0x7100,0x0000,
/* 'O' */	0x6FFD,0xDFF6,0x0000,
/* 'P' */	0xF7F7,0xFF63,0x1800,0x0000,
/* 'Q' */	0x6FFD,0xDFF7,0x0000,
/* 'R' */	0xF7F7,0xFF7F,0x7B00,0x0000,
/* 'S' */	0x7FE7,0xFF00,0x0000,
/* 'T' */	0xFFF6,0x6666,0x0000,
/* 'U' */	0xDDDD,0xDFF6,0x0000,
/* 'V' */	0xCF37,0x9E79,0xE30C,0x0000,0x0000,
/* 'W' */	0xC9EE,0xF76F,0xE773,0xB8D8,0x6C00,0x0000,0x0000,
/* 'X' */	0xEED9,0xF1C3,0x8F9B,0x7700,0x0000,0x0000,
/* 'Y' */	0xCDE7,0x9E30,0xC30C,0x0000,0x0000,
/* 'Z' */	0xFFE1,0x8C31,0x87FF,0x0000,0x0000,
/* '[' */	0xFF6D,0xB6FC,0x0000,
/* '\' */	0xAA55,0x0000,
/* ']' */	0xFDB6,0xDBFC,0x0000,
/* '^' */	0x2115,0x1000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0F00,
/* '`' */	0xD000,0x0000,
/* 'a' */	0x00EF,0x3FBF,0x0000,
/* 'b' */	0xC63D,0xFDEF,0xFE00,0x0000,
/* 'c' */	0x007F,0xCCF7,0x0000,
/* 'd' */	0x18DF,0xFDEF,0xEF00,0x0000,
/* 'e' */	0x006D,0xFCF6,0x0000,
/* 'f' */	0x3776,0xF666,0x0000,
/* 'g' */	0x001F,0xFDFD,0xF3FF,0x8000,
/* 'h' */	0xC63D,0xFDEF,0x7B00,0x0000,
/* 'i' */	0x4FFF,0x0000,
/* 'j' */	0x21B6,0xDFF8,0x0000,
/* 'k' */	0xC637,0xBF6F,0x7B00,0x0000,
/* 'l' */	0xFFFF,0x0000,
/* 'm' */	0x0000,0xF6FF,0xDBDB,0xDBDB,0x0000,0x0000,
/* 'n' */	0x003D,0xFDEF,0x7B00,0x0000,
/* 'o' */	0x006F,0xDDF6,0x0000,
/* 'p' */	0x003D,0xFDEF,0xFEC6,0x0000,
/* 'q' */	0x001F,0xFDEF,0xEF18,0xC000,
/* 'r' */	0x00FF,0xFCCC,0x0000,
/* 's' */	0x007F,0xC3FE,0x0000,
/* 't' */	0x04FF,0xCCF7,0x0000,
/* 'u' */	0x0037,0xBDEF,0xEF00,0x0000,
/* 'v' */	0x00DD,0xF666,0x0000,
/* 'w' */	0x000D,0x757D,0xE69A,0x0000,0x0000,
/* 'x' */	0x0036,0xE739,0xDB00,0x0000,
/* 'y' */	0x0037,0xB739,0xC663,0x0000,
/* 'z' */	0x00FE,0x247F,0x0000,
/* '{' */	0x692C,0x924C,0x0000,
/* '|' */	0xFF80,
/* '}' */	0xC924,0xD24B,0x0000,
/* '~' */	0x000C,0x3000,0x0000,
/* 0x7F */	0xF999,0x9999,0xF000,
};

static const u16 GillSans11b_offset[95] = {
    0,    2,    4,    8,   13,   19,   25,   26,   29,   32,   36,   40,   42,   45,   47,   49,
   54,   56,   59,   62,   66,   69,   73,   77,   81,   85,   87,   89,   92,   95,   98,  101,
  107,  113,  117,  121,  125,  128,  131,  134,  138,  140,  143,  147,  150,  155,  159,  162,
  166,  169,  173,  176,  179,  182,  187,  194,  200,  205,  210,  213,  215,  218,  222,  225,
  227,  230,  234,  237,  241,  244,  247,  251,  255,  257,  260,  264,  266,  272,  276,  279,
  283,  287,  290,  293,  296,  300,  303,  308,  312,  316,  319,  322,  323,  326,  329
};

static const u8 GillSans11b_width[95] = {
 2, 2, 5, 6, 8, 7, 1, 4, 3, 5, 5, 2, 3, 2, 2, 6,
 2, 4, 4, 5, 4, 5, 5, 5, 5, 2, 2, 4, 4, 4, 4, 7,
 7, 5, 5, 5, 3, 3, 4, 5, 2, 3, 5, 4, 6, 5, 4, 5,
 4, 5, 3, 4, 4, 6, 9, 7, 6, 6, 3, 2, 3, 5, 4, 2,
 4, 5, 4, 5, 4, 4, 5, 5, 2, 3, 5, 2, 8, 5, 4, 5,
 5, 4, 4, 4, 5, 4, 6, 5, 5, 4, 3, 1, 3, 4, 4
};

GillSans11b::GillSans11b(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	GillSans11b_glyphdata,
	GillSans11b_offset,
	GillSans11b_width,
	12,
	3,
	7,
	9
) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
