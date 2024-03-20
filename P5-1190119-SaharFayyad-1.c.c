#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int MAX = 0;
int re = 0;

struct vertix {
    int destination;
    int dist;
    char town[200];
    struct vertix *next;
};

struct adjlist {
    struct vertix *head;
};

struct graph {
    int id;
    struct adjlist *array;
};

struct dijkstra_solve {
    int road;
    char *town;
    int dest;
};

struct graph *graph;

struct vertix *newvertix(int id2, int des, char city[MAX]);

struct graph *creatgraph(int v);

// Adds an edge to an undirected graph
void addEdge(struct graph *graph, int src, int dest, int id2, char city[MAX]);

int minumum(struct dijkstra_solve *dest[], bool set[]);

void dijkstra(struct graph *g, struct dijkstra_solve *dist[MAX], int start, int end);

void printSolution(struct dijkstra_solve *dist[], int start, int end);

void fprintSolution(FILE *file, struct dijkstra_solve *dist[], int start, int end);

void printGraph(struct graph *graph);

int max(int id1,int id2);

void read();


int main() {

    FILE *file = fopen("segments.txt", "r");
    char lines[4000];
    char id1[40] ,id2[40];
    char *token;

    while(fgets(lines,sizeof(lines),file)){
        token = strtok(lines," ");
        strcpy(id1,token);

        token = strtok(NULL," ");
        strcpy(id2,token);

        MAX = max(atoi(id1),atoi(id2)) + 1 ;

    }

    graph = creatgraph(MAX);
    struct dijkstra_solve *dest[MAX];
    FILE *out;

    int s, e;

    printf("-----Dijkstra Algorithm Menu----- \n");
    printf("1.Read \n");
    printf("2.Enter 2 points to compute the shortest path between them, Then if you want to save the file \n");
    printf("3.Exit \n");


    while (1) {

        printf("\nEnter An Operation\n");
        int op;
        scanf("%d", &op);

        switch (op) {
            case 1:
                if (re == 0) {
                    read();
                    //    printGraph(graph);
                    printf("Done\n");
                } else {
                    printf("you already read it!!!\n");
                }
                break;
            case 2:
                if (re == 1) {
                    printf("choose the start point or the end point -1 to exit the program\n");

                    while (1) {
                        printf("enter the start point:");
                        scanf("%d", &s);

                        printf("\nenter the second point:");
                        scanf("%d", &e);

                        if(s > MAX || e > MAX)
                        {
                            printf("choose to the max number only !!!!\n");
                            continue;
                        }

                        if (s == -1 || e == -1) {
                            exit(0);
                        } else {
                            char o;
                            dijkstra(graph, dest, s, e);
                            printf("\nDO YOU WANT TO SAVE TO FILE?\nPRESS Y IF Yes AND N FOR No\n");
                            scanf("%c", &o);
                            scanf("%c", &o);
                            if (o == 'Y' || o == 'y') {
                                out = fopen("route.txt", "a");
                                printf("Done\n");
                                fprintf(out, "THE SHORT PATH DISTENCE FROM %d TO %d WITH COST %d:", s, e, dest[e]->dest);
                                fprintSolution(out, dest, s, e);
                                fprintf(out,"\n");
                                fclose(out);
                            }
                        }
                    }
                } else {
                    printf("read the file first!!!");
                }

                break;


            case 3:
                exit(1);
                break;

            default:
                printf("choose from 1 to 3 only !!");
                break;
        }
    }
    return 0;
}

struct vertix *newvertix(int id2, int des, char city[MAX]) {
    struct vertix *newNode = (struct vertix *) malloc(sizeof(struct vertix));
    newNode->dist = des;
    newNode->destination = id2;
    strcpy(newNode->town, city);
    newNode->next = NULL;
    return newNode;
}

struct graph *creatgraph(int v) {
    struct graph *graph = (struct graph *) malloc(sizeof(struct graph));
    graph->id = v;

    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array = (struct adjlist *) malloc(v * sizeof(struct adjlist));

    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < v; ++i)
        graph->array[i].head = NULL;

    return graph;
}


