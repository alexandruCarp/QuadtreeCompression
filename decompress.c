#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "structs.h"
#include "compress.h"

//functie care citeste un fisier binar si returneaza vectorul din care va fi construit arborele
QuadtreeNode* readBinary(char *filename,uint32_t *numar_culori,uint32_t *numar_noduri){
    FILE *file = fopen(filename,"rb");
    fread(numar_culori,sizeof(*numar_culori),1,file);
    fread(numar_noduri,sizeof(*numar_noduri),1,file);
    QuadtreeNode *vector = malloc(*numar_noduri * sizeof(*vector));
    fread(vector,sizeof(*vector),*numar_noduri,file);
    fclose(file);
    return vector;
}

//functie recursiva pentru construirea arborelui din vector
QTree buildTreeFromVector(QuadtreeNode nod, QuadtreeNode *vector){
    QTree treeNode = malloc(sizeof(*treeNode));
    treeNode->pix.r = nod.red;
    treeNode->pix.g = nod.green;
    treeNode->pix.b = nod.blue;
    treeNode->size = sqrt(nod.area);

    //se verifica daca nodul are descendenti, in caz afirmativ se apeleaza functia recursiv pentru fiecare
    if(nod.top_left != -1){
        treeNode->topL = buildTreeFromVector(vector[nod.top_left],vector);
        treeNode->topR = buildTreeFromVector(vector[nod.top_right],vector);
        treeNode->botR = buildTreeFromVector(vector[nod.bottom_right],vector);
        treeNode->botL = buildTreeFromVector(vector[nod.bottom_left],vector);
    }
    else{
        treeNode->topL = treeNode->topR = treeNode->botL = treeNode->botR = NULL;
    }
    return treeNode;
}

//functie pentru constrirea matricei de pixeli din arbore
void buildImage(Pixel **image, int x, int y,QTree nod){

    //se parcurge in adancime arborele pana se ajunge la frunze, care se pun in matrice
    if(nod->topL != NULL){
        buildImage(image, x, y, nod->topL);
        buildImage(image, x, y + nod->size/2, nod->topR);
        buildImage(image, x + nod->size/2, y, nod->botL);
        buildImage(image, x + nod->size/2, y + nod->size/2, nod->botR);
    }
    else{
        for(int i = x; i < x + nod->size; i++)
            for(int j = y; j < y + nod->size; j++)
                image[i][j] = nod->pix;
    }
}

//citesc vectorul, construiesc arborele, apoi matricea pe care o scriu in fisierul PPM
void decompress(char *filenameIN, char *filenameOUT){
    uint32_t numar_noduri,numar_culori;
    QuadtreeNode *vector = readBinary(filenameIN, &numar_culori, &numar_noduri);
    QTree root = buildTreeFromVector(vector[0],vector);
    int size = root->size;
    Pixel **image = malloc(size * sizeof(*image));
    for(int i = 0; i < size; i++){
        image[i] = malloc(size * sizeof(**image));
    }
    buildImage(image,0,0,root);

    FILE *file = fopen(filenameOUT,"w");
    fprintf(file,"P6\n");
    fprintf(file,"%d %d\n",size,size);
    fprintf(file,"255\n");
    for(int i = 0; i < size; i++)
        fwrite(image[i],sizeof(Pixel),size,file);
    fclose(file);
    free_matrix(image,size);
    free_tree(root);
    free(vector);
}