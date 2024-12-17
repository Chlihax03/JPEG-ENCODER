#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Définition des constantes
#define PI 3.14159265358979323846264338327950288
#define BLOCK_SIZE 8


/**
 * @brief Affiche une table des cosinus pour un bloc de taille BLOCK_SIZE x BLOCK_SIZE.
 *
 * Cette fonction calcule et affiche les valeurs de cosinus utilisées pour la transformation
 * en cosinus discrète (DCT) d'un bloc de taille spécifiée par BLOCK_SIZE.
 */
void cos_table(void);

/**
 * @brief Affiche les indices en ordre zig-zag pour un bloc de taille BLOCK_SIZE x BLOCK_SIZE.
 *
 * @param i_or_j Indicateur pour choisir l'affichage des indices de ligne (0) ou de colonne (1).
 *
 * Cette fonction affiche les indices i et j dans l'ordre zig-zag couramment utilisé pour
 * réorganiser les coefficients de la transformation en cosinus discrète (DCT).
 */
void zig_zag_index(uint8_t i_or_j);

/**
 * @brief Transforme un tableau 1D en un tableau 2D de taille BLOCK_SIZE x BLOCK_SIZE.
 *
 * @param input Pointeur vers le tableau 1D d'entrée.
 * @param output Tableau 2D de sortie de taille [BLOCK_SIZE][BLOCK_SIZE].
 *
 * Cette fonction prend un tableau 1D de taille BLOCK_SIZE * BLOCK_SIZE et le convertit
 * en un tableau 2D de taille BLOCK_SIZE x BLOCK_SIZE.
 */
void transformTo2DArray(uint8_t *input, uint8_t output[BLOCK_SIZE][BLOCK_SIZE]);

void free_2d_array(int nb_rows, void **array);

void free_3d_array(int depth, int nb_rows, void ***array);

#endif // HELPER_H