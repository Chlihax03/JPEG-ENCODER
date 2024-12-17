#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


void rgb_to_ycbcr(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* Cb, uint8_t* Cr)
{
    *Y = (uint8_t)round(0.299 * (double)R + 0.587 *(double)G + 0.114 * (double)B);
    *Cb = (uint8_t)round(-0.1687 * (double)R - 0.3313 * (double)G + 0.5 * (double)B + 128.0);
    *Cr = (uint8_t)round(0.5 * (double)R - 0.4187 * (double)G - 0.0813 * (double)B + 128.0);
}