// Adds an edge to an undirected graph
void addEdge(struct graph *graph, int src, int dest, int id2, char city[MAX]) {
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning

    struct vertix *newNode = newvertix(dest, id2, city);

    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from
    // dest to src also
    newNode = newvertix(src, id2, city);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;

    //return graph;
}

int minumum(struct dijkstra_solve *dest[], bool set[]) {
    int min = INT_MAX, min_index = -1;
    //int i;
    for (int i = 0; i < MAX; i++)
        if (set[i] == false && dest[i]->dest <= min)
            min = dest[i]->dest, min_index = i;

    return min_index;
}

//print the shortest path
void printSolution(struct dijkstra_solve *dist[], int start, int end) {
    if (dist[end]->dest == INT_MAX || dist[end]->dest < 0 ) {
        printf("no road between start and end\n");
        return;
    }
    if (dist[end]->road != start) {
        printSolution(dist, start, dist[end]->road);
    }
    printf("->%s ", dist[end]->town);

}

//print the shortest path to file
void fprintSolution(FILE *file, struct dijkstra_solve *dist[], int start, int end) {
    if (dist[end]->dest == INT_MAX  || dist[end]->dest < 0) {
        fprintf(file, "no road between start and end\n");
        return;
    }
    if (dist[end]->road != start) {
        fprintSolution(file, dist, start, dist[end]->road);
    }
    fprintf(file, "->%s ", dist[end]->town);
}

//algorthim to edit the path
void dijkstra(struct graph *g, struct dijkstra_solve *dist[MAX], int start, int end) {

    bool set[MAX];
    for (int j = 0; j < MAX; j++) {
        dist[j] = malloc(sizeof(struct dijkstra_solve));
        dist[j]->dest = INT_MAX;
        set[j] = false;
    }

    dist[start]->dest = 0;

    for (int count = 0; count < MAX - 1; count++) {

        int u = minumum(dist, set);
        if (u == -1)
            break;
        set[u] = true;
        for (struct vertix *temp = g->array[u].head; temp != NULL; temp = temp->next) {
            if (!set[temp->destination] && dist[u]->dest + temp->dist < dist[temp->destination]->dest) {
                dist[temp->destination]->dest = dist[u]->dest + temp->dist;
                dist[temp->destination]->road = u;
                dist[temp->destination]->town = temp->town;
            }
        }
    }
    printf("\nTHE SHORT PATH DISTENCE FROM %d TO %d WITH COST %d:", start, end, dist[end]->dest);
    printSolution(dist, start, end);
}

//print the graph
void printGraph(struct graph *graph) {
    int v;
    for (v = 0; v <= MAX; ++v) {
        struct vertix *pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n ", v);
        while (pCrawl != NULL) {
            printf("the distance is :%d and the city is :%s ||", pCrawl->destination, pCrawl->town);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

int max(int id1,int id2){
    if(id1 > id2){
        return id1;
    }
    else if(id2 > id1){
        return id2;
    }
    else
    {
        return id1;
    }
}

//read the file
void read() {
    FILE *file = fopen("segments.txt", "r");
    char a[900], b[900], c[900], d[900], e[1000], l[800], *token;

    while (!feof(file)) {
        fgets(a, sizeof(a), file);
        token = strtok(a, " ");
        strcpy(b, token);

        token = strtok(NULL, " ");//1
        strcpy(c, token);

        token = strtok(NULL, " ");//2
        strcpy(e, token);

        token = strtok(NULL, " ");//3
        strcpy(l, token);
        while (atoi(l) == 0) {
            strcat(e, " ");
            strcat(e, token);
            token = strtok(NULL, " ");
            strcpy(l, token);
        }
        //printf("%d %d %s %d\n",atoi(b),atoi(c),e,atoi(l));
        addEdge(graph, atoi(b), atoi(c), atoi(l), e);
    }
    fclose(file);
    re++;
}