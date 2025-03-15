
#include <stdint.h>

int main()
{
    uint8_t* text_buffer = (uint8_t*)0xB8000;
    text_buffer[0] = 'H';
    text_buffer[1] = 0x0F;
    text_buffer[2] = 'i';
    text_buffer[3] = 0x0F;
    while (1)
        ;
}
