/* Ionita Alexandra-Mihaela 314CB */
#include "tema3.h"

int main()
{
    TGL* map;

    map = ReadMap();
    if(!map) { 
        fprintf(stderr, "-=- eroare la citire -=-\n"); 
        return -1;
    }

    int *componente_conexe = calloc(sizeof(int), map->n);
    int cnt = Componente_conexe(map, &componente_conexe);
    Cost_Total_Minim(map, componente_conexe, cnt);

    Distroy_Map(map);
    free(componente_conexe);
    return 0;
}
