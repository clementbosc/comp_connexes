#include "limace.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TAB_SIZE 10000//1024

/* ------------------------------------------------------------------- 
 * Fonctions utiles
 -------------------------------------------------------------------  */
typedef enum {
  V4=0,
  V8=1
}TypeVoisinage;

int max(int x,int y){
  int res=x>y?x:y;
  return res;
}
int min(int x,int y){
  int res=x<y?x:y;
  return res;
}


void saveResult(Matrix res,const char* fileName){
  int** data=MatGetInt(res);
  int x,y;
  int labelMax=0;
  int newLabel;
  int *newLabels;
  
  /*find max value of labels */
  for(y=0;y<MatNbRow(res);y++){
    for(x=0;x<MatNbCol(res);x++){
      labelMax=max(data[y][x],labelMax);
    }
  }
  labelMax++;
  newLabels=(int*)malloc(sizeof(int)*labelMax);
  for(x=0;x<labelMax;x++){
    newLabels[x]=-1;
  }
  /*create continious labels*/
  newLabels[0]=0;
  newLabel=1;
  
  for(y=0;y<MatNbRow(res);y++){
    for(x=0;x<MatNbCol(res);x++){
      if(newLabels[data[y][x]]==-1){
        newLabels[data[y][x]]=newLabel;
        newLabel++;
      }
    }
  }
  for(y=0;y<MatNbRow(res);y++){
    for(x=0;x<MatNbCol(res);x++){
      data[y][x]=newLabels[data[y][x]];
    }
  }
  
  /*save result as image*/
  {
    Image image=ImCAlloc(Color,MatNbRow(res),MatNbCol(res));
    unsigned char** red=ImGetR(image);
    unsigned char** green=ImGetG(image);
    unsigned char** blue=ImGetB(image);
    long step=(256*256*256)/newLabel;
    for(y=0;y<MatNbRow(res);y++){
      for(x=0;x<MatNbCol(res);x++){
        int etiquette=data[y][x];
        long idxColor=step*etiquette;
        blue[y][x]=idxColor/(256*256);
        idxColor=idxColor%(256*256);
        green[y][x]=idxColor/256;
        red[y][x]=idxColor%256;
      }
    }
    ImWrite(image,fileName);
    
    ImFree(&image);
  }
  
  
  free(newLabels);
  
}

/*create array to save equivalence*/
int* creerTableau(){
 int i;
  int* tab=(int*)malloc(sizeof(int)*TAB_SIZE);
  
  for( i=0;i<TAB_SIZE;i++){
    tab[i] = -1;
  }
  
  return tab;
}

/* add label and increment next label value*/
void ajoutEtiquette(int* equivalences, int* nbEtiquettes){
  equivalences[*nbEtiquettes]=*nbEtiquettes;
  *nbEtiquettes=(*nbEtiquettes)+1;
}


/* ------------------------------------------------------------------- 
 * To do
 -------------------------------------------------------------------  */
 
