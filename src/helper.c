#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../include/helper.h"

// Affiche une table des cosinus pour un bloc de taille BLOCK_SIZE x BLOCK_SIZE.
void cos_table() {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            printf("%.15f ", cos((2 * i + 1) * j * PI / (2 * BLOCK_SIZE)));
        }
        printf("\n");
    }
}

// Affiche les indices en ordre zig-zag pour un bloc de taille BLOCK_SIZE x BLOCK_SIZE.
void zig_zag_index(uint8_t i_or_j) {
    uint8_t up_down = 1; // Direction du mouvement
    uint8_t i = 0, j = 0;
    uint8_t index = 1;

    if (i_or_j == 0) {
        printf("%d ", i);
    } else {
        printf("%d ", j);
    }

    while (index < BLOCK_SIZE * BLOCK_SIZE) {
        if (up_down == 1) { // Montée
            if (i == 0 && j < BLOCK_SIZE - 1) {
                j++; up_down = 0;
            } else if (j == BLOCK_SIZE - 1) {
                i++; up_down = 0;
            } else {
                i--; j++;
            }
        } else { // Descente
            if (j == 0 && i < BLOCK_SIZE - 1) {
                i++; up_down = 1;
            } else if (i == BLOCK_SIZE - 1) {
                j++; up_down = 1;
            } else {
                i++; j--;
            }
        }

        if (i_or_j == 0) {
            printf("%d ", i);
        } else {
            printf("%d ", j);
        }
        index++;
    }
    printf("\n");
}

// Transforme un tableau 1D en un tableau 2D de taille BLOCK_SIZE x BLOCK_SIZE.
void transformTo2DArray(uint8_t *input, uint8_t output[BLOCK_SIZE][BLOCK_SIZE]) {
    for (uint8_t i = 0; i < BLOCK_SIZE; i++) {
        for (uint8_t j = 0; j < BLOCK_SIZE; j++) {
            output[i][j] = input[i * BLOCK_SIZE + j];
        }
    }
}

void free_2d_array(int nb_rows, void **array) {
    for (int i=0; i<nb_rows; i++) {
        free(array[i]);
    }
    free(array);
}

void free_3d_array(int depth, int nb_rows, void ***array) {
    for (int i=0; i<depth; i++) {
        for (int j=0; j<nb_rows; j++) {
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
}

/*
int main() {
    // Affiche la table des cosinus
    cos_table();
    // Affiche les indices en ordre zig-zag
    zig_zag_index(0);
    zig_zag_index(1);
    return 0;
}
*/