#ifndef RECUPERE_PARAMETRES_H
#define RECUPERE_PARAMETRES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Vérifie si le préfixe est présent au début de la chaîne de caractères.
 * 
 * @param mot Chaîne de caractères à vérifier.
 * @param prefixe Préfixe à rechercher.
 * @return true si le préfixe est présent au début de la chaîne de caractères, sinon false.
 */
bool est_prefixe(char* mot, char* prefixe);

/**
 * @brief Affiche les paramètres que prend ppm2jpeg.
*/
void display_help(void);

/**
 * @brief Prend la chaine de caractères chaine qui est sous la forme --outfile=sortie.jpg
 *        et retourne une chaine de caractères contenant sortie.jpg
 * 
 * @param chaine Chaine de caractères sous la forme --outfile=sortie.jpg
 * @return  Pointeur vers le nom du fichier de sortie, NULL si aucun fichier de sortie n'est spécifié
 */
char* get_outfile_name(char *chaine);

/**
 * @brief Prend la chaîne de caractères chaine qui est sous la forme --sample=h1xv1,h2xv2,h3xv3
 *        extrait les facteurs h1, v1, h2, v2, h3, v3, et les stocke dans le tableau tab dans l'ordre précedant
 * 
 * @param chaine Chaîne de caractères sous la forme --sample=h1xv1,h2xv2,h3xv3
 * @param tab Pointeur vers un tableau d'entiers pour stocker les facteurs d'échantillonage
 */
void get_sample_factors(char *chaine, int* tab);

/**
 * @brief Récupère les paramètres depuis la ligne de commande.
 * 
 * @param argc Nombre d'arguments de la ligne de commande.
 * @param argv Tableau d'arguments de la ligne de commande.
 * @param help Pointeur vers un booléen, initialisé à false, qui devient true si l'option --help est spécifiée, sinon false.
 * @param input_name Chaîne de caractères pour stocker le nom de l'image d'entrée.
 * @param outfile_name Pointeur vers une chaîne de caractères pour stocker le nom du fichier de sortie.
 * @param sample_factors Pointeur vers un tableau d'entiers pour stocker les facteurs d'échantillonnage.
 */
void get_parameters_from_command_line(int argc, char* argv[], bool* help, char* input_name, char* outfile_name, int* sample_factors);

/**
 * @brief Lit le format, la longueur et la largeur de l'image à partir d'un fichier.
 * 
 * @param file_name Nom du fichier à partir duquel lire les paramètres de l'image.
 * @param format Pointeur vers une chaîne de caractères pour stocker le format de l'image (P6 ou P5).
 * @param height Pointeur vers un entier pour stocker la longueur de l'image.
 * @param width Pointeur vers un entier pour stocker la largeur de l'image.
 */
void get_parameters_from_file(char* file_name, char* format, int* height, int* width);

#endif /* RECUPERE_PARAMETRES_H */
