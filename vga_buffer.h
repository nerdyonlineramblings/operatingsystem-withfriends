#ifndef VGA_BUFFER_H_
#define VGA_BUFFER_H_

#ifndef STDINT_H_
#define STDINT_H_
#include <stdint.h>
#endif

#define VGA_BUFFER_HEIGHT 25
#define VGA_BUFFER_WIDTH  80
#define VGA_BUFFER_LOCATION 0xB8000

#define DEFAULT_VGA_BG_COLOUR Black
#define DEFAULT_VGA_FG_COLOUR LightCyan
#define DEFAULT_VGA_BLINK DOESNT_BLINK

/*
 * VGA BUFFER STRUCTURE:
 * 2 BYTES LONG
 * 2ND BYTE
 * 7	      |	6	5	4	| 3	2	1	0
 * Blink      | BG_COLOUR     		| FG_COLOUR
 * 
 * 2ND CHAR IS THE CHARACTER
 * */


enum BlinksEnum;
typedef enum BlinksEnum Blinks;

// enum of the colours supported in VGA text mode 
enum ColourEnum;
typedef enum ColourEnum Colour;

// this is typedeffing a typedeffed type. lovely.
// useful 8 bit integer
typedef uint8_t cellColour;

typedef int Cell;

// lovely wrappers because im a recovering rustacean
cellColour getColour(Colour colour_to_get);

// help i deleted this comment
Cell getCell(Colour cell_background_colour, Colour cell_foreground_colour, char character, Blinks blinks);

void writeBuffer(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer);

void shiftBuffer(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH]); // todo

// this writes a cell
void writeCell(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell* Writer, int *current_pos_x, int *current_pos_y, Colour cell_background_colour, Colour cell_foreground_colour, char character, Blinks blinks);

void writeString(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer, int *current_pos_x, int *current_pos_y, Colour cell_background_colour, Colour cell_foreground_colour, char string[], Blinks blinks);

extern void printstr(char msg[], Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer, int *current_pos_x, int *current_pos_y);

#endif
