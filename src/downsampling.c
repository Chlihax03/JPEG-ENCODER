#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/downsampling.h"

// Transforme chaque MCU en une liste de blocs
uint8_t** MCU_to_Blocks(uint8_t* MCU, uint8_t hi, uint8_t vi) {
    uint8_t** blocks = (uint8_t**)malloc(hi * vi * sizeof(uint8_t*));

    if (blocks == NULL) {
        // Gestion de l'Ã©chec d'allocation mÃ©moire
        return NULL;
    }

    for (int i = 0; i < hi * vi; i++) {
        blocks[i] = (uint8_t*)malloc(64 * sizeof(uint8_t));

        if (blocks[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            return NULL;
        }
    }

    int id_block = 0;
    for (int i = 0; i < vi; i++) {
        for (int j = 0; j < hi; j++) {
            int id_pixel = 0;
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    blocks[id_block][id_pixel] = MCU[8 * hi * (k + i * 8) + (j * 8 + l)];
                    id_pixel++;
                }
            }
            id_block++;
        }
    }
    return blocks;
}

// ConcatÃ¨ne les blocs des MCUs de l'image
uint8_t** concatenate_blocks(uint8_t*** MCUs, int nb_MCUs, int h, int v) {
    uint8_t** blocks = (uint8_t**)malloc(h * v * nb_MCUs * sizeof(uint8_t*));

    if (blocks == NULL) {
        // Gestion de l'Ã©chec d'allocation mÃ©moire
        return NULL;
    }

    for (int i = 0; i < h * v * nb_MCUs; i++) {
        blocks[i] = (uint8_t*)malloc(64 * sizeof(uint8_t));

        if (blocks[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            return NULL;
        }
    }

    int id_block = 0;
    for (int i = 0; i < nb_MCUs; i++) {
        for (int j = 0; j < h * v; j++) {
            memcpy(blocks[id_block], MCUs[i][j], 64 * sizeof(uint8_t)); // Copie des blocs source dans le tableau de destination
            id_block++;
        }
    }
    return blocks;
}

//ProcÃ¨de au sous-Ã©chantillonnage d'une composante
uint8_t** downsampling(uint8_t* tab, int h, int v, int h1, int v1) {
    int size = 64 * (h * v);
    uint8_t* tab_sampled = (uint8_t*)malloc(size * sizeof(uint8_t));
    if (tab_sampled == NULL) {
        printf("Erreur allocation de mÃ©moire.\n");
        return NULL;
    }
    
    for (int i = 0; i < v*8; i++) {
        for (int j = 0; j < h*8; j++) {

                    int sum = 0;

                    for (int a = 0; a < (v1 / v); a++) {
                        for (int b = 0; b < (h1 / h); b++) {
                            sum += tab[8 * h1 * (  a +  (v1 / v)* i) + b +  (h1 / h) * j];

                        }
                    }
                    tab_sampled[8*h*i+j] = sum / ((h1 / h) * (v1 / v));
                }
            }



    uint8_t** tab_sampled_final = MCU_to_Blocks(tab_sampled, h, v);
    free(tab_sampled); // LibÃ©ration de la mÃ©moire allouÃ©e pour tab_sampled
    return tab_sampled_final;
}

