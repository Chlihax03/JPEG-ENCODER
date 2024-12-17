# Projet d'encodeur JPEG

Ce projet consiste en l'implémentation d'un encodeur JPEG en langage C. Il permet de compresser des images au format PGM (niveaux de gris) ou PPM (couleurs) en suivant le standard JPEG.


## Répartition des tâches et estimation du temps

**Omar OUKHTITE:**
- Récupération des paramètres depuis la ligne de commande;
- Récupération des paramètres depuis le fichier d'entrée;
- Conversion des données RGB en YCbCr;
- Écriture dans le fichier de sortie.

**Ziyad CHLIHI:**
- Récupération des données RGB depuis l'image d'entrée;
- Sous-échantillonnage (downsampling).

**Adil KASSAOUI:**
- Application de la Transformée en Cosinus Discrète (DCT);
- Parcours en zig-zag;
- Quantification;
- Codage de Huffman;
- Génération du flux binaire.

**Travail en commun:**
- Assemblage des différents modules et ecriture du programme principal (main).

* Pour la répartition des tâches, nous nous sommes basés sur le niveau de difficulté de chaque étape, comme mentionné dans la documentation du codec JPEG. La seule estimation dont nous sommes sûrs, bien que évidente, est que la durée nécessaire pour chaque étape sera proportionnelle à sa difficulté.

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


## Avancement du projet

Nous avons réussi à compresser des images au format PPM et PGM sans utiliser de sous-échantillonnage. Les étapes de lecture de l'image, de conversion RGB en YCbCr (pour PPM), de compression, de génération et d'écriture du flux binaire dans l'image sont fonctionnelles.

**Difficultes rencontres**
1. Pour la compression des images PPM avec sous-échantillonnage, les images sont compressées mais elles sont un peu perturbées (quelques pixels sont decales), selon les facteurs d'échantillonnage (par exemple l'image zig-zag.ppm avec les facteurs d'échantillonnage 2x2,1x1,1x1).
2. L'utilisation d'une structure MCU nous a posé problème dans la partie de compression des images PPM, alors que pour les images PGM, nous avons réussi à les compresser en utilisant cette structure.

**Tâches restantes**
1. Optimiser la DCT: Améliorer la performance de la DCT en implémentant des algorithmes optimisés.
2. Définir et utiliser une structure pour les MCUs:
                    Créer une structure MCU qui encapsule les données Y, Cb, Cr.
                    Adapter les fonctions existantes pour utiliser cette nouvelle structure.
3. Corriger le sous-échantillonnage:
                    Compresser des images PPM avec sous-échantillonnage sans provoquer de décalages de pixels.

