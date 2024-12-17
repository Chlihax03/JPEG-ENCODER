#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../include/recupere_parametres.h"
#include "../include/RGB2YCbCr.h"
#include "../include/ecrit_marqueurs.h"
#include "../include/read_PPM.h"
#include "../include/read_PGM.h"
#include "../include/downsampling.h"
#include "../include/compressionMCUs.h"
#include "../include/creatMCUs.h"
#include "../include/helper.h"

int main(int argc, char* argv[]) {
    bool help = false;
    char *input_file_name = (char*)malloc(100*sizeof(char));
    char outfile_name[100] = "outfile.jpg"; /* nom par default de l'image de sortie, peut etre modifiÃ© par 
    l'utilisateur par passage en paramÃ¨tre dans la ligne de commande */
    int sample_factors[6] = {1, 1, 1, 1, 1, 1};

    get_parameters_from_command_line(argc, argv, &help, input_file_name, outfile_name, sample_factors);
    if (help) {
        display_help();
        return EXIT_SUCCESS;
    }
    
    char format[3];
    int height;
    int width;

    get_parameters_from_file(input_file_name, format, &height, &width);

    int nb_MCUs;
    
    if (strcmp(format, "P6") == 0) {
        /* On lit les données depuis l'image d'entrée */
        uint8_t*** MCUs = read_from_input_filename_PPM(input_file_name, sample_factors[0]*8, sample_factors[1]*8, &nb_MCUs);
        /* Création d'un tableau Y_values pour stocker les valeurs de luminance Y de chaque MCU */
        uint8_t **Y_values = (uint8_t**)malloc(nb_MCUs* sizeof(uint8_t*));
        if (Y_values == NULL) {
            perror("Erreur d'allocation mÃ©moire pour les lignes.\n");
            exit(EXIT_FAILURE);
        }
        for (int i=0; i<nb_MCUs; i++) {
            Y_values[i] = (uint8_t*)malloc(64* sample_factors[0]*sample_factors[1] * sizeof(uint8_t));
            if (Y_values[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire pour les colonnes.\n");
                for (int j=0; j<i; j++) {
                    free(Y_values[j]);
                }
                free(Y_values);
                exit(EXIT_FAILURE);
            }
        }
        

        /* Création d'un tableau Cb_values pour stocker les valeurs de chrominance Cb de chaque MCU */
        uint8_t **Cb_values = (uint8_t**)malloc(nb_MCUs * sizeof(uint8_t*));
        if (Cb_values == NULL) {
            perror("Erreur d'allocation mÃ©moire pour les lignes.\n");
            exit(EXIT_FAILURE);
        }
        for (int i=0; i<nb_MCUs; i++) {
            Cb_values[i] = (uint8_t*)malloc(64* sample_factors[0]*sample_factors[1] * sizeof(uint8_t));
            if (Cb_values[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire pour les colonnes.\n");
                for (int j=0; j<i; j++) {
                    free(Cb_values[j]);
                }
                free(Cb_values);
                exit(EXIT_FAILURE);
            }
        }

        /* Création d'un tableau Cr_values pour stocker les valeurs de chrominance Cr de chaque MCU */
        uint8_t **Cr_values = (uint8_t**)malloc(nb_MCUs * sizeof(uint8_t*));
        if (Cr_values == NULL) {
            perror("Erreur d'allocation mÃ©moire pour les lignes.\n");
            exit(EXIT_FAILURE);
        }
        for (int i=0; i<nb_MCUs; i++) {
            Cr_values[i] = (uint8_t*)malloc(64* sample_factors[0]*sample_factors[1] * sizeof(uint8_t));
            if (Cr_values[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire pour les colonnes.\n");
                for (int j=0; j<i; j++) {
                    free(Cr_values[j]);
                }
                free(Cr_values);
                exit(EXIT_FAILURE);
            }
        }     
        

        /* Conversion RGB en YCbCr */
        for (int i=0; i<nb_MCUs; i++) {
            for (int j=0; j<64*sample_factors[0]*sample_factors[1]; j++) {
                rgb_to_ycbcr(MCUs[i][j][0], MCUs[i][j][1], MCUs[i][j][2], &Y_values[i][j], &Cb_values[i][j], &Cr_values[i][j]);
            }
        }
        /* Sous-échantillonnage */

        uint8_t ***Y_sampled = (uint8_t***)malloc(nb_MCUs * sizeof(uint8_t**));
        if (Y_sampled == NULL) {
            perror("Erreur d'allocation mÃ©moire \n");
            exit(EXIT_FAILURE);
        }
        uint8_t ***Cr_sampled = (uint8_t***)malloc(nb_MCUs * sizeof(uint8_t**));
        if (Cr_sampled == NULL) {
            perror("Erreur d'allocation mÃ©moire \n");
            exit(EXIT_FAILURE);
        }
        uint8_t ***Cb_sampled = (uint8_t***)malloc(nb_MCUs * sizeof(uint8_t**));
        if (Cb_sampled == NULL) {
            perror("Erreur d'allocation mÃ©moire .\n");
            exit(EXIT_FAILURE);
        }

        for(int i=0; i<nb_MCUs; i++){
            Y_sampled[i] = (uint8_t**)malloc(64 * sizeof(uint8_t*));
            if (Y_sampled[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire \n");
                exit(EXIT_FAILURE);
            }
            Y_sampled[i] = downsampling(Y_values[i], sample_factors[0], sample_factors[1], sample_factors[0], sample_factors[1]);
            Cr_sampled[i] = (uint8_t**)malloc(64 * sizeof(uint8_t*));
            if (Cr_sampled[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire \n");
                exit(EXIT_FAILURE);
            }
            Cr_sampled[i] = downsampling(Cr_values[i], sample_factors[2], sample_factors[3], sample_factors[0], sample_factors[1]);
            Cb_sampled[i] = (uint8_t**)malloc(64 * sizeof(uint8_t*));
            if (Cb_sampled[i] == NULL) {
                perror("Erreur d'allocation mÃ©moire \n");
                exit(EXIT_FAILURE);
            }
            Cb_sampled[i] = downsampling(Cb_values[i], sample_factors[4], sample_factors[5], sample_factors[0], sample_factors[1]);
        }       

        uint8_t** Y = concatenate_blocks(Y_sampled, nb_MCUs, sample_factors[0], sample_factors[1]);
        uint8_t** Cb = concatenate_blocks(Cb_sampled, nb_MCUs, sample_factors[2], sample_factors[3]);
        uint8_t** Cr = concatenate_blocks(Cr_sampled, nb_MCUs, sample_factors[4], sample_factors[5]);

        /* Libération de la mémoire allouée pour les blocs */
        free_3d_array(nb_MCUs, 64*sample_factors[0]*sample_factors[1], (void***)MCUs);

        /* Libération de la mémoire occupée par Y_values, Cb_values et Cr_values */
        free_2d_array(nb_MCUs, Y_values);
        free_2d_array(nb_MCUs, Cb_values);
        free_2d_array(nb_MCUs, Cr_values);


        /* Début de l'écriture dans le fichier */
        FILE *fptr = fopen(outfile_name, "wb");
        if (fptr == NULL) {
            printf("Erreur d'ouverture du fichier.\n");
            return EXIT_FAILURE;
        }

        write_sections_in_file(fptr, format, width, height, sample_factors);
        bitstream_MCUs(fptr, format, Y, sample_factors[0]*sample_factors[1], Cb, sample_factors[2]*sample_factors[3], Cr, sample_factors[4]*sample_factors[5], nb_MCUs);
        
        write_EOI_marker(fptr);
        fclose(fptr);

        free_2d_array(nb_MCUs*sample_factors[0]*sample_factors[1], Y);
        free_2d_array(nb_MCUs*sample_factors[2]*sample_factors[3], Cb);
        free_2d_array(nb_MCUs*sample_factors[4]*sample_factors[5], Cr);
    }

    else if (strcmp(format, "P5") == 0) {
        int nb_blocks;
        uint8_t **Y_values;
        Y_values = read_from_input_filename_PGM(input_file_name, &nb_blocks);

    
        FILE *fptr = fopen(outfile_name, "wb");
        if (fptr == NULL) {
            printf("Erreur d'ouverture du fichier.\n");
            return EXIT_FAILURE;
        }

        write_sections_in_file(fptr, format, width, height, sample_factors);
        bitstream_MCUs(fptr, format, Y_values, 1, NULL, 0, NULL, 0, nb_blocks);
        write_EOI_marker(fptr);
        fclose(fptr);
        
    }
    return EXIT_SUCCESS;
}
