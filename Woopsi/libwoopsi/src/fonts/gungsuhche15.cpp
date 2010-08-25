#include "gungsuhche15.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 Gungsuhche15_glyphdata[596] = {
/* '!' */	0xFFFF,0xC3C0,
/* '"' */	0xDEE4,0x0000,0x0000,0x0000,0x0000,
/* '#' */	0x6CD9,0xB7F6,0xCD9B,0x366D,0xFDB3,0x66C0,0x0000,
/* '$' */	0x3060,0xDB7C,0x78DC,0x0E07,0x8F1F,0x6D83,0x0600,
/* '%' */	0x00C3,0xC48F,0x2DC6,0x38CC,0x3C48,0xF0C0,0x0000,
/* '&' */	0x0071,0x8306,0x041C,0x3BDD,0x9B36,0x77A0,0x0000,
/* ''' */	0xF800,0x0000,
/* '(' */	0x1366,0xCCCC,0xCC66,0x3100,
/* ')' */	0x8C66,0x3333,0x3326,0xC800,
/* '*' */	0x0000,0x0CB7,0xF31E,0x4800,0x0000,0x0000,
/* '+' */	0x0000,0x0C30,0xCFCC,0x30C0,0x0000,0x0000,
/* ',' */	0x0000,0x03D8,
/* '-' */	0x0000,0x0000,0x003F,0x8000,0x0000,0x0000,0x0000,
/* '.' */	0x0000,0x03C0,
/* '/' */	0x0410,0xC218,0x4308,0x610C,0x2080,0x0000,
/* '0' */	0x0071,0xB63C,0x78F1,0xE3C7,0x8F1B,0x6380,0x0000,
/* '1' */	0x03C3,0x0C30,0xC30C,0x30C3,0x0CFC,0x0000,
/* '2' */	0x00FB,0x1E3C,0x6186,0x1860,0x839E,0xBCC0,0x0000,
/* '3' */	0x01FF,0x1E6C,0xC30E,0x0606,0x0F1E,0x6780,0x0000,
/* '4' */	0x0018,0x70E3,0xC59B,0x26CD,0xFC30,0x63E0,0x0000,
/* '5' */	0x01FF,0x1E3C,0x1F33,0x6306,0x0F1E,0x6780,0x0000,
/* '6' */	0x0079,0x9E3C,0x1F3B,0x63C7,0x8F1B,0x6380,0x0000,
/* '7' */	0x01EF,0x3E3C,0x4183,0x0618,0x3060,0xC180,0x0000,
/* '8' */	0x0071,0xB63C,0x6D8E,0x36C7,0x8F1B,0x6380,0x0000,
/* '9' */	0x0071,0xB63C,0x78F1,0xB736,0x0F1E,0x6780,0x0000,
/* ':' */	0x03C0,0x3C00,
/* ';' */	0x0036,0x0001,0xBC00,
/* '<' */	0x0000,0x0031,0xCE30,0x6070,0x3818,0x0000,0x0000,
/* '=' */	0x0000,0x0000,0x1FC0,0x00FE,0x0000,0x0000,0x0000,
/* '>' */	0x0000,0x0607,0x0381,0x831C,0xE300,0x0000,0x0000,
/* '?' */	0x38DB,0x1E30,0x6186,0x1830,0x6001,0x8300,0x0000,
/* '@' */	0x0071,0xB63D,0xFAF5,0xEBDF,0x8199,0xE000,0x0000,
/* 'A' */	0x00E0,0x41C3,0x850A,0x367C,0xD9B3,0x6EE0,0x0000,
/* 'B' */	0x01F9,0x9B36,0x6CDF,0x3366,0xCD9B,0x3FC0,0x0000,
/* 'C' */	0x0075,0xBE3C,0x3870,0x60C1,0x830B,0x33C0,0x0000,
/* 'D' */	0x01F1,0xB336,0x6CD9,0xB366,0xCD9B,0x6F80,0x0000,
/* 'E' */	0x01FD,0x9B36,0x0C9F,0x3264,0xC19B,0x3FE0,0x0000,
/* 'F' */	0x01FD,0x9B36,0x0C9F,0x3264,0xC183,0x0F80,0x0000,
/* 'G' */	0x006D,0xBE3C,0x7830,0x6FC7,0x8F1B,0x7360,0x0000,
/* 'H' */	0x018F,0x1E3C,0x78FF,0xE3C7,0x8F1E,0x3C60,0x0000,
/* 'I' */	0x03F3,0x0C30,0xC30C,0x30C3,0x0CFC,0x0000,
/* 'J' */	0x00FC,0x60C1,0x8306,0x0C19,0xB366,0xCF00,0x0000,
/* 'K' */	0x01DD,0x9B66,0xCF1C,0x3C6C,0xD99B,0x3EE0,0x0000,
/* 'L' */	0x01E1,0x8306,0x0C18,0x3060,0xCD9B,0x3FE0,0x0000,
/* 'M' */	0x018F,0x1F7E,0xFDFF,0xEBD7,0xAF1E,0x3C60,0x0000,
/* 'N' */	0x01DD,0x9B37,0x6EDD,0xB76E,0xDD9B,0x3F60,0x0000,
/* 'O' */	0x0071,0xB63C,0x78F1,0xE3C7,0x8F1B,0x6380,0x0000,
/* 'P' */	0x01F1,0xB336,0x6CD9,0xB678,0xC183,0x0F80,0x0000,
/* 'Q' */	0x0071,0xB63C,0x78F1,0xE3C7,0xAFBB,0x63C0,0xC000,
/* 'R' */	0x01F9,0x9B36,0x6CDB,0x3C6C,0xCD9B,0x3F60,0x0000,
/* 'S' */	0x0075,0xBE3C,0x385C,0x0E87,0x0F1F,0x6B80,0x0000,
/* 'T' */	0x03FB,0x6DB4,0xC30C,0x30C3,0x0C78,0x0000,
/* 'U' */	0x01DD,0x9B36,0x6CD9,0xB366,0xCD9B,0x33C0,0x0000,
/* 'V' */	0x01DD,0xB366,0xCD9B,0x3638,0x70E0,0x8100,0x0000,
/* 'W' */	0x018F,0x1E3C,0x7AF5,0xEBFE,0xF9B3,0x66C0,0x0000,
/* 'X' */	0x01DF,0xBA26,0xCD8E,0x1C6C,0xD917,0x7EE0,0x0000,
/* 'Y' */	0x01DD,0x9B36,0x6CCF,0x0C18,0x3060,0xC3C0,0x0000,
/* 'Z' */	0x03FC,0xF3D8,0x630C,0x61BC,0xF3FC,0x0000,
/* '[' */	0xFCCC,0xCCCC,0xCCCC,0xCF00,
/* '\' */	0x0063,0x636B,0x6BEB,0x6B6B,0x3E3E,0x3636,0x3600,0x0000,
/* ']' */	0xF333,0x3333,0x3333,0x3F00,
/* '^' */	0x23B6,0x0000,0x0000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,0xC000,
/* '`' */	0xCC00,0x0000,0x0000,
/* 'a' */	0x0000,0x0007,0x9983,0x3ECD,0x9B36,0xF760,0x0000,
/* 'b' */	0x01C1,0x8307,0x8D99,0xB366,0xCD9B,0x6780,0x0000,
/* 'c' */	0x0000,0x0003,0x6DF1,0xE0C1,0x831B,0x33C0,0x0000,
/* 'd' */	0x0078,0x3063,0xCDB3,0x66CD,0x9B33,0xE360,0x0000,
/* 'e' */	0x0000,0x0003,0x8DB1,0xE3FF,0x830B,0x33C0,0x0000,
/* 'f' */	0x003C,0xC183,0x1FCC,0x1830,0x60C1,0x87C0,0x0000,
/* 'g' */	0x0000,0x0007,0xF9B3,0x66CD,0x99F0,0x60DF,0x0000,
/* 'h' */	0x01C1,0x8306,0xCED9,0xB366,0xCD9B,0x3EE0,0x0000,
/* 'i' */	0x00C3,0x00F0,0xC30C,0x30C3,0x0CFC,0x0000,
/* 'j' */	0x00C6,0x078C,0x6318,0xC631,0xF800,
/* 'k' */	0x01C1,0x8306,0xEC9B,0x3C78,0xD9B3,0x3EE0,0x0000,
/* 'l' */	0x03C3,0x0C30,0xC30C,0x30C3,0x0CFC,0x0000,
/* 'm' */	0x0000,0x000A,0xDAF5,0xEBD7,0xAF5E,0xBD60,0x0000,
/* 'n' */	0x0000,0x000D,0x8D9B,0x366C,0xD9B3,0x6FE0,0x0000,
/* 'o' */	0x0000,0x0003,0x8DB1,0xE3C7,0x8F1B,0x6380,0x0000,
/* 'p' */	0x0000,0x000F,0xCCD9,0xB366,0xCDF3,0x061E,0x0000,
/* 'q' */	0x0000,0x0007,0xF9B3,0x66CD,0x99F0,0x60C3,0xC000,
/* 'r' */	0x0000,0x000E,0xEED9,0xB060,0xC183,0x0F80,0x0000,
/* 's' */	0x0000,0x007F,0x3CD8,0x306C,0xF3F8,0x0000,
/* 't' */	0x00C1,0x830F,0xCC18,0x3060,0xC18B,0x33C0,0x0000,
/* 'u' */	0x0000,0x000E,0xECD9,0xB366,0xCD9B,0x33E0,0x0000,
/* 'v' */	0x0000,0x000E,0xED9B,0x366C,0x70E0,0x8100,0x0000,
/* 'w' */	0x0000,0x000C,0x78F5,0xEBFF,0xDDB2,0x2440,0x0000,
/* 'x' */	0x0000,0x000E,0xF8DB,0x1C10,0x71B6,0x3EE0,0x0000,
/* 'y' */	0x0000,0x000C,0x78F1,0xB66C,0x70C1,0x861E,0x0000,
/* 'z' */	0x0000,0x00FF,0x3D86,0x3186,0xF3FC,0x0000,
/* '{' */	0x1CC3,0x0C30,0xCE0C,0x30C3,0x0C30,0x7000,
/* '|' */	0xFFFF,0xFFF0,
/* '}' */	0xE0C3,0x0C30,0xC1CC,0x30C3,0x0C33,0x8000,
/* '~' */	0x0000,0x0000,0x0EF7,0x0000,0x0000,0x0000,0x0000,
/* 0x7F */	0x000F,0xE186,0x1861,0x8618,0x61FC,0x0000,
};

