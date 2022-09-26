#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "structs.h"
#include "compress.h"

int main(int argc, char **argv)
{
    //se apeleaza functia corespunzatoare in functie de argumente
        
    if(!strcmp(argv[1],"-c")){
        compress(argv[3],argv[4],atoi(argv[2]));
    }
    if(!strcmp(argv[1],"-d")){
        decompress(argv[2],argv[3]);
    }
    if(!strcmp(argv[1],"-m")){
        mirror(argv[2][0],argv[4],argv[5],atoi(argv[3]));
    }
    return 0;
}