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
bool est_prefixe(char* mot, char* prefixe)
{
    return strncmp(mot, prefixe, strlen(prefixe)) == 0;
}

/**
 * @brief Affiche les paramètres que prend ppm2jpeg.
*/
void display_help(void)
{
    printf("ppm2jpeg prend en paramètre le nom de l'image PPM ou PGM à convertir. Il prend aussi les paramètres optionnels suivants :\n");
    printf("  * --help : pour afficher la liste des options acceptées ;\n");
    printf("  * --outfile=sortie.jpg : pour redéfinir le nom du fichier de sortie ;\n");
    printf("  * --sample=h1xv1,h2xv2,h3xv3 : pour définir les facteurs d'échantillonnage hxv des trois composantes de couleur.\n");
}


/**
 * @brief Prend la chaine de caractères chaine qui est sous la forme --outfile=sortie.jpg
 *        et retourne une chaine de caractères contenant sortie.jpg
 * 
 * @param chaine Chaine de caractères sous la forme --outfile=sortie.jpg
 * @return  Pointeur vers le nom du fichier de sortie, NULL si aucun fichier de sortie n'est spécifié
 */
char* get_outfile_name(char *chaine)
{   
    char* name = malloc(strlen(chaine)-9);

    if (name==NULL) {
        printf("Erreur d'allocation de mémoire.");
        return NULL;
    }

    if (sscanf(chaine, "--outfile=%s", name)!=1) {
        printf("Erreur d'extraction du nom de l'image de sortie. ");
        free(name);
    }

    return name;
}


/**
 * @brief Prend la chaîne de caractères chaine qui est sous la forme --sample=h1xv1,h2xv2,h3xv3
 *        extrait les facteurs h1, v1, h2, v2, h3, v3, et les stocke dans le tableau tab dans l'ordre précedant
 * 
 * @param chaine Chaîne de caractères sous la forme --sample=h1xv1,h2xv2,h3xv3
 * @param tab Pointeur vers un tableau d'entiers pour stocker les facteurs d'échantillonage
 */
void get_sample_factors(char *chaine, int* tab)
{
    sscanf(chaine, "--sample=%dx%d,%dx%d,%dx%d", tab, tab+1, tab+2, tab+3, tab+4, tab+5);
}


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
void get_parameters_from_command_line(int argc, char* argv[], bool* help, char* input_name, char* outfile_name, int* sample_factors)
{
    if (argc == 1) {
        printf("Vous n'avez pas fourni le nom de l'image à convertir.\n") ;
        return;
    
    } else {
        
        if (strcmp(argv[1], "--help") == 0) {
            // on affiche la liste des options acceptées
            *help = true;
            return;

        } else {
            // on a entré que le nom du ficher PPM
            strcpy(input_name, argv[1]);

            for (int i=2; i<argc; i++) {
                if (est_prefixe(argv[i], "--outfile=")) {
                    // on associe à fptr_jpg le nom du fichier de l'image de sortie.
                    strcpy(outfile_name, get_outfile_name(argv[i]));
                }

                if (est_prefixe(argv[i], "--sample=")) {
                    // on stocke les parametres h1, v1, h2, v2, h3, v3 dans le tableau sample.
                    get_sample_factors(argv[i], sample_factors);
                }
            }
        }
    }
}



/**
 * @brief Lit le format, la longueur et la largeur de l'image à partir d'un fichier.
 * 
 * @param file_name Nom du fichier à partir duquel lire les paramètres de l'image.
 * @param format Pointeur vers une chaîne de caractères pour stocker le format de l'image (P6 ou P5).
 * @param height Pointeur vers un entier pour stocker la longueur de l'image.
 * @param width Pointeur vers un entier pour stocker la largeur de l'image.
 */
void get_parameters_from_file(char* file_name, char* format, int* height, int* width)
{
    FILE* fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // récupération du format de l'image
    fgets(format, 3, fptr);

    // récupération des dimensions de l'image
    fscanf(fptr, "%d %d", width, height);

    fclose(fptr);
}
