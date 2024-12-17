#include <stdio.h>
#include <stdint.h>
#include "../include/zig_zag.h"

// Tableaux d'indices i et j pour l'ordre zig-zag générés par la fonction zig_zag_index dans le fichier team31/src/helper.c
const uint8_t i_index[LINEAR_BLOCK_SIZE] = {0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7};
const uint8_t j_index[LINEAR_BLOCK_SIZE] = {0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7};

// Réorganise un bloc en ordre zig-zag et le stocke dans un tableau linéaire de 64 éléments.
void zzBlock(int16_t* block, int16_t* linear_block) {
    for (int k = 0; k < LINEAR_BLOCK_SIZE; k++) {
        linear_block[k] = block[i_index[k]*N + j_index[k]];
    }
}
