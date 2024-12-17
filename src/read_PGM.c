#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Lit un fichier PGM (P5) et retourne les pixels regroupés en MCU (Minimum Coded Unit)
 *
 * @param input_filename nom du fichier PGM (P5) à lire
 * @param nb_blocks pointeur vers l'entier où on stocke le nombre de blocks
 * @return tableau 2D contenant les MCU (8x8 pixels) de l'image, ou NULL en cas d'erreur
 */
uint8_t** read_from_input_filename_PGM(const char* input_filename, int *nb_blocks) {
    FILE* image = fopen(input_filename, "rb");
    if (image == NULL) {
        printf("Erreur de lecture du fichier %s.\n", input_filename);
        return NULL;
    }

    // Lire l'en-tête du fichier PGM
    char format_image[3];
    int largeur, hauteur, val_max;
    fscanf(image, "%2s", format_image);
    if (format_image[0] != 'P' || format_image[1] != '5') {
        printf("Format de fichier non supporté. Attendu : P5.\n");
        fclose(image);
        return NULL;
    }

    // Lire les dimensions et la valeur maximale de gris
    fscanf(image, "%d %d %d", &largeur, &hauteur, &val_max);
    fgetc(image);  // Lire le caractère de nouvelle ligne après l'entête

    // Calculer le nombre de MCU nécessaires
    int mcu_largeur = (largeur + 7) / 8;
    int mcu_hauteur = (hauteur + 7) / 8;
    int nb_mcus = mcu_largeur * mcu_hauteur;

    // Stocker la valeur du nombre de blocs
    *nb_blocks = nb_mcus;

    // Allouer de la mémoire pour les MCU
    uint8_t** mcus = malloc(nb_mcus * sizeof(uint8_t*));
    if (mcus == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        fclose(image);
        return NULL;
    }

    for (int i = 0; i < nb_mcus; i++) {
        mcus[i] = malloc(64 * sizeof(uint8_t));
        if (mcus[i] == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            for (int j = 0; j < i; j++) {
                free(mcus[j]);
            }
            free(mcus);
            fclose(image);
            return NULL;
        }
    }

    // Lire les pixels et les regrouper en MCU
    uint8_t* pixels = malloc(largeur * hauteur * sizeof(uint8_t));
    if (pixels == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        for (int i = 0; i < nb_mcus; i++) {
            free(mcus[i]);
        }
        free(mcus);
        fclose(image);
        return NULL;
    }

    fread(pixels, sizeof(uint8_t), largeur * hauteur, image);
    fclose(image);

    // Remplir les MCU en dupliquant les dernières lignes et colonnes si nécessaire
    for (int by = 0; by < mcu_hauteur; by++) {
        for (int bx = 0; bx < mcu_largeur; bx++) {
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    int pixel_y = by * 8 + y;
                    int pixel_x = bx * 8 + x;
                    if (pixel_y < hauteur && pixel_x < largeur) {
                        mcus[by * mcu_largeur + bx][y * 8 + x] = pixels[pixel_y * largeur + pixel_x];
                    } else if (pixel_y < hauteur) {
                        mcus[by * mcu_largeur + bx][y * 8 + x] = pixels[pixel_y * largeur + (largeur - 1)];
                    } else if (pixel_x < largeur) {
                        mcus[by * mcu_largeur + bx][y * 8 + x] = pixels[(hauteur - 1) * largeur + pixel_x];
                    } else {
                        mcus[by * mcu_largeur + bx][y * 8 + x] = pixels[(hauteur - 1) * largeur + (largeur - 1)];
                    }
                }
            }
        }
    }

    free(pixels);
    return mcus;
}
/*
int main() {
    char* file_name = "/Users/adilkassaoui/ENSIMAG/Projet_C/team31_old/images/invader.pgm";

    int nb_blocks;

    uint8_t **blocks;

    blocks = read_from_input_filename_PGM(file_name, &nb_blocks);

    printf("Nombre de blocks : %d\n", nb_blocks);

    for (int i=0; i<64; i++) {
        if (i%8 == 0) {
            printf("\n");
        }
        printf("%d\t", blocks[0][i]);
    }
    printf("\n");
    free(blocks[0]);
    free(blocks);

}
*/