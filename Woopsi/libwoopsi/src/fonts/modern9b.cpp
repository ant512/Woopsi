#include "modern9b.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 Modern9b_glyphdata[345] = {
/* '!' */	0xFA00,
/* '"' */	0xB680,0x0000,0x0000,
/* '#' */	0x249F,0xD2FE,0x4900,0x0000,0x0000,
/* '$' */	0x23AB,0x4716,0xAE20,0x0000,
/* '%' */	0xE252,0x291D,0x7128,0x948E,0x0000,0x0000,0x0000,
/* '&' */	0x3294,0xC9C9,0xA000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x6AAA,0x4000,
/* ')' */	0x9555,0x8000,
/* '*' */	0x5D50,0x0000,0x0000,
/* '+' */	0x0109,0xF210,0x0000,0x0000,
/* ',' */	0x0380,
/* '-' */	0x00C0,0x0000,
/* '.' */	0x0200,
/* '/' */	0x2525,0x2000,0x0000,
/* '0' */	0x7463,0x18C5,0xC000,0x0000,
/* '1' */	0x2E92,0x4800,0x0000,
/* '2' */	0x7442,0x2223,0xE000,0x0000,
/* '3' */	0x7442,0x60C5,0xC000,0x0000,
/* '4' */	0x1194,0xA97C,0x4000,0x0000,
/* '5' */	0x7A3D,0x10C5,0xC000,0x0000,
/* '6' */	0x7461,0xE8C5,0xC000,0x0000,
/* '7' */	0xF844,0x2210,0x8000,0x0000,
/* '8' */	0x7462,0xE8C5,0xC000,0x0000,
/* '9' */	0x7462,0xF0C5,0xC000,0x0000,
/* ':' */	0x2200,
/* ';' */	0x2380,
/* '<' */	0x005D,0x0704,0x0000,0x0000,
/* '=' */	0x003E,0x0F80,0x0000,0x0000,
/* '>' */	0x041C,0x1740,0x0000,0x0000,
/* '?' */	0x7442,0x2200,0x8000,0x0000,
/* '@' */	0x1F18,0x25F6,0x89A2,0x69A9,0xB183,0x1F00,0x0000,0x0000,
/* 'A' */	0x1050,0xA227,0xC8A0,0x8000,0x0000,0x0000,
/* 'B' */	0xF463,0xE8C7,0xC000,0x0000,
/* 'C' */	0x7461,0x0845,0xC000,0x0000,
/* 'D' */	0xF463,0x18C7,0xC000,0x0000,
/* 'E' */	0xFC21,0xF843,0xE000,0x0000,
/* 'F' */	0xF88F,0x8880,0x0000,
/* 'G' */	0x7A18,0x2786,0x1780,0x0000,0x0000,
/* 'H' */	0x8C63,0xF8C6,0x2000,0x0000,
/* 'I' */	0xFE00,
/* 'J' */	0x1111,0x1960,0x0000,
/* 'K' */	0x8CA9,0xA946,0x2000,0x0000,
/* 'L' */	0x8888,0x88F0,0x0000,
/* 'M' */	0x838F,0x1D5A,0xB264,0x8000,0x0000,0x0000,
/* 'N' */	0x8E73,0x59CE,0x2000,0x0000,
/* 'O' */	0x7A18,0x6186,0x1780,0x0000,0x0000,
/* 'P' */	0xF463,0xE842,0x0000,0x0000,
/* 'Q' */	0x7A18,0x6196,0x3781,0x0000,0x0000,
/* 'R' */	0xF463,0xE94A,0x2000,0x0000,
/* 'S' */	0x7460,0xE0C5,0xC000,0x0000,
/* 'T' */	0xF908,0x4210,0x8000,0x0000,
/* 'U' */	0x8C63,0x18C5,0xC000,0x0000,
/* 'V' */	0x8289,0x1142,0x8204,0x0000,0x0000,0x0000,
/* 'W' */	0x88C4,0x554A,0xA551,0x1088,0x0000,0x0000,0x0000,
/* 'X' */	0x8A94,0x452A,0x2000,0x0000,
/* 'Y' */	0x8C54,0x4210,0x8000,0x0000,
/* 'Z' */	0xF124,0x48F0,0x0000,
/* '[' */	0xEAAA,0xC000,
/* '\' */	0x9124,0x4800,0x0000,
/* ']' */	0xD555,0xC000,
/* '^' */	0x6699,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0xFC00,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x003C,0x17C5,0xE000,0x0000,
/* 'b' */	0x843D,0x18C7,0xC000,0x0000,
/* 'c' */	0x001D,0x1845,0xC000,0x0000,
/* 'd' */	0x085F,0x18C5,0xE000,0x0000,
/* 'e' */	0x001D,0x1FC1,0xE000,0x0000,
/* 'f' */	0xEEA8,0x0000,
/* 'g' */	0x001F,0x18C5,0xF170,0x0000,
/* 'h' */	0x843D,0x18C6,0x2000,0x0000,
/* 'i' */	0xBE00,
/* 'j' */	0x4555,0x8000,
/* 'k' */	0x889A,0xCA90,0x0000,
/* 'l' */	0xFE00,
/* 'm' */	0x0003,0xB499,0x3264,0x8000,0x0000,0x0000,
/* 'n' */	0x003D,0x18C6,0x2000,0x0000,
/* 'o' */	0x001D,0x18C5,0xC000,0x0000,
/* 'p' */	0x003D,0x18C7,0xD080,0x0000,
/* 'q' */	0x001F,0x18C5,0xE108,0x0000,
/* 'r' */	0x03C9,0x2000,0x0000,
/* 's' */	0x001F,0x0707,0xC000,0x0000,
/* 't' */	0xAEAC,0x0000,
/* 'u' */	0x0023,0x18C5,0xE000,0x0000,
/* 'v' */	0x0008,0x5248,0xC300,0x0000,0x0000,
/* 'w' */	0x0000,0x2229,0x2551,0x1088,0x0000,0x0000,0x0000,
/* 'x' */	0x0022,0xA22A,0x2000,0x0000,
/* 'y' */	0x0022,0xA528,0x84C0,0x0000,
/* 'z' */	0x00F2,0x48F0,0x0000,
/* '{' */	0x7444,0x8444,0x7000,
/* '|' */	0xFF80,
/* '}' */	0xE222,0x1222,0xE000,
/* '~' */	0x0006,0x6600,0x0000,0x0000,0x0000,
/* 0x7F */	0x07F7,0xBDEF,0xE000,0x0000,
};

