#ifndef ECRIT_MARQUEURS_H
#define ECRIT_MARQUEURS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/**
 * @brief Insère des valeurs dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier où les valeurs doivent être insérées.
 * @param values Tableau des valeurs à insérer.
 * @param values_nb Nombre de valeurs à insérer.
 */
void insert_values_in_file(FILE *fptr, uint8_t *values, size_t values_nb);

/**
 * @brief Écrit le marqueur Start of Image (SOI) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 */
void write_SOI_marker(FILE *fptr);

/**
 * @brief Écrit le marqueur End of Image (EOI) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 */
void write_EOI_marker(FILE *fptr);

/**
 * @brief Écrit la section APP0 dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 */
void write_APPO_section(FILE *fptr);

/**
 * @brief Écrit la section Define Quantization table (DQT) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param QT_index Index de la table de quantification.
 * @param precision Précision de la table (8 ou 16 bits).
 * @param QT_values_zz_format Valeurs de la table de quantification au format zig-zag.
 */
void write_DQT_section(FILE *fptr, int QT_index, int precision, uint8_t* QT_values_zz_format);

/**
 * @brief Écrit la section Début de Frame (SOFO) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param image_height Hauteur de l'image en pixels.
 * @param image_width Largeur de l'image en pixels.
 * @param components_num Nombre de composantes de l'image.
 * @param components_id_array Tableau contenant les identifiants des composantes.
 * @param sampling_factors_array Tableau contenant les facteurs d'échantillonnage.
 * @param QT_indexes Tableau contenant les index de tables de quantification.
 */
void write_SOFO_section(FILE *fptr, int image_height, int image_width, int components_num, uint8_t *components_id_array, 
                        int *sampling_factors_array, uint8_t *QT_indexes);

/**
 * @brief Écrit la section Définir Table de Huffman (DHT) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param table_type Type de la table de Huffman (DC ou AC).
 * @param table_index Index de la table de Huffman.
 * @param symb_nb_per_length Tableau contenant le nombre de symboles par longueur.
 * @param symbs_sorted_by_length Tableau contenant les symboles triés par longueur.
 * @param size_of_symbs_sorted_by_length Taille du tableau des symboles triés.
 */
void write_DHT_section(FILE *fptr, uint8_t table_type, uint8_t table_index, uint8_t *symb_nb_per_length, 
                       uint8_t *symbs_sorted_by_length, size_t size_of_symbs_sorted_by_length);

/**
 * @brief Écrit la section Start of Scan (SOS) dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param components_nb Nombre de composantes de l'image.
 * @param components_id_array Tableau contenant les identifiants des composantes.
 * @param HT_indexes_DC_array Tableau contenant les indexes des tables de Huffman DC.
 * @param HT_indexes_AC_array Tableau contenant les indexes des tables de Huffman AC.
 */
void write_SOS_section(char *fptr, int components_nb, uint8_t *components_id_array, uint8_t *HT_indexes_DC_array, 
                       uint8_t *HT_indexes_AC_array);

/**
 * @brief Écrit une section de commentaire dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param com Pointeur vers la chaîne de commentaire.
 * @param characteres_nb Nombre de caractères dans la chaîne de commentaire.
 */
void write_com_in_file(FILE *fptr, char* com, int characteres_nb);

/**
 * @brief Écrit différentes sections JPEG dans un fichier.
 * 
 * @param fptr Pointeur vers le fichier.
 * @param format Format de l'image (P5 ou P6).
 * @param width Largeur de l'image.
 * @param height Hauteur de l'image.
 * @param sample_factors Tableau contenant les facteurs d'échantillonnage.
 */
void write_sections_in_file(FILE *fptr, char *format, int width, int height, int sample_factors[6]);

#endif /* ECRIT_MARQUEURS_H */