Matrix coordonneesVoisins(Image image,int y,int x, TypeVoisinage voisinage){
    Matrix coords = NULL;
    int NbLig,NbCol;
    int **M;

    NbLig = ImNbRow(image) - 1; 
    NbCol = ImNbCol(image) - 1;


    if (voisinage == V4){



        if(x == 0 && y == 0){ /*Coin 1*/
            coords = MatAlloc(Int,2,2);
            M = MatGetInt(coords);

            M[1][0] = x+1;
            M[0][0] = y;

            M[1][1] = x;
            M[0][1] = y+1;
            
        }
        else if(x == NbCol && y == 0){ /*Coin 2*/
            coords = MatAlloc(Int,2,2);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y;

            M[1][1] = x;
            M[0][1] = y+1;
            
        }
        else if(x == 0 && y == NbLig){ /*Coin 3*/
            coords = MatAlloc(Int,2,2);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x+1;
            M[0][1] = y;
            
        }
        else if(x == NbCol && y == NbLig){ /*Coin 4*/
            coords = MatAlloc(Int,2,2);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x-1;
            M[0][1] = y;
            
        }

        else if(x > 0 && y == 0){ /* Bord haut */
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y;

            M[1][1] = x+1;
            M[0][1] = y;

            M[1][2] = x;
            M[0][2] = y+1;
            
        }

        else if(x == NbCol && y > 0){  /* Bord droite */
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x-1;
            M[0][1] = y;

            M[1][2] = x;
            M[0][2] = y+1;
            
        }

        else if(x > 0 && y == NbLig){ /* Bord bas */
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x-1;
            M[0][1] = y;

            M[1][2] = x+1;
            M[0][2] = y;
            
        } 

        else if(x == 0 && y > 0){ /* Bord gauche */
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x+1;
            M[0][1] = y;

            M[1][2] = x;
            M[0][2] = y+1;
            
        } 
        
        else if(x > 0 && y > 0){ /* millieu */
            coords = MatAlloc(Int,2,4);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x-1;
            M[0][1] = y;

            M[1][2] = x+1;
            M[0][2] = y;

            M[1][3] = x;
            M[0][3] = y+1;
            
        }

    }
    else if(voisinage == V8){

        if(x == 0 && y == 0){ /*Coin 1*/
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x+1;
            M[0][0] = y;

            M[1][1] = x;
            M[0][1] = y+1;

            M[1][2] = x+1;
            M[0][2] = y+1;

        }
        else if(x == NbCol && y == 0){ /*Coin 2*/
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y;

            M[1][1] = x-1;
            M[0][1] = y+1;

            M[1][2] = x;
            M[0][2] = y+1;
            
        }
        else if(x == 0 && y == NbLig){ /*Coin 3*/
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x+1;
            M[0][1] = y-1;

            M[1][2] = x+1;
            M[0][2] = y;
            
        }
        else if(x == NbCol && y == NbLig){ /*Coin 4*/
            coords = MatAlloc(Int,2,3);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y-1;

            M[1][1] = x;
            M[0][1] = y-1;

            M[1][2] = x-1;
            M[0][2] = y;
            
        }
        else if(x > 0 && y == 0){ /* Bord haut */
            coords = MatAlloc(Int,2,5);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y;

            M[1][1] = x+1;
            M[0][1] = y;

            M[1][2] = x-1;
            M[0][2] = y+1;

            M[1][3] = x;
            M[0][3] = y+1;

            M[1][4] = x+1;
            M[0][4] = y+1;
            
        }

        else if(x == NbCol && y > 0){  /* Bord droite */
            coords = MatAlloc(Int,2,5);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y-1;

            M[1][1] = x;
            M[0][1] = y-1;

            M[1][2] = x-1;
            M[0][2] = y;

            M[1][3] = x-1;
            M[0][3] = y+1;

            M[1][4] = x;
            M[0][4] = y+1;
            
        }

        else if(x > 0 && y == NbLig){ /* Bord bas */
            coords = MatAlloc(Int,2,5);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y-1;

            M[1][1] = x;
            M[0][1] = y-1;

            M[1][2] = x+1;
            M[0][2] = y-1;

            M[1][3] = x-1;
            M[0][3] = y;

            M[1][4] = x+1;
            M[0][4] = y;
            
        } 

        else if(x == 0 && y > 0){ /* Bord gauche */
            coords = MatAlloc(Int,2,5);
            M = MatGetInt(coords);

            M[1][0] = x;
            M[0][0] = y-1;

            M[1][1] = x+1;
            M[0][1] = y-1;

            M[1][2] = x+1;
            M[0][2] = y;

            M[1][3] = x;
            M[0][3] = y+1;

            M[1][4] = x+1;
            M[0][4] = y+1;
            
        } 
        
        else if(x > 0 && y > 0){ /* millieu */
            coords = MatAlloc(Int,2,8);
            M = MatGetInt(coords);

            M[1][0] = x-1;
            M[0][0] = y-1;

            M[1][1] = x;
            M[0][1] = y-1;

            M[1][2] = x+1;
            M[0][2] = y-1;

            M[1][3] = x-1;
            M[0][3] = y;

            M[1][4] = x+1;
            M[0][4] = y;

            M[1][5] = x-1;
            M[0][5] = y+1;

            M[1][6] = x;
            M[0][6] = y+1;

            M[1][7] = x+1;
            M[0][7] = y+1;
            
        }

    }

    return coords;
}


int findCC(int* equivalences, int nbEtiquettes, int etiquette) {

    if (equivalences[etiquette] == etiquette){
        return etiquette;
    }else{
        return findCC(equivalences, nbEtiquettes, equivalences[etiquette]);
    }
  
}


void unionCC(int* equivalences, int nbEtiquettes, int etiquette1, int etiquette2){
    int ref1, ref2;

    ref1 = findCC(equivalences, nbEtiquettes, etiquette1);
    ref2 = findCC(equivalences, nbEtiquettes, etiquette2);

    equivalences[ref2] = ref1;
}

bool voisinsHasSameEtiquette(Matrix voisins, Matrix res){
    int **Mvoisins, **Mres;

    Mvoisins = MatGetInt(voisins);
    Mres = MatGetInt(res);
    int sizeVoisins = MatNbCol(voisins);

    int xPremierVoisin = Mvoisins[0][0];
    int yPremierVoisin = Mvoisins[1][0];

    for (int i = 0; i < sizeVoisins; i++)
    {
        int x = Mvoisins[0][i];
        int y = Mvoisins[1][i];

        if(Mres[x][y] != Mres[xPremierVoisin][yPremierVoisin]){
            return false;
        }
    }
    return true;
}

