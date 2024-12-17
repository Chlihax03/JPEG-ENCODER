#include <stdio.h>
#include <stdint.h>

#include "../include/quantification.h"

// Effectue la quantification d'un bloc selon une table de quantification donnée.
void quantizeBlock(int16_t* zz_block, const uint8_t qTable[LINEAR_BLOCK_SIZE]) {
    for (uint8_t i=0; i<LINEAR_BLOCK_SIZE; i++) {
            zz_block[i] = (int16_t)(zz_block[i]/qTable[i]);
    }
}
