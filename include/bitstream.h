#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "compressionACDC.h"


// Définition de constantes
#define LINEAR_BLOCK_SIZE 64
#define MAX_AC_COEFFICIENTS 63

/**
 * @brief Écrit un nombre spécifié de bits dans le tampon.
 * 
 * @param file_ptr Pointeur vers le fichier de sortie.
 * @param buffer Tampon de bits.
 * @param buffer_bits Nombre de bits actuellement dans le tampon.
 * @param value Valeur à écrire dans le tampon.
 * @param nb_bits Nombre de bits à écrire.
 */
void write_bits(FILE *file_ptr, uint8_t *buffer, uint8_t *buffer_bits, uint16_t value, uint8_t nb_bits);

/**
 * @brief Traite un bloc de données et écrit ses bits dans le fichier.
 * 
 * @param file_ptr Pointeur vers le fichier de sortie.
 * @param linear_block Bloc de données à traiter.
 * @param DC_previous Coefficient DC précédent.
 * @param buffer Tampon de bits.
 * @param buffer_bits Nombre de bits actuellement dans le tampon.
 * @param component Composant de l'image (Y, Cb ou Cr).
 */
void bitstream_block(FILE *file_ptr, int16_t* linear_block, int16_t *DC_previous, uint8_t *buffer, uint8_t *buffer_bits, uint8_t component);

#endif // BITSTREAM_H