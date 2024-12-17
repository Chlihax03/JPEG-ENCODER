#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/**
 * @param input_filename nom du fichier image d'entée
 * @param MCU_width largeur de la mcu (sample factor horizontal * 8 )
 * @param MCU_height hauteur de la mcu (sample factor vertical * 8)
 * @param number_of_mcus adresse du nombre représentant le nombre de mcus de l'image
*/
uint8_t*** read_from_input_filename_PPM(const char* input_filename, int MCU_width, int MCU_height, int *number_of_mcus) {
    FILE* image = fopen(input_filename, "rb");
    if (image == NULL) {
        printf("Erreur de lecture du fichier %s.\n", input_filename);
        return NULL;
    }

    // Lire l'en-tête du fichier PPM
    char format_image[3];
    int largeur, hauteur, max_val;
    fscanf(image, "%2s", format_image);
    if (format_image[0] != 'P' || format_image[1] != '6') {
        printf("Format de fichier non supporté. Attendu : P6.\n");
        fclose(image);
        return NULL;
    }

    // Lire les dimensions et la valeur maximale
    fscanf(image, "%d %d %d", &largeur, &hauteur, &max_val);
    fgetc(image); // Lire le caractère de nouvelle ligne après le max_val

    // Calculer le nombre de MCUs nécessaires
    int nb_MCUs_x = (largeur + MCU_width - 1) / MCU_width;
    int nb_MCUs_y = (hauteur + MCU_height - 1) / MCU_height;
    int nb_MCUs = nb_MCUs_x * nb_MCUs_y;

    *number_of_mcus = nb_MCUs;

    // Allouer de la mémoire pour les MCUs
    uint8_t*** MCUs = malloc(nb_MCUs * sizeof(uint8_t**));
    if (MCUs == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        fclose(image);
        return NULL;
    }

    for (int i = 0; i < nb_MCUs; i++) {
        MCUs[i] = malloc(MCU_height * MCU_width * sizeof(uint8_t*));
        if (MCUs[i] == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            for (int j = 0; j < i; j++) {
                free(MCUs[j]);
            }
            free(MCUs);
            fclose(image);
            return NULL;
        }

        for (int j = 0; j < MCU_height * MCU_width; j++) {
            MCUs[i][j] = malloc(3 * sizeof(uint8_t));
            if (MCUs[i][j] == NULL) {
                printf("Erreur d'allocation mémoire.\n");
                for (int k = 0; k < j; k++) {
                    free(MCUs[i][k]);
                }
                free(MCUs[i]);
                for (int l = 0; l < i; l++) {
                    for (int m = 0; m < MCU_height * MCU_width; m++) {
                        free(MCUs[l][m]);
                    }
                    free(MCUs[l]);
                }
                free(MCUs);
                fclose(image);
                return NULL;
            }
        }
    }

    // Lire les pixels de l'image
    uint8_t* pixels = malloc(largeur * hauteur * 3 * sizeof(uint8_t));
    if (pixels == NULL) {
        printf("Erreur d'allocation mémoire pour les pixels.\n");
        for (int i = 0; i < nb_MCUs; i++) {
            for (int j = 0; j < MCU_height * MCU_width; j++) {
                free(MCUs[i][j]);
            }
            free(MCUs[i]);
        }
        free(MCUs);
        fclose(image);
        return NULL;
    }
    fread(pixels, sizeof(uint8_t), largeur * hauteur * 3, image);
    fclose(image);

    // Remplir les MCUs en dupliquant les dernières lignes et colonnes si nécessaire
    for (int by = 0; by < nb_MCUs_y; by++) {
        for (int bx = 0; bx < nb_MCUs_x; bx++) {
            for (int y = 0; y < MCU_height; y++) {
                for (int x = 0; x < MCU_width; x++) {
                    int pixel_y = by * MCU_height + y;
                    int pixel_x = bx * MCU_width + x;
                    int mcu_index = by * nb_MCUs_x + bx;
                    int pixel_index = (y * MCU_width + x);

                    if (pixel_y < hauteur && pixel_x < largeur) {
                        memcpy(MCUs[mcu_index][pixel_index], &pixels[(pixel_y * largeur + pixel_x) * 3], 3);
                    } else if (pixel_y < hauteur) {
                        memcpy(MCUs[mcu_index][pixel_index], &pixels[(pixel_y * largeur + (largeur - 1)) * 3], 3);
                    } else if (pixel_x < largeur) {
                        memcpy(MCUs[mcu_index][pixel_index], &pixels[((hauteur - 1) * largeur + pixel_x) * 3], 3);
                    } else {
                        memcpy(MCUs[mcu_index][pixel_index], &pixels[((hauteur - 1) * largeur + (largeur - 1)) * 3], 3);
                    }
                }
            }
        }
    }

    free(pixels);
    return MCUs;
}

// int main() {
//     int MCU_width = 8; 
//     int MCU_height = 8; 
//     uint8_t*** MCUs = read_from_input_filename("shaun_the_sheep.ppm", MCU_width, MCU_height);

//     if (MCUs == NULL) {
//         printf("Erreur lors de la lecture du fichier PPM.\n");
//         return 1;
//     }

//     // Calculer le nombre de MCUs comme dans read_from_input_filename
//     FILE* image = fopen("output.ppm", "rb");
//     int largeur, hauteur;
//     fscanf(image, "%*s %d %d %*d", &largeur, &hauteur);
//     fclose(image);
//     int nb_MCUs_x = (largeur + MCU_width - 1) / MCU_width;
//     int nb_MCUs_y = (hauteur + MCU_height - 1) / MCU_height;
//     int nb_MCUs = nb_MCUs_x * nb_MCUs_y;

//     // Afficher les données des MCUs
//     for (int i = 0; i < nb_MCUs; i++) {
//         printf("MCU %d:\n", i);
//         for (int j = 0; j < MCU_width * MCU_height; j++) {
//             printf("[%02x %02x %02x] ", MCUs[i][j][0], MCUs[i][j][1], MCUs[i][j][2]);
//             if ((j + 1) % MCU_width == 0) printf("\n");
//         }
//         printf("\n");
//     }

//     // Libérer la mémoire allouée pour les MCUs
//     for (int i = 0; i < nb_MCUs; i++) {
//         for (int j = 0; j < MCU_width * MCU_height; j++) {
//             free(MCUs[i][j]);
//         }
//         free(MCUs[i]);
//     }
//     free(MCUs);

//     return 0;
// }