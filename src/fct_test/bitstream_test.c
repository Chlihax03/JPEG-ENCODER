#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/bitstream.h"


int main() {
    // Définition du coefficient DC et des coefficients AC
    int16_t linear_block[LINEAR_BLOCK_SIZE] = {
        0x0018, 0x0000, 0xffff, 0xffbe, 0x0000, 0xffe9, 0x0000, 0xfff0,
        0x0000, 0xffce, 0x001f, 0x0000, 0xfff2, 0x0000, 0x0000, 0x0000,
        0x001a, 0x0000, 0xffed, 0x0000, 0xfff9, 0xfff8, 0x0000, 0x0010,
        0x0000, 0x000d, 0x0000, 0xfffe, 0x0000, 0xfffb, 0x0000, 0x0001,
        0x0000, 0x0002, 0x0000, 0x000c, 0x0000, 0x0000, 0x0000, 0x000b,
        0x0000, 0x000d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0003, 0x0000,
        0xfffb, 0x0000, 0xfff9, 0x0000, 0x0000, 0x0000, 0x0000, 0xfff0,
        0x0000, 0xfffe, 0x0000, 0x0008, 0x0000, 0x0000, 0xfffa, 0x0000
    };

    // Initialisation des variables pour le buffer de bits
    uint8_t buffer = 0;
    uint8_t buffer_bits = 0;
    int16_t DC_previous = 0; // Coefficient DC précédent
    uint8_t component = 0; // Composant (Y, Cb ou Cr), ici Y = 0

    // Ouvrir le fichier de sortie
    FILE *file_ptr = fopen("bitstream_invader.test", "wb");
    if (file_ptr == NULL) {
        perror("Erreur d'ouverture du fichier");
        return EXIT_FAILURE;
    }

    // Traiter le bloc linéaire et écrire les bits dans le fichier
    bitstream_block(file_ptr, linear_block, &DC_previous, &buffer, &buffer_bits, component);

    // Écrire les bits restants dans le buffer
    if (buffer_bits > 0) {
        buffer <<= (8 - buffer_bits);
        fwrite(&buffer, 1, 1, file_ptr);
    }

    /* Résultat attendu tiré de invaders.jpeg
     * d8 c7 c3 df ec 8f ec ff  00 f8 43 75 ff 00 b0 ff
     * 00 c8 6b fb 33 c8 b7 fe  d0 fb 6e df 2b 3b be d9
     * ff 00 1f 7f 6d fe 3f 2b  c8 ff 00 98 7e df b4 7e
     * e6 80
     */

    // Fermer le fichier
    fclose(file_ptr);

    printf("Fichier 'bitstream_invader.test' généré avec succès.\n");

    return EXIT_SUCCESS;
}
