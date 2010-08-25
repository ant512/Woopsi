#include "gungsuhche11.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 Gungsuhche11_glyphdata[357] = {
/* '!' */	0xFD80,
/* '"' */	0xB400,0x0000,0x0000,
/* '#' */	0x52BE,0xA52B,0xEA50,0x0000,
/* '$' */	0x2367,0x1410,0x51CD,0x8800,
/* '%' */	0x4510,0x1360,0x4510,0x0000,
/* '&' */	0x7210,0x8ACA,0x5268,0x0000,
/* ''' */	0xC000,
/* '(' */	0x2949,0x2448,0x8000,
/* ')' */	0x8912,0x494A,0x0000,
/* '*' */	0x0008,0x4F91,0x4000,0x0000,
/* '+' */	0x0008,0x4F90,0x8000,0x0000,
/* ',' */	0x0001,0x6000,
/* '-' */	0x0000,0x0F80,0x0000,0x0000,
/* '.' */	0x0180,
/* '/' */	0x0844,0x2211,0x0884,0x0000,
/* '0' */	0x7463,0x18C6,0x3170,0x0000,
/* '1' */	0xE108,0x4210,0x84F8,0x0000,
/* '2' */	0x7462,0x1111,0x19B8,0x0000,
/* '3' */	0xFC44,0xC106,0x3170,0x0000,
/* '4' */	0x118C,0xA54B,0xE238,0x0000,
/* '5' */	0xFC61,0xE886,0x3170,0x0000,
/* '6' */	0x7461,0xE8C6,0x3170,0x0000,
/* '7' */	0xECE2,0x2110,0x8420,0x0000,
/* '8' */	0x7463,0x1746,0x3170,0x0000,
/* '9' */	0x7463,0x1784,0x3170,0x0000,
/* ':' */	0x6300,
/* ';' */	0x1405,0x8000,
/* '<' */	0x0006,0xC830,0x6000,0x0000,
/* '=' */	0x0001,0xF07C,0x0000,0x0000,
/* '>' */	0x0030,0x609B,0x0000,0x0000,
/* '?' */	0x7462,0x2210,0x0420,0x0000,
/* '@' */	0x7467,0x5AD6,0xF070,0x0000,
/* 'A' */	0x6108,0x4539,0x4AD8,0x0000,
/* 'B' */	0xF252,0x9725,0x29F0,0x0000,
/* 'C' */	0x6CE3,0x0842,0x1170,0x0000,
/* 'D' */	0xF252,0x94A5,0x29F0,0x0000,
/* 'E' */	0xFA50,0xA729,0x09F8,0x0000,
/* 'F' */	0xFA50,0xA729,0x08E0,0x0000,
/* 'G' */	0x6CE3,0x0BC6,0x3368,0x0000,
/* 'H' */	0xED14,0x517D,0x1451,0xEC00,0x0000,
/* 'I' */	0xE924,0x92E0,0x0000,
/* 'J' */	0x3884,0x210A,0x52E0,0x0000,
/* 'K' */	0xEA54,0xA629,0x49E8,0x0000,
/* 'L' */	0xE210,0x8421,0x29F8,0x0000,
/* 'M' */	0xC5B6,0xD555,0x1451,0xEC00,0x0000,
/* 'N' */	0xCD96,0x5555,0x34D1,0xE400,0x0000,
/* 'O' */	0x7463,0x18C6,0x3170,0x0000,
/* 'P' */	0xF252,0x9721,0x08E0,0x0000,
/* 'Q' */	0x7463,0x18C7,0xB370,0xC000,
/* 'R' */	0xF252,0x9729,0x29E8,0x0000,
/* 'S' */	0x6CE2,0x820A,0x39B0,0x0000,
/* 'T' */	0xFD6A,0x4210,0x8470,0x0000,
/* 'U' */	0xED14,0x5145,0x1451,0x3800,0x0000,
/* 'V' */	0xED14,0x4A28,0xA104,0x1000,0x0000,
/* 'W' */	0xED15,0x5555,0x528A,0x2800,0x0000,
/* 'X' */	0xED12,0x8A10,0xA291,0xEC00,0x0000,
/* 'Y' */	0xED12,0x8A10,0x4104,0x3800,0x0000,
/* 'Z' */	0xFC64,0x2221,0x31F8,0x0000,
/* '[' */	0xEAAA,0xAC00,
/* '\' */	0xCD15,0x7555,0x528A,0x2800,0x0000,
/* ']' */	0xD555,0x5C00,
/* '^' */	0x5400,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x00F8,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x0019,0x213A,0x5278,0x0000,
/* 'b' */	0xC21C,0x94A5,0x2970,0x0000,
/* 'c' */	0x001B,0x38C2,0x1170,0x0000,
/* 'd' */	0x309D,0x294A,0x5268,0x0000,
/* 'e' */	0x001D,0x18FE,0x1170,0x0000,
/* 'f' */	0x344F,0x4444,0xE000,
/* 'g' */	0x001B,0x38C6,0x6D0F,0x8000,
/* 'h' */	0xC21C,0x94A5,0x29D8,0x0000,
/* 'i' */	0x4324,0x92E0,0x0000,
/* 'j' */	0x2192,0x4938,0x0000,
/* 'k' */	0xC216,0xA631,0x4AD8,0x0000,
/* 'l' */	0xC924,0x92E0,0x0000,
/* 'm' */	0x000A,0x9555,0x5555,0xD400,0x0000,
/* 'n' */	0x002C,0x94A5,0x29C8,0x0000,
/* 'o' */	0x001D,0x18C6,0x3170,0x0000,
/* 'p' */	0x003C,0x94A5,0x2E47,0x0000,
/* 'q' */	0x001F,0x294A,0x4E11,0xC000,
/* 'r' */	0x0036,0xD421,0x08E0,0x0000,
/* 's' */	0x001F,0x1838,0x31F0,0x0000,
/* 't' */	0x44F4,0x4445,0x3000,
/* 'u' */	0x000C,0xD145,0x1451,0x3C00,0x0000,
/* 'v' */	0x000E,0xD144,0xA284,0x1000,0x0000,
/* 'w' */	0x000E,0xD155,0x538A,0x2800,0x0000,
/* 'x' */	0x000E,0xD128,0x4291,0xEC00,0x0000,
/* 'y' */	0x000E,0xD144,0xA284,0x1180,0x0000,
/* 'z' */	0x003F,0x1111,0x11F8,0x0000,
/* '{' */	0x2925,0x1248,0x8000,
/* '|' */	0xFFE0,
/* '}' */	0x8924,0x524A,0x0000,
/* '~' */	0x0000,0x04D8,0x0000,0x0000,
/* 0x7F */	0xF999,0x9999,0xF000,
};

