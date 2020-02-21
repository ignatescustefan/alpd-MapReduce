#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi/mpi.h>

//radacina
#define ROOT 4 

int main(int argc,char **argv)
{
    int rank;
    int p;
    int sursa;
    int destinatie;
    int tag = 99;
    char M[100] = "Hello world";
    MPI_Status status;
    MPI_Comm graph;

    int index[] = {1, 2, 5, 7, 10, 11, 14, 15, 16};
    int nrNod = 9;
    int edges[] = {4, 6, 4, 7, 8, 4, 6, 2, 3, 0, 6, 3, 1, 5, 2, 2};

    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //creez graf
    if(nrNod!=p)
    {
        printf("Nu pot creea o topologice cu %d procese, am nevoi de: %d\n", p, nrNod);
        MPI_Finalize();
        return 0;
    }
    else
    {
        MPI_Graph_create(MPI_COMM_WORLD, nrNod, index, edges, 0, &graph);

        //aflu nr de vecini;
        int ncount;
        MPI_Graph_neighbors_count(graph, rank, &ncount);
        //aflu vecinii

        int *vecini=(int*) malloc(ncount *sizeof(int));
        if(vecini==0)
        {
            printf("Eroare la alocare\n");
            MPI_Comm_free(&graph);
            MPI_Finalize();
            exit(0);
        }
        MPI_Graph_neighbors(graph, rank, ncount, vecini);

        if(rank == ROOT)
        {
            for(int i=0;i<ncount;i++)
            {
                destinatie=vecini[i];
                MPI_Send(M, (int)strlen(M) + 1, MPI_CHAR, destinatie, tag, graph);
                printf("Radacina [%d] am trimis mesajul: %s ,catre [%d]\n",rank,M,destinatie);
            }
        }
        else
        {
            MPI_Recv(M,100,MPI_CHAR,MPI_ANY_SOURCE, tag, graph, &status);

            printf("nod [%d] am primit mesajul: %s ,de la  [%d]\n",rank,M,status.MPI_SOURCE);

            if(ncount > 1)
            {
                for(int i=0;i<ncount;i++)
            {
                    destinatie=vecini[i];
                    if(destinatie!=status.MPI_SOURCE)
                    {
                        MPI_Send(M, (int)strlen(M) + 1, MPI_CHAR, destinatie, tag, graph);
                        printf("Nod [%d] am trimis mesajul: %s ,catre [%d]\n",rank,M,destinatie);
                    }
                }
            }
        }
        if(vecini)
        {
            free(vecini);
        }
        MPI_Comm_free(&graph);
    }
    MPI_Finalize();
    return 0;
}