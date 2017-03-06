#include "limace.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAB_SIZE 1024

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
    unsigned char **I;
    int **M;

    NbLig = ImNbRow(image) - 1; 
    NbCol = ImNbCol(image) - 1;


    //I = ImGetI(image);

    if (voisinage == V4){



        if(x == 0 && y == 0){ /*Coin 1*/
            coords = MatAlloc(Int,2,2);
            M = MatGetInt(coords);

            M[1][0] = x+1;
            M[0][0] = y;

            M[1][1] = x;
            M[0][1] = y+1;
        }else if(x == NbCol && y == 0){ /*Coin 2*/
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
 int result=-1;
 return result;
  
}


void unionCC(int* equivalences, int nbEtiquettes, int etiquette1, int etiquette2){
}


int main(int argc, char const *argv[]){
    Image image;
    Matrix res;
    int NbLig,NbCol;

    image = ImRead("TestCC.pbm");

    /*Si erreur lecture image*/
    if (image == NULL) return 1;

    NbLig=ImNbRow(image); 
    NbCol=ImNbCol(image);

    res = coordonneesVoisins(image, 399, 599, V4);
    MatWriteAsc(res, "");
    res = coordonneesVoisins(image, 399, 599, V8);

    MatWriteAsc(res, "");

    return 0;
}
