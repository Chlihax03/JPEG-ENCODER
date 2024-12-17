#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../include/zig_zag.h"

int main(void){
    // Définition d'un bloc 8x8 d'exemple
    int16_t block[LINEAR_BLOCK_SIZE]= {
        1, 2, 6, 7, 15, 16, 28, 29,
        3, 5, 8, 14, 17, 27, 30, 43,
        4, 9, 13, 18, 26, 31, 42, 44,
        10, 12, 19, 25, 32, 41, 45, 54,
        11, 20, 24, 33, 40, 46, 53, 55,
        21, 23, 34, 39, 47, 52, 56, 61,
        22, 35, 38, 48, 51, 57, 60, 62,
        36, 37, 49, 50, 58, 59, 63, 64
    };

    // Tableau pour le bloc linéarisé
    int16_t* linear_block = (int16_t*)calloc(LINEAR_BLOCK_SIZE, sizeof(int16_t));

    // Vérification de l'allocation
    if (linear_block == NULL) {
        printf("Échec de l'allocation de mémoire pour linear_block.\n");
        return EXIT_FAILURE;
    }

    // Appel de la fonction pour réorganiser le bloc en ordre zig-zag
    zzBlock(block, linear_block);

    // Affichage du bloc linéarisé
    for (uint8_t i=0; i<LINEAR_BLOCK_SIZE; i++) {
        printf("%d ", linear_block[i]);
    }

    // Libérer la mémoire allouée
    free(linear_block);

    return EXIT_SUCCESS;
}
