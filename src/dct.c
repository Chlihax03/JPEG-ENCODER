#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/dct.h"

/**
 * @brief Tableau de pré-calcul des valeurs de cosinus pour éviter les recalculs redondants.
 * Ce tableau est généré par la fonction generate_cos_table du fichier team31/src/helper.c.
 */
static const double cos_table[8][8] = {
    {1.000000000000000, 0.980785280403230, 0.923879532511287, 0.831469612302545, 0.707106781186548, 0.555570233019602, 0.382683432365090, 0.195090322016128},
    {1.000000000000000, 0.831469612302545, 0.382683432365090, -0.195090322016128, -0.707106781186547, -0.980785280403230, -0.923879532511287, -0.555570233019602},
    {1.000000000000000, 0.555570233019602, -0.382683432365090, -0.980785280403230, -0.707106781186548, 0.195090322016128, 0.923879532511287, 0.831469612302545},
    {1.000000000000000, 0.195090322016128, -0.923879532511287, -0.555570233019602, 0.707106781186547, 0.831469612302545, -0.382683432365090, -0.980785280403231},
    {1.000000000000000, -0.195090322016128, -0.923879532511287, 0.555570233019602, 0.707106781186548, -0.831469612302545, -0.382683432365091, 0.980785280403230},
    {1.000000000000000, -0.555570233019602, -0.382683432365090, 0.980785280403230, -0.707106781186547, -0.195090322016128, 0.923879532511287, -0.831469612302545},
    {1.000000000000000, -0.831469612302545, 0.382683432365090, 0.195090322016129, -0.707106781186547, 0.980785280403231, -0.923879532511286, 0.555570233019602},
    {1.000000000000000, -0.980785280403230, 0.923879532511287, -0.831469612302545, 0.707106781186547, -0.555570233019602, 0.382683432365090, -0.195090322016129}
};

// Fonction pour calculer le coefficient de normalisation C
double C(int ksi) {
    if (ksi == 0)
        return (1.0 / sqrt(2.0));
    return 1.0;
}

// Fonction pour appliquer la Transformation en Cosinus Discrète (DCT) sur un bloc de 8x8
void applyDCT(uint8_t* block, int16_t* frequency_block) {
    if (block == NULL) {
        printf("pointeur nul.\n");
        exit(EXIT_FAILURE);
    }
    // Décalage des valeurs du bloc d'entrée de 128 pour centrer autour de zéro
    int8_t offset_block[64];
    for (uint8_t i = 0; i < 64; i++) {
        offset_block[i] = (int8_t)(block[i] - 128);
    }

    // Calcul du bloc de fréquences
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            double sum = 0.0;
            for (uint8_t x = 0; x < 8; x++) {
                for (uint8_t y = 0; y < 8; y++) {
                    sum += ((double)offset_block[x * 8 + y]) * cos_table[x][i] * cos_table[y][j];
                }
            }
            // Calcul de la valeur de fréquence pour (i,j)
            frequency_block[i * 8 + j] = (int16_t)(0.25 * C(i) * C(j) * sum);
        }
    }
}