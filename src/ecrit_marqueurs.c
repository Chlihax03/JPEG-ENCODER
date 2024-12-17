#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/qtables.h"
#include "../include/htables.h"

void insert_values_in_file(FILE *fptr, uint8_t *values, size_t values_nb)
{
    size_t written = fwrite(values, sizeof(uint8_t), values_nb, fptr);
    if (written != values_nb) {
        printf("Erreur lors de l'Ã©criture dans le fichier.\n");
        fclose(fptr);
        return;
    }

}


void write_SOI_marker(FILE *fptr)
{
    uint8_t values[] = {0xff, 0xd8};
    insert_values_in_file(fptr, values, 2);
}

void write_EOI_marker(FILE *fptr)
{
    uint8_t values[] = {0xff, 0xd9};
    insert_values_in_file(fptr, values, 2);
}

void write_APPO_section(FILE *fptr)
{
    uint8_t values[] = {0xff, 0xe0, // markeur de la section ffe0
                        0x00, 0x10, // longueur de la section (16 octets)
                        0x4a, 0x46, 0x49, 0x46, 0x00, // 'J' 'F' 'I' 'F' '0'
                        0x01, 0x01, // Version JFIF 1.1
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }; // donnÃ©es spcifiques au JFIF, tous Ã  0 dans notre cas

    insert_values_in_file(fptr, values, sizeof(values)/sizeof(values[0]));
}

void write_DQT_section(FILE *fptr, int QT_index, int precision, uint8_t* QT_values_zz_format) {
    // on insÃ¨re le marqueur de la section ffdb
    uint8_t marker[] = {0xff, 0xdb};
    insert_values_in_file(fptr, marker, 2);

    // on insÃ¨re la longueur de la section qui vaut 67, soit 0x43, sur 2 octets
    uint8_t length[] = {0x00, 0x43};
    insert_values_in_file(fptr, length, 2);

    /* on insÃ¨re la prÃ©cision, qui vaut 0 si 8 bits et 1 si 16 bits, sur 4 bits,
    et l'indice QT_index de la table de quantification QT_index sur 4 bits. */
    uint8_t precision_QTindex[] = {(uint8_t)((precision<<4) | QT_index)};
    insert_values_in_file(fptr, precision_QTindex, 1);


    // on insÃ¨re les valeurs de la table de quantification QT, stockÃ©es au format zig-zag
    insert_values_in_file(fptr, QT_values_zz_format, 64); 
}


