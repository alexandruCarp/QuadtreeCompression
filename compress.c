#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "coada.h"

//functia face citirea dintr-un fisier PPM si returneaza matricea de pixeli
Pixel** readPPM(char *filename,int *width,int *height){
    char type[10];
    unsigned int maxValue;
    FILE *file = fopen(filename,"r");
    
    fscanf(file,"%s",type);
    fscanf(file,"%u %u",width,height);
    fscanf(file,"%u",&maxValue);
    getc(file);

    Pixel **image = malloc(*height * sizeof(Pixel *));
    for(int i = 0; i < *height; i++){
        image[i] = malloc(*width * sizeof(Pixel));
        fread(image[i],sizeof(Pixel),*width,file);
    }

    fclose(file);

    return image;
}

//functie ce scoate intregii din fractie pe masura ce calculeaza suma, pentru a evita overflow-ul
void addtoSum(unsigned int *sum, unsigned int term,unsigned int denominator, unsigned int *result){
    *sum += term;
    *result += *sum / denominator;
    *sum = *sum % denominator;
}

//functie recursiva pentru construirea arborelui plecand de la matricea de pixeli
QTree buildTree(Pixel **image, int size, int x, int y,unsigned int threshold, uint32_t *numar_culori, uint32_t *numar_noduri){
    QTree node = malloc(sizeof(*node));
    (*numar_noduri)++;
    unsigned int resR = 0,resG = 0,resB = 0;
    node->size = size;
    unsigned int sumR = 0,sumG = 0,sumB = 0;

    //se calculeaza culoarea medie
    for(int i = x; i < x + size; i++){
        for(int j = y; j < y + size; j++){
            addtoSum(&sumR,image[i][j].r,size*size,&resR);
            addtoSum(&sumG,image[i][j].g,size*size,&resG);
            addtoSum(&sumB,image[i][j].b,size*size,&resB);
        }
    }
    node->pix.r = resR;
    node->pix.g = resG;
    node->pix.b = resB;
    unsigned int meanScore = 0, sum = 0;

    //se calculeaza scorul de similaritate
    for(int i = x; i < x + size; i++){
        for(int j = y; j < y + size; j++){
            addtoSum(&sum, (node->pix.r - image[i][j].r)*(node->pix.r - image[i][j].r), 3*size*size, &meanScore);
            addtoSum(&sum, (node->pix.g - image[i][j].g)*(node->pix.g - image[i][j].g), 3*size*size, &meanScore);
            addtoSum(&sum, (node->pix.b - image[i][j].b)*(node->pix.b - image[i][j].b), 3*size*size, &meanScore);
        }
    }
    if(meanScore > threshold){

        //se construiesc nodurile fiu (parametrii x si y reprezinta coltul din stanga-sus al submatricii)
        node->topL = buildTree(image, size/2, x, y, threshold,numar_culori,numar_noduri);
        node->topR = buildTree(image, size/2, x, y + size/2, threshold,numar_culori,numar_noduri);
        node->botL = buildTree(image, size/2, x + size/2, y, threshold,numar_culori,numar_noduri);
        node->botR = buildTree(image, size/2, x + size/2, y + size/2, threshold,numar_culori,numar_noduri);
    }
    else{
        node->topL = node->topR = node->botL = node->botR = NULL;
        (*numar_culori)++;
    }
    return node;
}

//functie pentru construirea vectorului din arbore
void buildVector(QuadtreeNode *vector, QTree root){

    //folosesc coada pentru parcurgerea in latime a arborelui
    Coada q = initQ();
    enqueue(q,root);

    //currentIndex reprezinta indexul elementului curent, care creste dupa prelucrarea fiecarui nod
    //childIndex reprezinta indexul pe care il va avea fiul nodului curent, care creste dupa fiecare adaugare in coada
    int32_t currentIndex = 0;
    int32_t childIndex = 0;
    while (!is_empty(q))
    {
        QTree current = first(q);
        dequeue(q);
        vector[currentIndex].area = current->size*current->size;
        vector[currentIndex].red = current->pix.r;
        vector[currentIndex].green = current->pix.g;
        vector[currentIndex].blue = current->pix.b;
        if(current->topL != NULL){
            vector[currentIndex].top_left = ++childIndex;
            vector[currentIndex].top_right = ++childIndex;
            vector[currentIndex].bottom_right = ++childIndex;
            vector[currentIndex].bottom_left = ++childIndex;

            enqueue(q,current->topL);
            enqueue(q,current->topR);
            enqueue(q,current->botR);
            enqueue(q,current->botL);
        }
        else{
            vector[currentIndex].top_left = -1;
            vector[currentIndex].top_right = -1;
            vector[currentIndex].bottom_right = -1;
            vector[currentIndex].bottom_left = -1;
        }

        currentIndex++;
    }
    free(q);
}

//functie pentru eliberarea memoriei alocate pentru matrice
void free_matrix(Pixel **img,int size){
    for(int i = 0; i < size; i++)
        free(img[i]);
    free(img);
}

//functie pentru eliberarea memoriei alocate pentru arbore
void free_tree(QTree root){
    if(root == NULL)
        return;
    free_tree(root->topL);
    free_tree(root->topR);
    free_tree(root->botR);
    free_tree(root->botL);
    free(root);
}

//citesc matricea,construiesc arborele, apoi vectorul, apoi il scriu in fisier
void compress(char *filenameIN, char *filenameOUT, unsigned int threshold){
    int width, height;
    Pixel **image = readPPM(filenameIN,&width,&height);
    uint32_t numar_culori = 0,numar_noduri = 0;
    QTree root = buildTree(image,width,0,0,threshold,&numar_culori,&numar_noduri);
    QuadtreeNode *vector = malloc(numar_noduri * sizeof(*vector));
    buildVector(vector,root);
    FILE *file = fopen(filenameOUT,"wb");
    fwrite(&numar_culori,sizeof(numar_culori),1,file);
    fwrite(&numar_noduri,sizeof(numar_noduri),1,file);
    fwrite(vector,sizeof(*vector),numar_noduri,file);
    fclose(file);
    free(vector);
    free_matrix(image,width);
    free_tree(root);
}