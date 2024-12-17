#ifndef RGB_TO_YCBCR_H_INCLUDED
#define RGB_TO_YCBCR_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

/**
 * @brief Convertit une couleur RVB en YCbCr.
 * 
 * @param R Valeur de la composante R (0-255).
 * @param G Valeur de la composante G (0-255).
 * @param B Valeur de la composante B (0-255).
 * @param Y Pointeur vers la variable où stocker la composante Y (0-255).
 * @param Cb Pointeur vers la variable où stocker la composante Cb (0-255).
 * @param Cr Pointeur vers la variable où stocker la composante Cr (0-255).
 */
void rgb_to_ycbcr(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* Cb, uint8_t* Cr);

#endif // RGB_TO_YCBCR_H_INCLUDED