/** @brief
 * 
 * ecrit dans le fichier la section SOFO - Start Of Frame
 * 
 * @param fptr pointeur vers le fichier image oÃ¹ on doit Ã©crire, ouvert en mode d'ecriture en binaire
 * @param image_height hauteur en pixels de l'image
 * @param image_width largeur en pixels de l'image
 * @param components_num nombre de composants, 3 pour le YCbCr, 1 pour les niveaux de gris
 * @param components_id_array tableau contenant le(s) indentifiant(s) de chacune des composantes, entre 0 et 255
 * @param sampling_factors_array tableau contenant les facteurs d'Ã©chantillonnage, sous la forme {h1, v1} en cas des 
 * niveaux de gris, {h1, v1, h2, v2, h3, v3} en cas de YCbCr
 * @param QT_indexes tableau contenant les indices de tables de quantification associÃ©es Ã  chacune des composantes
*/
void write_SOFO_section(FILE *fptr, int image_height, int image_width, int components_num, uint8_t *components_id_array, 
int *sampling_factors_array, uint8_t *QT_indexes)
{   

    for (int i=0; i<3; i++) {
        // printf("%d\n", components_id_array[i]);
    }
    // on insÃ¨re le marqueur, sur 2 octets
    uint8_t marker[] =  {0xff, 0xc0};
    insert_values_in_file(fptr, marker, sizeof(marker));

    // on insÃ¨re la longueur de la section, sur 2 octets
    uint8_t section_length_array[] = {(uint8_t)((8 + 3 * components_num) >> 8), (uint8_t)((8 + 3 * components_num) & 0xFF)};
    insert_values_in_file(fptr, section_length_array, sizeof(section_length_array));

    // on insÃ¨re la prÃ©cision en bits par composante, toujours 8 pour le baseline, sur 1 octet
    uint8_t precision_per_component[] = {8};
    insert_values_in_file(fptr, precision_per_component, 1);

    // on insÃ¨re les dimensions de l'image, chacune sur 2 octets
    uint8_t dimensions_array[] = {(uint8_t)(image_height >> 8), (uint8_t)(image_height & 0xFF),
                                 (uint8_t)(image_width >> 8), (uint8_t)(image_width & 0xFF)};
    insert_values_in_file(fptr, dimensions_array, 4);

    // on insÃ¨re le nombre de composantes components_num, sur un octet
    uint8_t components_num_arr[] = {(uint8_t)components_num};
    insert_values_in_file(fptr, components_num_arr, 1);

    /* on insÃ¨re compenents_num fois, sur 

     * 1 octet : Identifiant de composante iC,  de 0 Ã  255
     * 4 bits : Facteur d'Ã©chantillonnage (sampling factor) horizontal, de 1 Ã  4
     * 4 bits : Facteur d'Ã©chantillonnage (sampling factor) vertical, de 1 Ã  4
     * 1 octet : indice de table de quantification iQ associÃ©e, de 0 Ã  3 */
    for (int i=0; i<components_num; i++) {
        uint8_t iC_sample_iQ_arr[] = {(uint8_t)components_id_array[i], 
                                      (uint8_t)((sampling_factors_array[2*i] << 4) | sampling_factors_array[2*i + 1]),
                                      (uint8_t)QT_indexes[i]};
        insert_values_in_file(fptr, iC_sample_iQ_arr, 3);
    }
}

void write_DHT_section(FILE *fptr, uint8_t table_type, uint8_t table_index, uint8_t *symb_nb_per_length, 
uint8_t *symbs_sorted_by_length, size_t size_of_symbs_sorted_by_length)
{
    // on insÃ¨re le marqeur ffc4
    uint8_t marker[] = {0xff, 0xc4};
    insert_values_in_file(fptr, marker, 2);

    // on insÃ¨re la longueur de la section, sur 2 octets
    uint8_t length = 19 + size_of_symbs_sorted_by_length/sizeof(uint8_t);
    // printf("Longueur de la section : %d\n", length);
    uint8_t section_length[] = {(uint8_t)(length>>8), (uint8_t)(length & 0xFF)};
    insert_values_in_file(fptr, section_length, 2);

    /* On insÃ¨re, sur un octet, des informations sur la table de Huffman :
      ** 3 bits non utilisÃ©s ( Ã  0)
      ** 1 bits pour le type de table ( 0: DC, 1: AC)
      ** indice (0..3, ou erreur) 
    */
    uint8_t byte_to_insert[] = { (table_type << 4) | table_index };
    insert_values_in_file(fptr, byte_to_insert, 1);

    // on insÃ¨re le nombre de symboles pour chaque longueur entre 1 et 16
    insert_values_in_file(fptr, symb_nb_per_length, 16);

    // on insÃ¨re les symboles, triÃ©s par longueur
    insert_values_in_file(fptr, symbs_sorted_by_length, size_of_symbs_sorted_by_length/sizeof(uint8_t));
}


