#ifndef READ_PPM_H
#define READ_PPM_H

/**
 * @param input_filename nom du fichier image d'entée
 * @param MCU_width largeur de la mcu (sample factor horizontal * 8 )
 * @param MCU_height hauteur de la mcu (sample factor vertical * 8)
 * @param number_of_mcus adresse du nombre représentant le nombre de mcus de l'image
*/
uint8_t*** read_from_input_filename_PPM(const char* input_filename, int MCU_width, int MCU_height, int *number_of_mcus);

#endif /* READ_PPM_H */
