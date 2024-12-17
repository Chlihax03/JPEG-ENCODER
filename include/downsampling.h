#ifndef DOWNSAMPLING_H
#define DOWNSAMPLING_H

#include <stdint.h>

/**
 * @brief Transformer chaque MCU en liste de blocks
 * @param MCU MCU
 * @param hi Hauteur de l'image
 * @param vi Largeur de l'image
 * @return MCU dÃ©coupÃ© en blocs
 */
uint8_t** MCU_to_Blocks(uint8_t* MCU, uint8_t hi, uint8_t vi);

/**
 * @brief ConcatÃ©ner les blocs des MCUs de l'image
 * @param MCUs Les MCUs de l'image
 * @param nb_MCUs Nombre de MCUs
 * @param h Hauteur de l'image
 * @param v Largeur de l'image
 * @return Blocs concatÃ©nÃ©s
 */
uint8_t** concatenate_blocks(uint8_t*** MCUs, int nb_MCUs, int h, int v);

/**
 * @brief Sous-Ã©chantillonnage
 * @param tab Composante Ã  sous-Ã©chantillonner
 * @param h Facteur d'Ã©chantillonnage horizontal
 * @param v Facteur d'Ã©chantillonnage vertical
 * @param h1 Facteur d'Ã©chantillonnage horizontal de Y
 * @param v1 Facteur d'Ã©chantillonnage vertical de Y
 * @return MCU dÃ©coupÃ© en blocs aprÃ¨s sous-Ã©chantillonnage
 */
uint8_t **downsampling(uint8_t* tab, int h, int v, int h1, int v1);

#endif //DOWNSAMPLING_H