static const u16 Gungsuhche15_offset[95] = {
    0,    2,    7,   14,   21,   28,   35,   37,   41,   45,   51,   57,   59,   66,   68,   74,
   81,   87,   94,  101,  108,  115,  122,  129,  136,  143,  145,  148,  155,  162,  169,  176,
  183,  190,  197,  204,  211,  218,  225,  232,  239,  245,  252,  259,  266,  273,  280,  287,
  294,  301,  308,  315,  321,  328,  335,  342,  349,  356,  362,  366,  374,  378,  383,  390,
  393,  400,  407,  414,  421,  428,  435,  442,  449,  455,  460,  467,  473,  480,  487,  494,
  501,  508,  515,  521,  528,  535,  542,  549,  556,  563,  569,  575,  577,  583,  590
};

static const u8 Gungsuhche15_width[95] = {
 2, 5, 7, 7, 7, 7, 2, 4, 4, 6, 6, 2, 7, 2, 6, 7,
 6, 7, 7, 7, 7, 7, 7, 7, 7, 2, 3, 7, 7, 7, 7, 7,
 7, 7, 7, 7, 7, 7, 7, 7, 6, 7, 7, 7, 7, 7, 7, 7,
 7, 7, 7, 6, 7, 7, 7, 7, 7, 6, 4, 8, 4, 5, 7, 3,
 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 7, 6, 7, 7, 7, 7,
 7, 7, 6, 7, 7, 7, 7, 7, 7, 6, 6, 2, 6, 7, 6
};

Gungsuhche15::Gungsuhche15(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Gungsuhche15_glyphdata,
	Gungsuhche15_offset,
	Gungsuhche15_width,
	16,
	4,
	12,
	8
) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
