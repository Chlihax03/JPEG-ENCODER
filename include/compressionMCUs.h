#ifndef COMPRESSIONMCUS_H
#define COMPRESSIONMCUS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/dct.h"
#include "../include/zig_zag.h"
#include "../include/qtables.h"
#include "../include/quantification.h"
#include "../include/bitstream.h"

/**
 * @brief Fonction pour traiter tous les MCUs et les écrire dans le flux de bits
 *
 * @param file_ptr Pointeur vers le fichier où écrire le flux de bits
 * @param PGM_PPM Chaîne de caractères indiquant le type de fichier d'entrée ("P5" pour PGM, "P6" pour PPM)
 * @param Y_values Tableau 2D contenant les valeurs de luminance Y pour chaque MCU
 * @param numYbyMCU Nombre de blocs de luminance Y par MCU
 * @param Cb_values Tableau 2D contenant les valeurs de chrominance Cb pour chaque MCU (pour les fichiers PPM)
 * @param numCBbyMCU Nombre de blocs de chrominance Cb par MCU (pour les fichiers PPM)
 * @param Cr_values Tableau 2D contenant les valeurs de chrominance Cr pour chaque MCU (pour les fichiers PPM)
 * @param numCRbyMCU Nombre de blocs de chrominance Cr par MCU (pour les fichiers PPM)
 * @param number_of_mcu Nombre total de MCUs à traiter
 */
void bitstream_MCUs(
    FILE *file_ptr,
    char *PGM_PPM,
    uint8_t **Y_values,
    uint8_t numYbyMCU,
    uint8_t **Cb_values,
    uint8_t numCBbyMCU,
    uint8_t **Cr_values,
    uint8_t numCRbyMCU,
    uint64_t number_of_mcu
);

#endif // COMPRESSION_MCUS_H