static const u16 Gungsuhche11_offset[95] = {
    0,    1,    4,    8,   12,   16,   20,   21,   24,   27,   31,   35,   37,   41,   42,   46,
   50,   54,   58,   62,   66,   70,   74,   78,   82,   86,   87,   89,   93,   97,  101,  105,
  109,  113,  117,  121,  125,  129,  133,  137,  142,  145,  149,  153,  157,  162,  167,  171,
  175,  179,  183,  187,  191,  196,  201,  206,  211,  216,  220,  222,  227,  229,  232,  236,
  238,  242,  246,  250,  254,  258,  261,  265,  269,  272,  275,  279,  282,  287,  291,  295,
  299,  303,  307,  311,  314,  319,  324,  329,  334,  339,  343,  346,  347,  350,  354
};

static const u8 Gungsuhche11_width[95] = {
 1, 3, 5, 5, 5, 5, 1, 3, 3, 5, 5, 2, 5, 1, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 2, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 6, 3, 5, 5, 5, 6, 6, 5, 5,
 5, 5, 5, 5, 6, 6, 6, 6, 6, 5, 2, 6, 2, 3, 5, 2,
 5, 5, 5, 5, 5, 4, 5, 5, 3, 3, 5, 3, 6, 5, 5, 5,
 5, 5, 5, 4, 6, 6, 6, 6, 6, 5, 3, 1, 3, 5, 4
};

Gungsuhche11::Gungsuhche11(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Gungsuhche11_glyphdata,
	Gungsuhche11_offset,
	Gungsuhche11_width,
	12,
	3,
	8,
	6
) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
