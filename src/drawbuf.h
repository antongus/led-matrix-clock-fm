/**
*  @file drawbuf.h
*
*  Created on: 27.10.2012
*  Copyright (c) Anton Gusev aka AHTOXA
**/

#ifndef DRAWBUF_H_INCLUDED
#define DRAWBUF_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef uint32_t Coord;

template <uint32_t width, uint32_t height>
class DrawBuffer
{
public:
	static const uint32_t WIDTH = width;
	static const uint32_t HEIGHT = height;

	DrawBuffer()
		: writeOriginCol_(0)
		, writeOriginRow_(0)
		, readOriginCol_(0)
		, readOriginRow_(0)
	{}
	void setPixel(Coord col, Coord row, bool value = true);
	bool getPixel(Coord col, Coord row);
	uint8_t getByte(Coord col, Coord row);
	uint8_t getByte(Coord col, Coord row, uint8_t* masks);

	void setWriteOrigin(Coord col, Coord row);
	void getWriteOrigin(Coord* col, Coord* row);

	void setReadOrigin(Coord col, Coord row);
	void getReadOrigin(Coord* col, Coord* row);

	void putch(Coord& x, Coord& y, char ch);
	void put2ch(Coord& x, Coord& y, char ch1, char ch2, int step);
	void puts(Coord& x, Coord& y, const char* s);
	void putColumn(Coord x, Coord y, uint8_t b);

	void shiftRight();

	void clear()
	{
		memset(buf_, 0, sizeof(buf_));
	}
private:
	enum { BITS_IN_UINT32 = sizeof(uint32_t) * CHAR_BIT };
	static const uint32_t BUF_WIDTH = (width + BITS_IN_UINT32 - 1) / BITS_IN_UINT32;    // width in uint32_t words
	static const uint32_t BUF_HEIGHT = height;
	uint32_t buf_[BUF_HEIGHT][BUF_WIDTH];
	Coord writeOriginCol_;
	Coord writeOriginRow_;
	Coord readOriginCol_;
	Coord readOriginRow_;
	Coord checkCol(Coord col) { return col % WIDTH; }
	Coord checkRow(Coord row) { return row % HEIGHT; }
};

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::setPixel(Coord col, Coord row, bool value)
{
	col += writeOriginCol_;
	row += writeOriginRow_;
	if ((row < HEIGHT) && (col < WIDTH))
	{
		if (value)
			buf_[row][col / BITS_IN_UINT32] |= 1UL << (col % BITS_IN_UINT32);
		else
			buf_[row][col / BITS_IN_UINT32] &= ~(1UL << (col % BITS_IN_UINT32));
	}
}

template <uint32_t width, uint32_t height>
bool DrawBuffer<width, height>::getPixel(Coord col, Coord row)
{
	col = checkCol(col + readOriginCol_);
	row = checkRow(row + readOriginRow_);
	return buf_[row][col / BITS_IN_UINT32] & (1UL << (col % BITS_IN_UINT32));
}

template <uint32_t width, uint32_t height>
uint8_t DrawBuffer<width, height>::getByte(Coord col, Coord row)
{
	col = checkCol(col + readOriginCol_);
	row = checkRow(row + readOriginRow_);
	int mask = 1UL << (col % BITS_IN_UINT32);
	col /= BITS_IN_UINT32;
	uint32_t res = 0;
	for (int i = 0; i < 8; i++)
		if (buf_[row++][col] & mask) res |= 1 << i;

	return res;
}

/**
 * Get byte of vertical dots.
 * Bit order in byte is defined by masks array.
 */
template <uint32_t width, uint32_t height>
uint8_t DrawBuffer<width, height>::getByte(Coord col, Coord row, uint8_t* masks)
{
	col = checkCol(col + readOriginCol_);
	row = checkRow(row + readOriginRow_);
	int mask = 1UL << (col % BITS_IN_UINT32);
	col /= BITS_IN_UINT32;
	uint32_t res = 0;
	for (int i = 0; i < 8; i++)
		if (buf_[row++][col] & mask) res |= masks[i];

	return res;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::setWriteOrigin(Coord col, Coord row)
{
	writeOriginCol_ = checkCol(col);
	writeOriginRow_ = checkRow(row);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::getWriteOrigin(Coord* col, Coord* row)
{
	*col = writeOriginCol_;
	*row = writeOriginRow_;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::setReadOrigin(Coord col, Coord row)
{
	readOriginCol_ = checkCol(col);
	readOriginRow_ = checkRow(row);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::getReadOrigin(Coord* col, Coord* row)
{
	*col = readOriginCol_;
	*row = readOriginRow_;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::shiftRight()
{
	readOriginCol_ = checkCol(++readOriginCol_);
}

extern const char font7x5_ru[];

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::putColumn(Coord x, Coord y, uint8_t b)
{
	uint32_t mask = 0x01;
	for (Coord row = 0; row < 8; row++)
	{
		setPixel(x, y+row, b & mask);
		mask <<= 1;
	}
}

static int charPos(char ch)
{
	if (ch >= 'À') ch -= ('À' - 0x80);  // here is Russian letter 'A'
	ch -= 0x20;
	int pos = ch * 5;
	return pos;
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::putch(Coord& x, Coord& y, char ch)
{
	int pos = charPos(ch);
	for (ch = 5; ch; ch--)
	{
		putColumn(x++, y, font7x5_ru[pos++]);
	}
	putColumn(x++, y, 0);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::put2ch(Coord& x, Coord& y, char ch1, char ch2, int step)
{
	int pos1 = charPos(ch1);
	int pos2 = charPos(ch2);
	for (int i = 5; i; i--)
	{
		char b1 = font7x5_ru[pos1++];
		char b2 = font7x5_ru[pos2++];
		char b = (b2 >> step) | (b1 << (8-step));
		putColumn(x++, y, b);
	}
	putColumn(x++, y, 0);
}

template <uint32_t width, uint32_t height>
void DrawBuffer<width, height>::puts(Coord& x, Coord& y, const char* s)
{
	while (*s)
		putch(x, y, *s++);
}


#endif // DRAWBUF_H_INCLUDED
