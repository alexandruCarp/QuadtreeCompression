typedef struct nod{
    QTree val;
    struct nod *next;
}Node, *List;

typedef struct coada{
    List head,tail;
}*Coada;

Coada initQ();
int is_empty(Coada coada);
void enqueue(Coada coada, QTree x);
void dequeue(Coada coada);
QTree first(Coada coada);
QTree last(Coada coada);