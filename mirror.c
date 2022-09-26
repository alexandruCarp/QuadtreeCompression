#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "compress.h"

//functie pentru interschimbarea a doi fii
void swap(QTree *child1, QTree *child2){
    QTree aux = *child1;
    *child1 = *child2;
    *child2 = aux;
}

//parcurg arborele in adancime si daca nodul are fii ii interschimb in functie de tipul oglindirii
void dfs(QTree node, char type){
    if(node->topL==NULL)
        return;
    if(type == 'h'){
        swap(&node->topL,&node->topR);
        swap(&node->botL,&node->botR);
    }
    else{
        swap(&node->topL,&node->botL);
        swap(&node->topR,&node->botR);
    }
    dfs(node->topL,type);
    dfs(node->topR,type);
    dfs(node->botR,type);
    dfs(node->botL,type);
}

//folosesc functiile de construire a arborelui, pe care il prelucrez si il transform inapoi in matricea de pixeli
void mirror(char type, char *filenameIN, char *filenameOUT, int threshold){
    int width,height;
    Pixel **image = readPPM(filenameIN,&width,&height);
    uint32_t numar_culori, numar_noduri;
    QTree root = buildTree(image,width,0,0,threshold,&numar_culori,&numar_noduri);
    dfs(root,type);
    buildImage(image,0,0,root);

    FILE *file = fopen(filenameOUT,"w");
    fprintf(file,"P6\n");
    fprintf(file,"%d %d\n",width,height);
    fprintf(file,"255\n");

    for(int i = 0; i < height; i++)
        fwrite(image[i],sizeof(Pixel),width,file);
    fclose(file);
    free_matrix(image,width);
    free_tree(root);
}