static const u16 Modern9b_offset[95] = {
    0,    1,    4,    9,   13,   20,   24,   25,   27,   29,   32,   36,   37,   39,   40,   43,
   47,   50,   54,   58,   62,   66,   70,   74,   78,   82,   83,   84,   88,   92,   96,  100,
  108,  114,  118,  122,  126,  130,  133,  138,  142,  143,  146,  150,  153,  159,  163,  168,
  172,  177,  181,  185,  189,  193,  199,  206,  210,  214,  217,  219,  222,  224,  227,  232,
  234,  238,  242,  246,  250,  254,  256,  260,  264,  265,  267,  270,  271,  277,  281,  285,
  289,  293,  296,  300,  302,  306,  311,  318,  322,  326,  329,  332,  333,  336,  341
};

static const u8 Modern9b_width[95] = {
 1, 3, 6, 5, 9, 5, 1, 2, 2, 3, 5, 1, 2, 1, 3, 5,
 3, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 5, 5, 5, 5,10,
 7, 5, 5, 5, 5, 4, 6, 5, 1, 4, 5, 4, 7, 5, 6, 5,
 6, 5, 5, 5, 5, 7, 9, 5, 5, 4, 2, 3, 2, 4, 6, 2,
 5, 5, 5, 5, 5, 2, 5, 5, 1, 2, 4, 1, 7, 5, 5, 5,
 5, 3, 5, 2, 5, 6, 9, 5, 5, 4, 4, 1, 4, 6, 5
};

Modern9b::Modern9b(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Modern9b_glyphdata,
	Modern9b_offset,
	Modern9b_width,
	12,
	3,
	6,
	10
) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
