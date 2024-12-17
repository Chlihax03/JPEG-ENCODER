#ifndef READ_PGM_H
#define READ_PGM_H

#include <stdint.h>

/**
 * @brief Lit un fichier PGM (P5) et retourne les pixels regroupés en MCU (Minimum Coded Unit)
 *
 * @param input_filename nom du fichier PGM (P5) à lire
 * @param nb_blocks pointeur vers l'entier où on stocke le nombre de blocks
 * @return tableau 2D contenant les MCU (8x8 pixels) de l'image, ou NULL en cas d'erreur
 */
uint8_t** read_from_input_filename_PGM(const char* input_filename, int *nb_blocks);

#endif /* READ_PGM_H */
