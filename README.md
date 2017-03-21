# Composantes connexes
Algorithme d'identification de composantes connexes d'une image binaire.

## Utilisation
`./comp_connexes image_source image_dest type_voisinage`
avec `image_source` l'adresse de l'image à traiter (format .pbm), `image_dest` le chemin de l'image de sortie (format .ppm) et `type_voisignage` "V8" ou "V4" (pour une traitement à 8 voisignage ou à 4).

## Résultats

Pour l'image d'origine suivante :
![Image originale](https://ups.clementbosc.fr/tp2-igtai/original.jpg)

On obtient le résulat suivant en 4 voisinages :
![Image originale](https://ups.clementbosc.fr/tp2-igtai/res_v4.jpg)

Et pour 8 voisinages (une seule forme reconnue) :
![Image originale](https://ups.clementbosc.fr/tp2-igtai/res_v8.jpg)
