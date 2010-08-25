#include "timesnewroman9.h"
#include <nds.h>

using namespace WoopsiUI;

static const u16 TimesNewRoman9_glyphdata[335] = {
/* '!' */	0xFA00,
/* '"' */	0xB680,0x0000,0x0000,
/* '#' */	0x4A7E,0xAFCA,0x4000,0x0000,
/* '$' */	0x6DC6,0x5DE4,0x0000,
/* '%' */	0x44A4,0x4810,0x2225,0x4200,0x0000,0x0000,
/* '&' */	0x3146,0xD2B2,0x5E80,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x5AA9,0x4000,
/* ')' */	0xA556,0x8000,
/* '*' */	0x5FA0,0x0000,0x0000,
/* '+' */	0x0109,0xF210,0x0000,0x0000,
/* ',' */	0x000D,0x0000,
/* '-' */	0x00C0,0x0000,
/* '.' */	0x0200,
/* '/' */	0x2525,0x2000,0x0000,
/* '0' */	0x6999,0x9960,0x0000,
/* '1' */	0xC924,0xB800,0x0000,
/* '2' */	0x6911,0x25F0,0x0000,
/* '3' */	0xE126,0x11E0,0x0000,
/* '4' */	0x266A,0xF220,0x0000,
/* '5' */	0x3461,0x11E0,0x0000,
/* '6' */	0x34E9,0x9960,0x0000,
/* '7' */	0x7922,0x2440,0x0000,
/* '8' */	0x6996,0x9960,0x0000,
/* '9' */	0x6999,0x72C0,0x0000,
/* ':' */	0x2200,
/* ';' */	0x040D,0x0000,
/* '<' */	0x0168,0x6100,0x0000,
/* '=' */	0x00F0,0xF000,0x0000,
/* '>' */	0x0861,0x6800,0x0000,
/* '?' */	0x54A4,0x1000,0x0000,
/* '@' */	0x3E41,0x9FA3,0xA5A5,0x9B41,0x3E00,0x0000,
/* 'A' */	0x1020,0xA143,0x88B9,0x8000,0x0000,0x0000,
/* 'B' */	0xF252,0xE4A7,0xC000,0x0000,
/* 'C' */	0x7988,0x8960,0x0000,
/* 'D' */	0xF124,0x5145,0x2F00,0x0000,0x0000,
/* 'E' */	0xFD15,0x1C51,0x1FC0,0x0000,0x0000,
/* 'F' */	0xFA54,0xE523,0x8000,0x0000,
/* 'G' */	0x6A68,0x278A,0x2700,0x0000,0x0000,
/* 'H' */	0xEE89,0x13E4,0x48BB,0x8000,0x0000,0x0000,
/* 'I' */	0xE924,0xB800,0x0000,
/* 'J' */	0xE924,0xA000,0x0000,
/* 'K' */	0xEC91,0x4384,0x88BB,0x8000,0x0000,0x0000,
/* 'L' */	0xE210,0x8427,0xE000,0x0000,
/* 'M' */	0xE366,0x565A,0x4A4A,0xEB00,0x0000,0x0000,
/* 'N' */	0xCE89,0x92A4,0xC8B9,0x0000,0x0000,0x0000,
/* 'O' */	0x7463,0x18C5,0xC000,0x0000,
/* 'P' */	0xF252,0x9723,0x8000,0x0000,
/* 'Q' */	0x7463,0x18C5,0xC608,0x0000,
/* 'R' */	0xF124,0x9C51,0x2EC0,0x0000,0x0000,
/* 'S' */	0x7986,0x19E0,0x0000,
/* 'T' */	0xFD48,0x4211,0xC000,0x0000,
/* 'U' */	0xEE89,0x1224,0x488E,0x0000,0x0000,0x0000,
/* 'V' */	0xEE89,0x1142,0x8504,0x0000,0x0000,0x0000,
/* 'W' */	0xDBA4,0x924B,0x2551,0x1088,0x0000,0x0000,0x0000,
/* 'X' */	0xED24,0x8C31,0x2EC0,0x0000,0x0000,
/* 'Y' */	0xEE88,0xA141,0x020E,0x0000,0x0000,0x0000,
/* 'Z' */	0x7A84,0x4427,0xE000,0x0000,
/* '[' */	0xEAAA,0xC000,
/* '\' */	0x8444,0x2210,0x0000,
/* ']' */	0xD555,0xC000,
/* '^' */	0x6990,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x00F8,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x00C6,0xAA70,0x0000,
/* 'b' */	0x88E9,0x9960,0x0000,
/* 'c' */	0x01C9,0x1800,0x0000,
/* 'd' */	0x309D,0x2949,0xE000,0x0000,
/* 'e' */	0x015F,0x1800,0x0000,
/* 'f' */	0x7349,0x3000,0x0000,
/* 'g' */	0x001F,0x2643,0xD170,0x0000,
/* 'h' */	0x8429,0xA94B,0x6000,0x0000,
/* 'i' */	0x4324,0xB800,0x0000,
/* 'j' */	0x4D55,0x8000,
/* 'k' */	0x889E,0xAAD0,0x0000,
/* 'l' */	0xC924,0xB800,0x0000,
/* 'm' */	0x000F,0x2AAA,0xAFC0,0x0000,0x0000,
/* 'n' */	0x0029,0xA94B,0x6000,0x0000,
/* 'o' */	0x0069,0x9960,0x0000,
/* 'p' */	0x00E9,0x99E8,0xC000,
/* 'q' */	0x001D,0x2949,0xC238,0x0000,
/* 'r' */	0x03C9,0x3000,0x0000,
/* 's' */	0x01CC,0x7000,0x0000,
/* 't' */	0x1349,0x3800,0x0000,
/* 'u' */	0x002D,0x2949,0xE000,0x0000,
/* 'v' */	0x00FA,0xAA40,0x0000,
/* 'w' */	0x0003,0xDCAB,0x4B12,0x0000,0x0000,0x0000,
/* 'x' */	0x00BA,0x4AB0,0x0000,
/* 'y' */	0x002F,0x2510,0x88C0,0x0000,
/* 'z' */	0x00FA,0x25F0,0x0000,
/* '{' */	0x2928,0x9220,0x0000,
/* '|' */	0xFF00,
/* '}' */	0x8922,0x9280,0x0000,
/* '~' */	0x0000,0xDB00,0x0000,
/* 0x7F */	0x0F99,0x99F0,0x0000,
};

