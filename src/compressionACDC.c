#include <stdio.h>
#include <stdlib.h>
#include "../include/compressionACDC.h"

// Fonction pour obtenir la classe de magnitude d'un nombre
uint8_t get_magnitude(int16_t number){
    // Par symétrie, on considère uniquement les nombres positifs.
    if (number < 0){
        number *= -1;
    }
    uint8_t magnitude = 11;
    // Trouver la classe de magnitude
    if (number == 0) magnitude = 0;
    else if (number == 1) magnitude = 1;
    else if (number <= 3) magnitude = 2;
    else if (number <= 7) magnitude = 3;
    else if (number <= 15) magnitude = 4;
    else if (number <= 31) magnitude = 5;
    else if (number <= 63) magnitude = 6;
    else if (number <= 127) magnitude = 7;
    else if (number <= 255) magnitude = 8;
    else if (number <= 511) magnitude = 9;
    else if (number <= 1023) magnitude = 10;
    else if (number <= 2047) magnitude = 11;

    return magnitude;
}

// Fonction pour obtenir l'index d'un nombre avec une magnitude donnée
uint16_t get_index(int16_t number, uint8_t magnitude){
    uint16_t index;
    if (number < 0){
        index = number + (1 << magnitude) - 1;
    } else {
        index = number; 
    }
    return index;
}

// Fonction pour classifier la magnitude d'un nombre et obtenir son index
magnitude_classification_type get_magnitude_classification(int16_t number){
    magnitude_classification_type magnitude_index;
    magnitude_index.magnitude = get_magnitude(number);
    magnitude_index.index = get_index(number, magnitude_index.magnitude);
    return magnitude_index;
}

// Fonction pour générer des codes RLE à partir des coefficients AC
rle_code_type* get_rle_code(int16_t* AC_coefficients){
    // Allouer de la mémoire pour un tableau temporaire pour stocker les codes RLE
    rle_code_type* rle_code_table = calloc(63, sizeof(rle_code_type));
    uint8_t rle_code_table_size = 0;
    rle_code_type rle_code = {0, 0, 0, 0};

    // Itérer à travers les coefficients AC
    for (uint8_t i=0; i<MAX_AC_COEFFICIENTS; i++){
        if (AC_coefficients[i] == 0){
            rle_code.number_of_zeros ++;
            if (rle_code.number_of_zeros == 16){
                rle_code.rle_symbol = RLE_ZRL;
                rle_code_table[rle_code_table_size] = rle_code;
                rle_code_table_size ++;
                rle_code.number_of_zeros = 0; 
            }
        } else {
            // Obtenir la classification de la magnitude pour le coefficient non nul
            magnitude_classification_type magnitude_classification = get_magnitude_classification(AC_coefficients[i]);
            rle_code.magnitude = magnitude_classification.magnitude;
            rle_code.rle_symbol = (rle_code.number_of_zeros << 4) | rle_code.magnitude;
            rle_code.index = magnitude_classification.index;
            rle_code_table[rle_code_table_size] = rle_code;
            rle_code_table_size ++;
            rle_code.number_of_zeros = 0;
        }
    }

    // S'il reste des zéros à la fin
    if (rle_code.number_of_zeros > 0){ 
        rle_code.rle_symbol = RLE_ZRL;
        rle_code_table[rle_code_table_size] = rle_code;
        rle_code_table_size ++;
    }

    // Éliminer les zéros
    uint8_t i = rle_code_table_size-1;
    while(i>=0 && rle_code_table[i].rle_symbol == RLE_ZRL){
        rle_code_table[i].rle_symbol = RLE_END_OF_BLOCK;
        i --;
    }

    return rle_code_table;
}

// Fonction pour obtenir le code Huffman d'un nombre
huffman_code_type get_huffman_code(int16_t number, uint8_t DC_AC, uint8_t component){
    huffman_code_type huffman_code;

    // Obtenir l'index dans la table htables_symbols[DC_AC][component]
    uint8_t index_in_htable = 0;
    while (number != htables_symbols[DC_AC][component][index_in_htable]) {
        index_in_htable++;
    }

    // Obtenir la longueur du code Huffman
    uint8_t sum = 0;
    uint8_t j = 0;
    while (sum <= index_in_htable) {
        sum += htables_nb_symb_per_lengths[DC_AC][component][j];
        j++;
    }
    huffman_code.length = j;

    // Obtenir le code Huffman
    uint8_t sum_index = 0;
    huffman_code.code = 0;
    for (int8_t k = 0; k < (huffman_code.length - 1); k++) {
        sum_index += htables_nb_symb_per_lengths[DC_AC][component][k];
        huffman_code.code += htables_nb_symb_per_lengths[DC_AC][component][k] * (1 << (huffman_code.length - (k + 1)));
    }
    huffman_code.code += index_in_htable - sum_index;

    return huffman_code;
}

// Fonction pour générer le code DC à partir des coefficients DC
bitstream_code_type get_DC_code(int16_t DC_current, int16_t DC_previous, uint8_t component){
    int16_t DCPM = DC_current - DC_previous;
    bitstream_code_type DC_code;
    magnitude_classification_type DCPM_magnitude_classification = get_magnitude_classification(DCPM);
    DC_code.huffman_code = get_huffman_code(DCPM_magnitude_classification.magnitude, 0, component);
    DC_code.index = DCPM_magnitude_classification.index;
    DC_code.length_index = DCPM_magnitude_classification.magnitude;
    return DC_code;
}

// Fonction pour générer des codes Huffman à partir des coefficients AC
bitstream_code_type* get_AC_code(int16_t* AC_coefficients, uint8_t component){
    // Obtenir les codes RLE à partir des coefficients AC
    rle_code_type* rle_code_table = get_rle_code(AC_coefficients);

    // Allouer de la mémoire pour la table des codes Huffman
    bitstream_code_type* AC_code_table = calloc(63, sizeof(bitstream_code_type));

    uint8_t i = 0;
    // Boucler à travers la table des codes RLE
    while (i<MAX_AC_COEFFICIENTS){
        AC_code_table[i].huffman_code = get_huffman_code(rle_code_table[i].rle_symbol, 1, component);
        if (rle_code_table[i].rle_symbol == RLE_ZRL){;
            AC_code_table[i].length_index = 0;
        } else if (rle_code_table[i].rle_symbol == RLE_END_OF_BLOCK){
            AC_code_table[i].length_index = 0;
            break;
        } else {
            AC_code_table[i].index = rle_code_table[i].index;
            AC_code_table[i].length_index = rle_code_table[i].magnitude;
        }
        i ++;
    }

    // Libérer la mémoire allouée pour rle_code_table
    free(rle_code_table);

    return AC_code_table;
}
