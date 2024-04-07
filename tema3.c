/* Ionita Alexandra-Mihaela 314CB */
#include "tema3.h"

// Funcția pentru citirea hărții din fișier
TGL* ReadMap()
{
    FILE *InputFile;
    TGL* map = NULL;

    // Alocare memorie pentru string-urile s și d
    char *s;
    s = malloc(100 * sizeof(char));
    if (!s) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    char *d;
    d = malloc(100 * sizeof(char));
    if (!d) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    int cost, current_node = 0;
    int n, arce;

    // Deschiderea fișierului de intrare
    InputFile = fopen ("tema3.in", "rt");
    if (!InputFile) {
        fprintf(stderr, "Failed to open the file"); // Eroare dacă deschiderea a eșuat
        exit(-1);
    }

    // Citirea numărului de noduri și arce din fișier
    fscanf (InputFile, "%i %i", &n, &arce);

    // Alocarea memoriei pentru structura de date a hărții
    map = AlocG(n);
    if (!map) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    // Citirea arcelor și adăugarea lor în harta
    int j = 0;
    for (j = 0; j < arce; j++) { 
        fscanf(InputFile, "%s %s %i", s, d, &cost);
        int exists1 = -1, exists2 = -1;

        // Verificăm dacă nodurile există deja în hartă
        int i;
        for (i = 0; i < current_node; i++) {
            if (!strcmp(map->x[i]->data.name, s)) {
                exists1 = i;
            }

            if (!strcmp(map->x[i]->data.name, d)) {
                exists2 = i;
            }
        }

        // Dacă unul dintre noduri nu există, îl adăugăm în hartă
        if (exists1 == -1) {
            map->x[current_node] = malloc(sizeof(TCelArc));
            if (!map->x[current_node]) {
                fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
                exit(-1);
            }

            Insert_Head(map->x[current_node], current_node, s);
            exists1 = current_node;
            current_node++;
        }

        if (exists2 == -1) {
            map->x[current_node] = malloc(sizeof(TCelArc));
            if (!map->x[current_node]) {
                fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
                exit(-1);
            }

            Insert_Head(map->x[current_node], current_node, d);
            exists2 = current_node;
            current_node++;
        }

        /* Adăugăm conexiunea între nodurile existente sau nou-create. Fiind graf neorientat,
        adaug legatura atat intre sursa si destinatie cat si invers */
        Insert_Connection(map->x[exists2], map->x[exists1], cost);
        Insert_Connection(map->x[exists1], map->x[exists2], cost);
    }

    fclose(InputFile); // Închiderea fișierului de intrare
    free(s); // Eliberarea memoriei alocate pentru s
    free(d); // Eliberarea memoriei alocate pentru d
    return map;
}

// Funcția pentru adăugarea unei conexiuni între două noduri
void Insert_Connection(AArc dest, AArc src, int cost)
{
    AArc aux, ultim = NULL;
    aux = src;

    /* Aflu ultima pozitie din lista de adiacenta a nodului introdus pentru a sti
    unde sa inserez arcul */
    while (aux) {
        ultim = aux;
        aux = aux->urm;
    }

    /*Daca exista un ultim element, adica daca lista are elemente in ea, aloc memorie
    pentru legatura ce vreau sa o adaug si o adaug in lista */

    if (ultim) {
        AArc new_node = malloc(sizeof(TCelArc));
        if (!new_node) {
            fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
            exit(-1);
        }

        new_node->data = dest->data;
        new_node->urm = NULL;
        new_node->c = cost;
        ultim->urm = new_node;
    }
}

// Funcția pentru adăugarea unui nou nod
void Insert_Head(AArc head, int index_node, char* location) 
{
    // Alocare memorie pentru numele locației
    head->data.name = malloc(100 * sizeof(char));
    if (!head->data.name) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    /* Adaug numele in nodoul nou */

    strcpy(head->data.name, location);
    head->data.name[strlen(location)] = '\0';
    head->data.key = index_node;

    head->urm = NULL;
}

