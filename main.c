
#include <stdint.h>
#include "vga_buffer.h"
int main(){
   
   // multidimensional array to keep track of
   Cell *VGABuffer[VGA_BUFFER_HEIGHT][VGA_BUFFER_WIDTH];

   int x = 0;
   int y = 0;



   // finally this is the shit we're coming for yehhhhhh whooo yeah baybee! thats what we're waiting for!!!! thats what its all about!!!!
   Cell* Writer = (Cell*)VGA_BUFFER_LOCATION;

   printstr("hello world!", VGABuffer, Writer, &x, &y);

   while (1) {}; // until we can call halt.

  
}
