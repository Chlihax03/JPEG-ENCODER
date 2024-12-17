#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H

#include <stdint.h>

// Définition de la taille du bloc linéarisé
#define LINEAR_BLOCK_SIZE 64

/**
 * @brief Effectue la quantification d'un bloc selon une table de quantification donnée.
 *
 * @param zz_block Le bloc à quantifier
 * @param qTable La table de quantification à utiliser
 */
void quantizeBlock(int16_t* zz_block, const uint8_t qTable[LINEAR_BLOCK_SIZE]);

#endif // QUANTIFICATION_H