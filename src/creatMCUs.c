#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/creatMCUs.h"


// Fonction pour créer les MCUs à partir des valeurs données
MCU_type* creatMCUs(
    char* PGM_PPM,
    uint8_t** Y_values,
    uint8_t numYbyMCU,
    uint8_t** Cb_values,
    uint8_t numCBbyMCU,
    uint8_t** Cr_values,
    uint8_t numCRbyMCU,
    uint64_t number_of_mcu
) {
    // Allocation de mémoire pour le tableau de MCUs
    MCU_type* MCUs = (MCU_type*)malloc(number_of_mcu * sizeof(MCU_type));

    // Parcourir chaque MCU
    for (uint8_t i = 0; i < number_of_mcu; i++) {
        // Allocation de mémoire pour les blocs Y de chaque MCU
        MCUs[i].Y = (uint8_t**)malloc(numYbyMCU * sizeof(uint8_t*));
        MCUs[i].len_Y = numYbyMCU;
        
        // Initialisation des blocs Y avec les valeurs correspondantes
        for (uint8_t j = 0; j < numYbyMCU; j++) {
            MCUs[i].Y[j] = &Y_values[i * numYbyMCU + j];
        }

        // Si l'image est de type PPM (indiquée par '6'), initialiser Cb et Cr
        if (PGM_PPM[1] == '6') {
            // Allocation de mémoire pour les blocs Cb de chaque MCU
            MCUs[i].Cb = (uint8_t**)malloc(numCBbyMCU * sizeof(uint8_t*));
            MCUs[i].len_Cb = numCBbyMCU;

            // Allocation de mémoire pour les blocs Cr de chaque MCU
            MCUs[i].Cr = (uint8_t**)malloc(numCRbyMCU * sizeof(uint8_t*));
            MCUs[i].len_Cr = numCRbyMCU;

            // Initialisation des blocs Cb avec les valeurs correspondantes
            for (uint8_t j = 0; j < numCBbyMCU; j++) {
                MCUs[i].Cb[j] = &Cb_values[i * numCBbyMCU + j];
            }

            // Initialisation des blocs Cr avec les valeurs correspondantes
            for (uint8_t j = 0; j < numCRbyMCU; j++) {
                MCUs[i].Cr[j] = &Cr_values[i * numCRbyMCU + j];
            }
        }
    }
    // Retourner le tableau de MCUs alloué et initialisé
    return MCUs;
}

void free_MCUs(MCU_type* MCUs, uint64_t number_of_mcu) {
    // Parcourir chaque MCU
    for (uint64_t i = 0; i < number_of_mcu; i++) {
        free(MCUs[i].Y);

        // Si l'image est de type PPM
        if (MCUs[i].Cb != NULL) {
            free(MCUs[i].Cb);
        }

        if (MCUs[i].Cr != NULL) {
            free(MCUs[i].Cr);
        }
    }
    // Libérer le tableau de MCUs
    free(MCUs);
}
