#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/compressionMCUs.h"
#include "../include/creatMCUs.h"

// Fonction pour traiter Tous les MCU et l'écrire dans le flux de bits
void bitstream_MCUs(
    FILE *file_ptr, 
    char* PGM_PPM,
    uint8_t** Y_values,
    uint8_t numYbyMCU,
    uint8_t** Cb_values,
    uint8_t numCBbyMCU,
    uint8_t** Cr_values,
    uint8_t numCRbyMCU,
    uint64_t number_of_mcu
) { 
    // Initialisation des variables de buffer de bits
    uint8_t buffer = 0;
    uint8_t buffer_bits = 0;
    int16_t DC_previousY = 0;
    int16_t DC_previousCb = 0;
    int16_t DC_previousCr = 0;

    // Déclarer les variables pour le bloc de fréquences et le bloc zig-zag
    int16_t* frequency_block = (int16_t*)calloc(LINEAR_BLOCK_SIZE, sizeof(int16_t));
    if (frequency_block == NULL) {
        printf("Erreur d'allocation memoire.\n");
        exit(EXIT_FAILURE);
    }
    int16_t* zz_block = (int16_t*)calloc(LINEAR_BLOCK_SIZE, sizeof(int16_t));
    if (frequency_block == NULL) {
        printf("Erreur d'allocation memoire.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < number_of_mcu; i++) {
        for (int j = 0; j<numYbyMCU; j++){          
            applyDCT(Y_values[i*numYbyMCU + j], frequency_block);                      // Appliquer la DCT au bloc de composant Y           
            zzBlock(frequency_block, zz_block);                                        // Effectuer l'ordre en zig-zag des coefficients DCT
            quantizeBlock(zz_block, quantification_table_Y);                           // Quantifier les coefficients en utilisant la table de quantification Y
            bitstream_block(file_ptr, zz_block, &DC_previousY, &buffer, &buffer_bits, 0); // Écrire le bloc dans le flux de bits
        }

        if (PGM_PPM[1] == '6'){
            for (uint8_t j=0; j<numCBbyMCU; j++){
                applyDCT(Cb_values[i*numCBbyMCU + j], frequency_block);                    // Appliquer la DCT au bloc de composant Y
                zzBlock(frequency_block, zz_block);                                        // Effectuer l'ordre en zig-zag des coefficients DCT
                quantizeBlock(zz_block, quantification_table_CbCr);                        // Quantifier les coefficients en utilisant la table de quantification Y
                bitstream_block(file_ptr, zz_block, &DC_previousCb, &buffer, &buffer_bits, 1);// Écrire le bloc dans le flux de bits
            }
            for (uint8_t j=0; j<numCRbyMCU; j++){
                applyDCT(Cr_values[i*numCRbyMCU + j], frequency_block);                    // Appliquer la DCT au bloc de composant Y
                zzBlock(frequency_block, zz_block);                                        // Effectuer l'ordre en zig-zag des coefficients DCT
                quantizeBlock(zz_block, quantification_table_CbCr);                        // Quantifier les coefficients en utilisant la table de quantification Y
                bitstream_block(file_ptr, zz_block, &DC_previousCr, &buffer, &buffer_bits, 2);// Écrire le bloc dans le flux de bits
            }
        }
    }

    // Écrire les bits restants dans le buffer dans le fichier
    if (buffer_bits > 0) {
        buffer <<= (8 - buffer_bits);
        fwrite(&buffer, 1, 1, file_ptr);
    }

    // Libérer la mémoire allouée
    free(frequency_block);
    free(zz_block);
}
