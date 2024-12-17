#ifndef DCT_H
#define DCT_H

#include <stdint.h>

// Définition de la constante pour la taille des blocs
#define BLOCK_SIZE 64
#define N 8

/**
 * @brief Calcule le coefficient de normalisation C.
 *
 * @param ksi L'indice pour lequel le coefficient est calculé
 * @return Le coefficient de normalisation
 */
double C(int ksi);

/**
 * @brief Applique la transformée en cosinus discrète (DCT) à un bloc 8x8.
 *
 * @param block Le bloc sur lequel appliquer la DCT
 * @param frequency_block Le bloc de sortie contenant les fréquences DCT
 */
void applyDCT(uint8_t* block, int16_t* frequency_block);

#endif // DCT_H