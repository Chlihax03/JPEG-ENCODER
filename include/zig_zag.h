#ifndef ZIG_ZAG_H
#define ZIG_ZAG_H

#include <stdio.h>
#include <stdint.h>

// Définition de la constante pour la taille des blocs
#define LINEAR_BLOCK_SIZE 64
#define N 8

/**
 * @brief Réorganise un bloc en ordre zig-zag et le stocke dans un tableau linéaire de 64 éléments.
 *
 * @param block Bloc à réorganiser.
 * @param linear_block Tableau linéaire de 64 éléments pour stocker le bloc réorganisé.
 */
void zzBlock(int16_t* block, int16_t* linear_block);

#endif // ZIG_ZAG_H