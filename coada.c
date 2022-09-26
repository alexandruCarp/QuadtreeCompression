#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "coada.h"

//functii pentru coada

Coada initQ(){
    Coada coada = malloc(sizeof(*coada));
    coada->head = NULL;
    coada->tail = NULL;
    return coada;
}

int is_empty(Coada coada){
    if(coada->head == NULL)
        return 1;
    return 0;
}

void enqueue(Coada coada, QTree x){
    List nou = malloc(sizeof(*nou));
    nou->val = x;
    nou->next = NULL;
    if(is_empty(coada)){
        coada->head = nou;
        coada->tail = nou;
    }
    else{
        coada->tail->next = nou;
        coada->tail = nou;
    }
}

void dequeue(Coada coada){
    List p = coada->head;
    coada->head = p->next;
    free(p);
}

QTree first(Coada coada){
    return coada->head->val;
}

QTree last(Coada coada){
    return coada->tail->val;
}