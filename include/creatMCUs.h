#ifndef CREATMCUS_H
#define CREATMCUS_H

#include <stdint.h>
#include "../include/compressionMCUs.h"

// Structure représentant une MCU avec les composants Y, Cb et Cr.
typedef struct {
    uint8_t** Y;
    uint8_t len_Y;
    uint8_t** Cb;
    uint8_t len_Cb;
    uint8_t** Cr;
    uint8_t len_Cr;
} MCU_type;

/**
 * @brief Crée un tableau de MCUs à partir des valeurs données.
 *
 * @param PGM_PPM Indicateur du type d'image (PGM ou PPM)
 * @param Y_values Tableau des valeurs Y
 * @param numYbyMCU Nombre de blocs Y par MCU
 * @param Cb_values Tableau des valeurs Cb
 * @param numCBbyMCU Nombre de blocs Cb par MCU
 * @param Cr_values Tableau des valeurs Cr
 * @param numCRbyMCU Nombre de blocs Cr par MCU
 * @param number_of_mcu Nombre total de MCUs
 * @return MCU_type* Pointeur vers le tableau de MCUs créé
 */
MCU_type* creatMCUs(
    char* PGM_PPM, 
    uint8_t** Y_values, 
    uint8_t numYbyMCU, 
    uint8_t** Cb_values, 
    uint8_t numCBbyMCU, 
    uint8_t** Cr_values, 
    uint8_t numCRbyMCU, 
    uint64_t number_of_mcu
);

/**
 * @brief Libère la mémoire allouée pour un tableau de MCUs.
 *
 * @param MCUs Pointeur vers le tableau de MCUs
 * @param number_of_mcu Nombre total de MCUs
 */
void free_MCUs(MCU_type* MCUs, uint64_t number_of_mcu);

#endif // CREATMCUS_H