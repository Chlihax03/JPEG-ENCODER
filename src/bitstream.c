#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bitstream.h"

// Fonction pour écrire un nombre spécifié de bits dans le tampon
void write_bits(FILE *file_ptr, uint8_t *buffer, uint8_t *buffer_bits, uint16_t value, uint8_t nb_bits) {
    // Boucle sur chaque bit à écrire
    for (uint8_t i = 0; i < nb_bits; i++) {
        // Décalage et ajout du bit courant au buffer
        *buffer = (*buffer << 1) | ((value >> (nb_bits - i - 1)) & 1);
        (*buffer_bits)++;
        
        // Si le buffer est plein, écrire dans le fichier
        if (*buffer_bits == 8) {
            fwrite(buffer, 1, 1, file_ptr);
            // Byte stuffing : ajouter un zéro après un 0xff
            if (*buffer == 0xff) {
                uint8_t zero = 0x00;
                fwrite(&zero, 1, 1, file_ptr);
            }
            // Réinitialiser le buffer et le compteur de bits
            *buffer = 0;
            *buffer_bits = 0;
        }
    }
}

// Fonction pour traiter un bloc et écrire ses bits dans le fichier
void bitstream_block(FILE *file_ptr, int16_t* linear_block, int16_t* DC_previous, uint8_t *buffer, uint8_t *buffer_bits, uint8_t component) {
    // Déterminer les coefficients DC et AC
    int16_t DC = linear_block[0];
    int16_t* AC_coefficients = (int16_t*)calloc(MAX_AC_COEFFICIENTS, sizeof(int16_t));
    for (uint8_t i = 0; i < MAX_AC_COEFFICIENTS; i++) {
        AC_coefficients[i] = linear_block[i + 1];
    }

    // Obtenir les codes ACDC
    bitstream_code_type DC_code = get_DC_code(DC, *DC_previous, component);
    bitstream_code_type* AC_code = get_AC_code(AC_coefficients, component);

    // Changement DC_previous
    *DC_previous = DC;

    // Écrire le code DC dans le fichier
    printf("\n\n  Coefficients DC : \n\n");
    write_bits(file_ptr, buffer, buffer_bits, DC_code.huffman_code.code, DC_code.huffman_code.length);
    printf("Writing %d over %d\n", DC_code.huffman_code.code, DC_code.huffman_code.length);
    write_bits(file_ptr, buffer, buffer_bits, DC_code.index, DC_code.length_index);
    printf("Writing %d over %d\n", DC_code.index, DC_code.length_index);

    // Déterminer les codes RLE spéciaux
    uint16_t rle_zrl = get_huffman_code(RLE_ZRL, 1, component).code;
    uint16_t rle_end_of_block = get_huffman_code(RLE_END_OF_BLOCK, 1, component).code;

    // Écrire les codes des AC dans le fichier
    printf("\n\n Coefficients AC : \n\n");
    for (uint8_t i = 0; i < MAX_AC_COEFFICIENTS; i++) {
        if (AC_code[i].huffman_code.code == rle_end_of_block && AC_coefficients[MAX_AC_COEFFICIENTS-1] != 0) {
            break;
        }
        // Écrire le code Huffman du coefficient AC[i]
        write_bits(file_ptr, buffer, buffer_bits, AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
        printf("Writing %d over %d\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);

        if (AC_code[i].huffman_code.code == rle_end_of_block) {
            break;
        }
        if (AC_code[i].huffman_code.code == rle_zrl) {
            continue;
        }
        write_bits(file_ptr, buffer, buffer_bits, AC_code[i].index, AC_code[i].length_index);
        printf("Writing %d over %d\n", AC_code[i].index, AC_code[i].length_index);
    }

    // Libérer la mémoire allouée
    free(AC_code);
    free(AC_coefficients);
}