void write_SOS_section(char *fptr, int components_nb, uint8_t *components_id_array, uint8_t *HT_indexes_DC_array, 
                        uint8_t *HT_indexes_AC_array)
{
    // on insÃ¨re le markeur sur 2 octets
    uint8_t marker[] = {0xff, 0xda};
    insert_values_in_file(fptr, marker, 2);

    // on insÃ¨re la longueur de la section sur 2 octets
    uint16_t section_length = 2 * components_nb + 6;
    uint8_t section_length_arr[] = {(uint8_t)(section_length >> 8), (uint8_t)(section_length & 0xFF)};
    insert_values_in_file(fptr, section_length_arr, 2);

    // On insÃ¨re le nombre de composantes, sur un octet
    uint8_t components_nb_arr[] = {components_nb};
    insert_values_in_file(fptr, components_nb_arr, 1);

    /* On insÃ¨re components_nb fois, sur :

      ** 1 octet : identifiant iC de la composante
      ** 4 bits : indice de la table de Huffman (iH) pour les coefficients DC
      ** 4 bits : indice de la table de Huffman (iH) pour les coefficients AC */
    uint8_t values[2];
    for (int i=0; i<components_nb; i++) {
        values[0] = components_id_array[i];
        values[1] = (uint8_t)((HT_indexes_DC_array[i] << 4) | HT_indexes_AC_array[i]);
        insert_values_in_file(fptr, values, 2);
    }

    /* on insÃ¨re le premier indice de la sÃ©lection spectrale : 0 en mode baseline,
    le dernier indice de la sÃ©lection spectrale : 63 en mode baseline,
    et l'approximation successive :
        ** Ah : 4 bits, poids fort :  0 en mode baseline
        ** Al : 4 bits, poids faible :  0 en mode baseline  */

    uint8_t Ss_Se[] = {0, 63, 0};
    insert_values_in_file(fptr, Ss_Se, 3);
}

void write_com_in_file(FILE *fptr, char* com, int characteres_nb) {
    // on insÃ¨re le marqeur fffe
    uint8_t marker[] = {0xff, 0xfe};
    insert_values_in_file(fptr, marker, 2);

    insert_values_in_file(fptr, (uint8_t*)com, characteres_nb * sizeof(char));
}


// fonction qui Ecrit les sections SOI, APPO, DQT, SOFO, DHT, et SOS dans le fichier dont l'adresse est passÃ© en paramÃ¨tre
void write_sections_in_file(FILE *fptr, char *format, int width, int height, int sample_factors[6]) {
    write_SOI_marker(fptr);

    write_APPO_section(fptr);
    
    write_DQT_section(fptr, 0, 0, quantification_table_Y);

    
    
    if (strcmp(format, "P6") == 0) {
        write_DQT_section(fptr, 1, 0, quantification_table_CbCr);
        int components_num = 3;
        uint8_t components_indexes[] = {1, 2, 3};
        uint8_t QT_indexes[] = {0, 1, 1};
        write_SOFO_section(fptr, height, width, components_num, components_indexes, sample_factors, QT_indexes);

        for (int j=0; j<2; j++) {
            for (int i=0; i<2; i++) {
                write_DHT_section(fptr, i, j, htables_nb_symb_per_lengths[i][j], htables_symbols[i][j], htables_nb_symbols[i][j]*sizeof(uint8_t));
            }
        }


        uint8_t htable_DC_AC_indexes[] = {0, 1, 1};
        write_SOS_section(fptr, components_num, components_indexes, htable_DC_AC_indexes, htable_DC_AC_indexes);
    
    }

    if (strcmp(format, "P5") == 0) {
        uint8_t components_indexes[3] = {1, 2, 3};
        uint8_t QT_indexes[3];
        int components_num = 1;
        QT_indexes[0] = 0;
        write_SOFO_section(fptr, height, width, components_num, components_indexes, sample_factors, QT_indexes);

        for (int j=0; j<2; j++) {
            for (int i=0; i<2; i++) {
                write_DHT_section(fptr, i, j, htables_nb_symb_per_lengths[i][j], htables_symbols[i][j], htables_nb_symbols[i][j]*sizeof(uint8_t));
            }
        }
        int htable_DC_AC_indexes[] = {0};
        write_SOS_section(fptr, components_num, components_indexes, htable_DC_AC_indexes, htable_DC_AC_indexes);
    }
}