// Funcția pentru alocarea spațiului pentru un graf
TGL* AlocG(int nr)
{
    // Alocare memorie pentru descriptorul de graf
    TGL* g = (TGL*)malloc(sizeof(TGL));
    if (!g) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    // Alocare memorie pentru vectorul de adrese ale listelor de arce
    g->x = (AArc*)calloc((nr+1), sizeof(AArc));
    if(!g->x) { 
        free(g);
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    g->n = nr;
    return g;
}

// Funcția pentru eliberarea memoriei ocupate de o listă
void free_list(AArc list)
{
    AArc aux;

    while (list) {
        aux = list;
        list = list->urm;
        free(aux);
    }
}

// Funcția pentru distrugerea hărții, eliberând memoria
void Distroy_Map(TGL* map) 
{
    int i;
    for (i = 0; i <= map->n; i++) {
        if (map->x[i]) {
            free(map->x[i]->data.name);
            free_list(map->x[i]);
        }
    }

    free(map->x);
    free(map);
    map = NULL;
}

/* Algoritm de parcurgere in adancime */

void DFS(TGL *map, int node, int *visited) {
    visited[node] = 1;
    AArc aux = map->x[node];

    while(aux) {
        if(!visited[aux->data.key]) {
            DFS(map, aux->data.key, visited);
        }
        aux = aux->urm;
    }
}

/* Functie de aflare a numarului de componente conexe */

int Componente_conexe(TGL *map, int **componente_conexe) {
    int *visited = calloc(map->n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    int cnt = 0;

    int i;
    for (i = 0; i < map->n; i++) {
        if (!visited[i]) {
            DFS(map, i, visited);
            (*componente_conexe)[cnt] = i;
            cnt++;
        }
    }

    free(visited);

    return cnt;
}

// Funcția pentru alocarea unei celule în coada de priorități
Priority_Queue* AlocCelula(AArc elem) {
    Priority_Queue *aux = (Priority_Queue*) malloc(sizeof(Priority_Queue));
    if (!aux) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    if (aux) {
        aux->info = elem;
        aux->next = NULL;
    }

    return aux;
}

// Funcția pentru distrugerea cozii de priorități
void Distroy_pq(Priority_Queue *pq) {
    Priority_Queue *current = pq;
    while (current != NULL) {
        Priority_Queue *temp = current;
        current = current->next;
        free(temp);
    }
}

// Funcția pentru inserarea unui element în coada de priorități, sortat crescător după cost
Priority_Queue* InsertSorted(Priority_Queue *pq, AArc info)
{
    Priority_Queue *newNode = AlocCelula(info);
    if (!newNode) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    if (pq == NULL || info->c < pq->info->c) {
        newNode->next = pq;
        pq = newNode;
    } else {
        Priority_Queue *curr = pq;
        while (curr->next != NULL && curr->next->info->c < info->c)
        {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }
    return pq;
}

/*Funcția pentru adăugarea arcelor adiacente nodului în coada de priorități. Marcheaza nodul
ca fiind vizitat si adauga toate nodyrile adiacente in lista in ordine crescatoare */
void addEdges(AArc node, int **visited, Priority_Queue **pq)
{
    (*visited)[node->data.key] = 1;
    AArc aux = node->urm;

    while (aux) {
        if (!(*visited)[aux->data.key]) {
            *pq = InsertSorted(*pq, aux);
        }

        aux = aux->urm;
    }
}

/*Funcția pentru parcurgerea algoritmului lui Prim pentru determinarea 
costului minim al arborelui de acoperire*/

int Lazy_prim (TGL* map, AArc head) {
    int mstCost = 0;

    /* Tin socoteala nodurilor deja vizitate */

    int *visited = calloc(map->n, sizeof(int));
    if (!visited) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    /* Adaug nodul de inceput in lista de prioritati */

    Priority_Queue *pq = NULL;
    addEdges(head, &visited, &pq);

    /* Cat timp lista nu e goala, gasesc cel mai bun nod din lista, adica cel cu cel
    mai mic cost, adica primul element din lista deoarece aceasta e sortata, elimin
    nodul si toate nodurile adiacente lui di lista de prioritati */

    while (pq) {
        AArc nbe = pq->info;
        Priority_Queue *temp = pq;
        pq = pq->next; 
        free(temp); // eliberați nodul pe care tocmai l-ați extras

        if (!visited[nbe->data.key]) {
            mstCost += nbe->c;
            addEdges(map->x[nbe->data.key], &visited, &pq);
        }
    }

    // Eliberăm memoria pentru fiecare element din coadă
    Distroy_pq(pq);
    free(visited);

    return mstCost;
}

// Funcția pentru eliminarea nodului din coada de priorități
void dequeue(Priority_Queue **pq, TGL *map) 
{
    if (*pq == NULL) {
        return;
    }

    // Obținem lista de adiacență a primului element
    AArc adjacency_list = map->x[(*pq)->info->data.key];

    // Ștergem primul element din coada de priorități
    Priority_Queue *temp = *pq;
    *pq = (*pq)->next;
    free(temp);

    // Acum eliminăm toate nodurile adiacente cu nodul șters din coada de priorități
    Priority_Queue *prev = NULL, *curr = *pq;
    
    while (curr != NULL) {
        AArc adj = adjacency_list;
        int should_delete = 0;
        while (adj != NULL) {
            if (curr->info->data.key == adj->data.key) {
                should_delete = 1;
                break;
            }
            adj = adj->urm;
        }

        if (should_delete) {
            if (prev != NULL) {
                prev->next = curr->next;
            } else {
                *pq = curr->next;
            }
            temp = curr;
            curr = curr->next;
            free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

// Funcția pentru determinarea costului total minim pentru fiecare componentă conexă a grafului
void Cost_Total_Minim(TGL* map, int *componente_conexe, int cnt) {
    int *costs = calloc(cnt, sizeof(int));
    if (!costs) {
        fprintf(stderr, "Malloc failed"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    int i;
    for (i = 0; i < cnt; i++) {
        costs[i] = Lazy_prim(map, map->x[componente_conexe[i]]);
    }

    // Sortăm costurile în ordine crescătoare
    for (i = 0; i < cnt; i++) {
        int aux = costs[i];
        int j = i - 1;

        /* Mutăm elementele costs[0..i-1], care sunt mai mari decât aux, 
           la pozițiile lor următoare */
        while (j >= 0 && costs[j] > aux) {
            costs[j + 1] = costs[j];
            j = j - 1;
        }
        costs[j + 1] = aux;
    }

    FILE *OutputFile = fopen("tema3.out", "wt");
    if (!OutputFile) {
        fprintf(stderr, "Failed to open the file"); // Eroare dacă alocarea a eșuat
        exit(-1);
    }

    fprintf(OutputFile, "%d\n", cnt);

    // Scriem costurile în fișierul de ieșire
    for (i = 0; i < cnt; i++) {
        fprintf(OutputFile, "%d\n", costs[i]);
    }

    fclose(OutputFile);
    free(costs); // Eliberăm memoria alocată pentru costs
}
