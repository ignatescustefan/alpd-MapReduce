#include<iostream>
#include<mpi/mpi.h>

#define ROOT 4
#define NOODES 9

using namespace std;

int main(int argc,char **argv)
{
    int rank;
    int np;

    MPI_Comm graph;

    int M;

    int sursa;
    int destinatie;
    int tag=99;

    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Comm_size(MPI_COMM_WORLD,&np);

    int edges[]={4,6,4,7,8,4,6,2,3,0,6,3,1,5,2,2};
    int index[]={1,2,5,7,10,11,14,15,16};
    if( np!=NOODES)
    {
        cout<<"Nu pot \n";
        MPI_Finalize();
        return 0;
    }
    else
    {
        MPI_Graph_create(MPI_COMM_WORLD,NOODES,index,edges,1,&graph);
        int nrVecini;
        MPI_Graph_neighbors_count(graph,rank,&nrVecini);
        //to do
        //aloc memorie pt vecini
        int *vecini=new int[nrVecini];
        MPI_Graph_neighbors(graph,rank,nrVecini,vecini);

        if(rank == ROOT) // radacina
        {   
            int calcul=0;
            int x;
            for(int i=0;i<nrVecini;i++)
            {
                sursa=vecini[i];
                MPI_Recv(&x,1,MPI_INT,sursa,tag,graph,&status);
                calcul+=x;
            }
            calcul+=rank;
            cout<<"Radacina ["<<rank<<"] a calculat "<<calcul<<"\n";
        }
        else
        {
            if(nrVecini>1) //nod interior
            {
                int calcul=0;
                int x;
                //primesc val de la fii
                for(int i=1;i<nrVecini;i++)
                {
                    sursa=vecini[i];

                    MPI_Recv(&x,1,MPI_INT,sursa,tag,graph,&status);

                    calcul+=x;
                }
                calcul+=rank;
                //trimit parintelui
                destinatie=vecini[0];
                M=calcul;
                MPI_Send(&M,1,MPI_INT,destinatie,tag,graph);
                cout<<"Nod I "<<rank<<" am trimis mesajul: "<<M<<" catre parintele "<<destinatie<<"\n";
         
            }
            else // frunza
            {
                //trimit parintelui valoare rank;
                destinatie=vecini[0];
                M=rank;
                MPI_Send(&M,1,MPI_INT,destinatie,tag,graph);
                cout<<"Frunza "<<rank<<" am trimis mesajul: "<<M<<" catre parintele "<<destinatie<<"\n";
            }
            
        }
        
    }
    MPI_Comm_free(&graph);
    MPI_Finalize();
}