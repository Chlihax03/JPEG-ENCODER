#include <stdio.h>
#include <assert.h>
#include "../../include/compressionACDC.h"

int main() {
    // Coefficient DC de test
    int16_t DC_coefficient = 0x0018;

    // Tableau des coefficients AC de test
    int16_t AC_coefficients[MAX_AC_COEFFICIENTS] = {
        0x0000, 0xffff, 0xffbe, 0x0000, 0xffe9, 0x0000, 0xfff0,
        0x0000, 0xffce, 0x001f, 0x0000, 0xfff2, 0x0000, 0x0000, 0x0000,
        0x001a, 0x0000, 0xffed, 0x0000, 0xfff9, 0xfff8, 0x0000, 0x0010,
        0x0000, 0x000d, 0x0000, 0xfffe, 0x0000, 0xfffb, 0x0000, 0x0001,
        0x0000, 0x0002, 0x0000, 0x000c, 0x0000, 0x0000, 0x0000, 0x000b,
        0x0000, 0x000d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0003, 0x0000,
        0xfffb, 0x0000, 0xfff9, 0x0000, 0x0000, 0x0000, 0x0000, 0xfff0,
        0x0000, 0xfffe, 0x0000, 0x0008, 0x0000, 0x0000, 0xfffa, 0x0000
    };

    // Tester le codage DC
    bitstream_code_type DC_code = get_DC_code(DC_coefficient, 0, 0); // DC_previous = 0, composant = Y
    magnitude_classification_type magnitude_classification_DC = get_magnitude_classification(DC_coefficient);
    
    // Affichage des résultats pour le coefficient DC
    printf("DC:\n");
    printf("\tvalue = %d, magnitude = %d, index = %d\n", DC_coefficient, magnitude_classification_DC.magnitude, DC_code.index);
    printf("\t\tbitstream => écriture de %d sur %d bits\n", DC_code.huffman_code.code, DC_code.huffman_code.length);
    printf("\t\tbitstream => écriture de %d sur %d bits\n", magnitude_classification_DC.index, magnitude_classification_DC.magnitude);

    // Tester le codage AC
    bitstream_code_type* AC_code = get_AC_code(AC_coefficients, 0); // composant = Y
    rle_code_type* AC_rle_code = get_rle_code(AC_coefficients);

    // Affichage des résultats pour les coefficients AC
    printf("AC:\n");
    uint8_t i = 0;
    while (i < MAX_AC_COEFFICIENTS) {
        if (AC_rle_code[i].rle_symbol == RLE_END_OF_BLOCK && AC_coefficients[MAX_AC_COEFFICIENTS-1] != 0){
            break;
        }
        if (AC_rle_code[i].rle_symbol == RLE_ZRL) {
            // Cas où le symbole RLE est ZRL
            printf("\tvalue = ZRL, huffman path = %d, nb_bits = %d\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
            printf("\t\tbitstream => écriture de %d sur %d bits\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);

        } else if (AC_rle_code[i].rle_symbol == RLE_END_OF_BLOCK) {
            // Cas où le symbole RLE est end of block
            printf("\tvalue = endofblock, huffman path = %d, nb_bits = %d\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
            printf("\t\tbitstream => écriture de %d sur %d bits\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
            break;

        } else {
            // Cas général pour les coefficients AC
            printf("\tvalue = XX, magnitude = %d, index = %d\n", AC_code[i].length_index, AC_code[i].index);
            printf("\tRLE code = %d, huffman path = %d, nb_bits = %d\n", AC_rle_code[i].rle_symbol, AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
            printf("\t\tbitstream => écriture de %d sur %d bits\n", AC_code[i].huffman_code.code, AC_code[i].huffman_code.length);
            printf("\t\tbitstream => écriture de %d sur %d bits\n", AC_code[i].index, AC_code[i].length_index);
        }
        i++;
    }

    // Libérer la mémoire allouée pour les codes AC
    free(AC_code);
    free(AC_rle_code);

    return 0;
}
