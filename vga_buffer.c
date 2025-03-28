#ifndef VGA_BUFFER_C_
#define VGA_BUFFER_C_

#include "vga_buffer.h"

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


enum BlinksEnum {
   BLINKS = 128, // 10000000 in binary
   DOESNT_BLINK = 0 // 00000000 in binary
};
typedef enum BlinksEnum Blinks;

// enum of the colours supported in VGA text mode 
enum ColourEnum {
   Black,
   Blue,
   Green,
   Cyan,
   Red,
   Magenta,
   Brown,
   LightGray,
   DarkGray,
   LightBlue,
   LightGreen,
   LightCyan,
   LightRed,
   Pink,
   Yellow,
   White
};
typedef enum ColourEnum Colour;

// this is typedeffing a typedeffed type. lovely.
// useful 8 bit integer
typedef uint8_t cellColour;

typedef int Cell;

// lovely wrappers because im a recovering rustacean
cellColour getColour(Colour colour_to_get){
   return (cellColour) colour_to_get;
};

// help i deleted this comment
Cell getCell(Colour cell_background_colour, Colour cell_foreground_colour, char character, Blinks blinks){
   Cell temp_cell;
   cellColour cellbg = getColour(cell_background_colour);
   cellColour cellfg = getColour(cell_foreground_colour);
   cellColour bg_fg  = blinks | cellbg << 4 | cellfg;
   temp_cell = (Cell) bg_fg << 8 | character;
   return temp_cell;
};

void writeBuffer(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer) {
   for (int y = 0; y < VGA_BUFFER_HEIGHT; y++) {
      for (int x = y; x < y+VGA_BUFFER_WIDTH; x++) {
	 Cell* temp_pointer = Writer + x;
	 temp_pointer = VGABuffer[y][x];
      }
   }
};

void shiftBuffer(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH]){}; // todo

// this writes a cell
void writeCell(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell* Writer, int *current_pos_x, int *current_pos_y, Colour cell_background_colour, Colour cell_foreground_colour, char character, Blinks blinks){
   if (*current_pos_x == (VGA_BUFFER_WIDTH-1)) {
      shiftBuffer(VGABuffer);
      current_pos_x = 0;
      ++current_pos_y;
   }
   
   // an optimisation might be made by moving the writeBuffer call to writeString
   Cell temp_cell = getCell(cell_background_colour, cell_foreground_colour, character, blinks);
   *VGABuffer[(VGA_BUFFER_HEIGHT-1)][*current_pos_x] = temp_cell;
   writeBuffer(VGABuffer, Writer);
   ++current_pos_x;
};

void writeString(Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer, int *current_pos_x, int *current_pos_y, Colour cell_background_colour, Colour cell_foreground_colour, char string[], Blinks blinks){
   int i = 0;
   do {
      writeCell(VGABuffer, Writer, current_pos_x, current_pos_y, cell_background_colour, cell_foreground_colour, string[i], blinks);
   } while ( i < (sizeof(*string) / sizeof(string[0])));
};

extern void printstr(char msg[], Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH], Cell *Writer, int *current_pos_x, int *current_pos_y){
   writeString(VGABuffer, Writer, current_pos_x, current_pos_y, DEFAULT_VGA_BG_COLOUR, DEFAULT_VGA_FG_COLOUR, msg, DEFAULT_VGA_BLINK);
};



#endif