bool aucunVoisinAEtiquette(Matrix voisins, Matrix res){
    int **Mvoisins, **Mres;

    Mvoisins = MatGetInt(voisins);
    Mres = MatGetInt(res);
    int sizeVoisins = MatNbCol(voisins);

    for (int i = 0; i < sizeVoisins; i++)
    {
        int x = Mvoisins[0][i];
        int y = Mvoisins[1][i];

        if(Mres[x][y] != 0){
            return false;
        }
    }
    return true;
}


Matrix filterVoisins(Matrix voisins, Image image){

    unsigned char **I;
    int **Mvoisins, **Mres;
    Mvoisins = MatGetInt(voisins);
    int j = 0;

    I = ImGetI(image);
    int sizeVoisins = MatNbCol(voisins);

    for (int i = 0; i < sizeVoisins; i++)
    {
        int x = Mvoisins[0][i];
        int y = Mvoisins[1][i];
        if(I[x][y] == 1){
            j++;
        }
    }

    Matrix coords = MatAlloc(Int,2,j);
    Mres = MatGetInt(coords);
    j = 0;

    for (int i = 0; i < sizeVoisins; i++)
    {
        int x = Mvoisins[0][i];
        int y = Mvoisins[1][i];

        if(I[x][y] == 1){
            Mres[0][j] = x;
            Mres[1][j] = y;
            j++;
        }
    }

    return coords;
}



int main(int argc, char const *argv[]){
    Image image;
    Matrix res;
    Matrix voisins;
    int nbEtiquettes = 0;
    int* equivalences;
    unsigned char **I;
    int i, j, NbLig, NbCol;
    TypeVoisinage type_voisinage;

    int **Mres, **Mvoisins;

    if (argc < 4){
        printf("Usage : ./comp_connexes image_source image_dest type_voisinage\n");
        return 1;
    }

    if(strcmp(argv[3], "V4")){
        type_voisinage = V8;
    }else if(strcmp(argv[3], "V8")){
        type_voisinage = V4;
    }else{
        printf("Type voisinage inconnu\n");
        return 1;
    }

    image = ImRead(argv[1]);

    /*Si erreur lecture image*/
    if (image == NULL) return 1;

    NbLig = ImNbRow(image); 
    NbCol = ImNbCol(image);

    res = MatAlloc(Int, NbLig, NbCol);
    Mres = MatGetInt(res);

    //Créer le tableau d'équivalences
    equivalences = creerTableau();

    //Ajouter l'étiquette 0 au tableau d'équivalence (pour le fond)
    ajoutEtiquette(equivalences, &nbEtiquettes);

    I = ImGetI(image);


    //Pour chaque pixel
    for (i = 0; i < NbLig; i++){
        for (j = 0; j < NbCol; j++){


            // Si le pixel appartient à la forme
            if(I[i][j] == 1){

                //Récupérer ses voisins
                voisins = coordonneesVoisins(image, i, j, type_voisinage);
                voisins = filterVoisins(voisins, image);
                Mvoisins = MatGetInt(voisins);
                int sizeVoisins = MatNbCol(voisins);

                //Si aucun des voisins n'a d'étiquette
                if(aucunVoisinAEtiquette(voisins, res)){

                    //Créer une nouvelle étiquette
                    Mres[i][j] = nbEtiquettes;
                    ajoutEtiquette(equivalences, &nbEtiquettes);
                }
                else{
                    
                    // Si tous les voisins ont la même étiquette
                    if(voisinsHasSameEtiquette(voisins, res)){
                        //Donner l'étiquette au pixel
                        Mres[i][j] = Mres[Mvoisins[0][0]][Mvoisins[1][0]];
                    }else{
                        // Donner l'étiquette du premier voisin au pixel
                        Mres[i][j] = Mres[Mvoisins[0][0]][Mvoisins[1][0]];

                        //Stocker la ou les équivalences
                        //Pour chaque voisin
                        for (int k = 1; k < sizeVoisins; k++)
                        {
                            int x = Mvoisins[0][k];
                            int y = Mvoisins[1][k];
                            if(Mres[x][y] > 0){
                                unionCC(equivalences, nbEtiquettes, Mres[i][j], Mres[x][y]);                        
                            }
                        }
                        
                    }
                }
                
                //libérer la mémoire
                MatFree(&voisins);

            }
        }
    }

    for (i = 0; i < NbLig; i++){
        for (j = 0; j < NbCol; j++){
            if(I[i][j] == 1){
                Mres[i][j] = findCC(equivalences,nbEtiquettes,Mres[i][j]);
            }
        }
    } 

    saveResult(res, argv[2]);
    

    return 0;
}
