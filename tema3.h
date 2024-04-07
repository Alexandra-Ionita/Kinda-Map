/* Ionita Alexandra-Mihaela 314CB */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// Structura pentru datele unui nod
typedef struct {
    int key; // Cheia unică a nodului
    char *name; // Numele nodului
} Data;

// Structura pentru o celulă a unui arc
typedef struct celarc {
    Data data; // Datele nodului
    int c; // Costul arcului
    struct celarc *urm; // Următorul arc în listă
} TCelArc, *AArc;

// Structura pentru un graf
typedef struct {
    int n; // Numărul de noduri
    AArc* x; // Vector de liste de adiacență pentru fiecare nod
} TGL;

// Structura pentru un nod în coada de priorități
typedef struct prio {
    AArc info; // Informațiile despre arc
    struct prio *next; // Următorul element în coada de priorități
} Priority_Queue;

TGL* ReadMap();  /* citeste descriere graf */
void Insert_Connection(AArc dest, AArc src, int cost);
void Insert_Head(AArc head, int index_node, char* location);
TGL* AlocG(int nr);
void Distroy_Map(TGL* map);
int Componente_conexe(TGL *map, int **componente_conexe);
int Lazy_prim (TGL* map, AArc head);
void Cost_Total_Minim(TGL* map, int *componente_conexe, int cnt);
void dequeue(Priority_Queue **pq, TGL *map);
void addEdges(AArc node, int **visited, Priority_Queue **pq);
void Distroy_pq(Priority_Queue *pq);
Priority_Queue* AlocCelula(AArc elem);