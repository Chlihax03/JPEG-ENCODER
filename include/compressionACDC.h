#ifndef COMPRESSIONACDC_H
#define COMPRESSIONACDC_H

#include <stdint.h>
#include <stdlib.h>
#include "../include/htables.h"

// Définition de constantes
#define MAX_AC_COEFFICIENTS 63
#define RLE_END_OF_BLOCK 0x00
#define RLE_ZRL 0xF0

// Structure pour classer les magnitudes des coefficients RLE
typedef struct {
    uint8_t magnitude;
    uint16_t index;
} magnitude_classification_type;

// Structure pour les codes RLE
typedef struct {
    uint8_t number_of_zeros;
    uint8_t magnitude;
    uint8_t rle_symbol;
    uint16_t index;
} rle_code_type;

// Structure pour les codes Huffman
typedef struct {
    uint8_t length;
    uint16_t code;
} huffman_code_type;

// Structure pour les codes bitstream
typedef struct {
    huffman_code_type huffman_code;
    uint16_t index;
    uint8_t length_index;
} bitstream_code_type;

/**
 * @brief Obtenir la classe de magnitude d'un nombre.
 *
 * @param number Le nombre en entrée
 * @return La magnitude du nombre
 */
uint8_t get_magnitude(int16_t number);

/**
 * @brief Obtenir l'index d'un nombre avec une magnitude donnée.
 *
 * @param number Le nombre en entrée
 * @param magnitude La magnitude du nombre
 * @return L'index du nombre
 */
uint16_t get_index(int16_t number, uint8_t magnitude);

/**
 * @brief Classifier la magnitude d'un nombre et obtenir son index.
 *
 * @param number Le nombre en entrée
 * @return Le résultat de la classification de magnitude
 */
magnitude_classification_type get_magnitude_classification(int16_t number);

/**
 * @brief Générer des codes RLE à partir des coefficients AC.
 *
 * @param AC_coefficients Le tableau des coefficients AC
 * @return Le tableau des codes RLE générés
 */
rle_code_type* get_rle_code(int16_t* AC_coefficients);

/**
 * @brief Obtenir le code Huffman d'un nombre.
 *
 * @param number Le nombre en entrée
 * @param DC_AC Le paramètre DC=0 et AC=1
 * @param component Le composant (Y=0, Cb=1 et Cr=2)
 * @return Le code Huffman généré
 */
huffman_code_type get_huffman_code(int16_t number, uint8_t DC_AC, uint8_t component);

/**
 * @brief Générer le code DC à partir du coefficients DC.
 *
 * @param DC_current Le coefficient DC actuel
 * @param DC_previous Le coefficient DC précédent
 * @param component Le composant (Y=0, Cb=1 et Cr=2)
 * @return Le code bitstream généré pour le DC
 */
bitstream_code_type get_DC_code(int16_t DC_current, int16_t DC_previous, uint8_t component);

/**
 * @brief Générer des codes Huffman à partir des coefficients AC.
 *
 * @param AC_coefficients Le tableau des coefficients AC
 * @param component Le composant (Y=0, Cb=1 et Cr=2)
 * @return Le tableau des codes bitstream générés pour les AC
 */
bitstream_code_type* get_AC_code(int16_t* AC_coefficients, uint8_t component);

#endif // COMPRESSIONACDC_H