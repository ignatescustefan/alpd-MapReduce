#include<iostream>
#include<mpi/mpi.h>

#define NNODES 9

using namespace std;

int main(int argc,char **argv)
{
    int rank;
    int np;
    MPI_Comm graph;
    int edges[]={1,4,5,6,0,6,4,7,8,5,6,8,0,2,7,0,3,7,0,1,3,2,4,5,8,2,3,7};
    int index[]={4,6,9,12,15,18,21,25,28};
    
    MPI_Init(&argc,&argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    int max_id=rank;
    int d=NNODES;

    if(np!=NNODES)
    {
        cout<<"Nu pot rula cu: "<<np<<" procese\n";
        MPI_Finalize();
    }
    else
    {
        //creez topologia
        MPI_Graph_create(MPI_COMM_WORLD,NNODES,index,edges,0,&graph);
        int noNeighbors;
        MPI_Graph_neighbors_count(graph,rank,&noNeighbors);
        int neighbors[noNeighbors];
        MPI_Graph_neighbors(graph,rank,noNeighbors,neighbors);
        int i,j;
        for(i=0;i<d;i++)
        {
            //d= diametrul grafului
            //cout<<"Rank "<<rank<<" : ";
            for(j=0;j<noNeighbors;j++)
            {
                //trimit maximul initial
                MPI_Send(&max_id,1,MPI_INT,neighbors[j],99,graph);
              //  cout<<neighbors[j]<<" ";
            }
            //cout<<"\n";
            for(j=0;j<noNeighbors;j++)
            {
                //primesc val de la vecini
                int id_buff;
                MPI_Recv(&id_buff,1,MPI_INT,neighbors[j],99,graph,MPI_STATUS_IGNORE);
                //verific daca ce am primit e mai mare decat max_id
                if(max_id<id_buff)
                {
                    //actualizez maximul
                    max_id=id_buff;
                }
            }
        }
        cout<<"Procesul curent: "<<rank<<" liderul: "<<max_id<<"\n";
    }
    MPI_Finalize();
    return 0;
}