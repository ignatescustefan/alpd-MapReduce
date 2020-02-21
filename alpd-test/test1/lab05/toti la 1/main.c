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
    int M;
    int calculMesaj;
    int sumaVecini;
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

        sumaVecini=0;
        for(int i=0;i<ncount;i++)
        {
            sumaVecini+=vecini[i];
        }

        if(rank == ROOT)
        {
            calculMesaj=0;
            for(int i=0;i<ncount;i++)
            {
                sursa=vecini[i];
                MPI_Recv(&M, 1, MPI_INT, sursa, tag, graph, &status);
                printf("Radacina [%d] am primit mesajul: %d ,de la fiul [%d]\n",rank, M, sursa);
                calculMesaj+=M;
            }
            printf("Radacina [%d] Calcul final: %d\n",rank,calculMesaj);
        }
        else
        {
            if(ncount == 1)
            {
                //frunza -> trimit mesaj;
                destinatie=vecini[0];
                MPI_Send(&rank, 1, MPI_INT, destinatie, tag, graph);
                printf("Frunza [%d] am trimis mesajul :%d catre parintele [%d]\n", rank, rank, destinatie);
            }
            else
            {
                //calcul mesaj;
                calculMesaj=0;
                for(int i=1;i<ncount;i++)
                {
                    sursa=vecini[i];
                    MPI_Recv(&M, 1, MPI_INT, sursa, tag, graph, &status);
                    calculMesaj+=M;
                }
                //trimit mesajul parintelui;
                destinatie=vecini[0];
                //adun nodul curent
                calculMesaj+=rank;
                MPI_Send(&calculMesaj,1,MPI_INT,destinatie,tag,graph);
                printf("Nodul [%d] am trimis mesajul %d ,parintelui :%d\n", rank, calculMesaj, destinatie);
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