static const u16 TimesNewRoman9_offset[95] = {
    0,    1,    4,    8,   11,   17,   22,   23,   25,   27,   30,   34,   36,   38,   39,   42,
   45,   48,   51,   54,   57,   60,   63,   66,   69,   72,   73,   75,   78,   81,   84,   87,
   93,   99,  103,  106,  111,  116,  120,  125,  131,  134,  137,  143,  147,  153,  159,  163,
  167,  171,  176,  179,  183,  189,  195,  202,  207,  213,  217,  219,  222,  224,  227,  231,
  233,  236,  239,  242,  246,  249,  252,  256,  260,  263,  265,  268,  271,  276,  280,  283,
  286,  290,  293,  296,  299,  303,  306,  312,  315,  319,  322,  325,  326,  329,  332
};

static const u8 TimesNewRoman9_width[95] = {
 1, 3, 5, 4, 8, 6, 1, 2, 2, 3, 5, 2, 2, 1, 3, 4,
 3, 4, 4, 4, 4, 4, 4, 4, 4, 1, 2, 4, 4, 4, 3, 8,
 7, 5, 4, 6, 6, 5, 6, 7, 3, 3, 7, 5, 8, 7, 5, 5,
 5, 6, 4, 5, 7, 7, 9, 6, 7, 5, 2, 4, 2, 4, 5, 2,
 4, 4, 3, 5, 3, 3, 5, 5, 3, 2, 4, 3, 6, 5, 4, 4,
 5, 3, 3, 3, 5, 4, 7, 4, 5, 4, 3, 1, 3, 4, 4
};

TimesNewRoman9::TimesNewRoman9(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	TimesNewRoman9_glyphdata,
	TimesNewRoman9_offset,
	TimesNewRoman9_width,
	12,
	3,
	6,
	9
) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
