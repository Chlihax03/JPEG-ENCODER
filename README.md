# Projet d'encodeur JPEG

Ce projet consiste en l'implémentation d'un encodeur JPEG en langage C. Il permet de compresser des images au format PGM (niveaux de gris) ou PPM (couleurs) en suivant le standard JPEG.

## Modules

Le projet est divisé en plusieurs modules, chacun gérant une partie spécifique de l'encodage :

1. **read_PPM**: Lecture des données d'une image au format PPM.
2. **read_PGM**: Lecture des données d'une image au format PGM.
3. **downsampling**: Sous-échantillonnage des composantes de chrominance.
4. **RGB2YCBCR**: Conversion des données RGB en YCbCr.
5. **dct**: Calcul de la Transformée en Cosinus Discrète (DCT).
6. **quantification**: Quantification des coefficients DCT.
7. **zig_zag**: Parcours en zig-zag des blocs 8x8.
8. **compressionACDC**: Codage en entropie (RLE et Huffman) des coefficients AC et DC.
9. **compressionMCUs**: Génération du flux binaire pour toutes les MCUs de l'image.
10. **bitstream**: Écriture du flux binaire dans un fichier.
11. **ecrit_marqueurs**: Écriture des marqueurs et sections JPEG dans le flux.

## Structures de données

---------------------------
|   Lecture de l'image    |
|-------------------------|
| Structure:              |
| - uint8_t*** MCUs       |
| - uint8_t** Y_values    |
---------------------------
-----------------------------------
| Conversion RGB en YCbCr         |
|---------------------------------|
| Structure:                      |
| - uint8_t** Y_values            |
| - uint8_t** Cb_values           |
| - uint8_t** Cr_values           |
-----------------------------------
------------------------------------
| Sous-échantillonnage             |
|----------------------------------|
| Structure:                       |
| - uint8_t*** Y_sampled           |
| - uint8_t*** Cb_sampled          |
| - uint8_t*** Cr_sampled          |
------------------------------------
------------------------------------
| Concaténation des blocs          |
|----------------------------------|
| Structure:                       |
| - uint8_t** Y                    |
| - uint8_t** Cb                   |
| - uint8_t** Cr                   |
------------------------------------
------------------------------------
| Écriture des sections de fichier |
|----------------------------------|
| Structure:                       |
| - FILE *fptr                     |
| - bitstream_type *bitstream      |
------------------------------------
------------------------------------
| Compression des MCUs             |
|----------------------------------|
| Tableau des MCU:                 |
| - uint8_t **mcus                 |
------------------------------------
------------------------------------
| Écriture du flux binaire final   |
|----------------------------------|
| Structure:                       |
| - bitstream_type *bitstream      |
------------------------------------



