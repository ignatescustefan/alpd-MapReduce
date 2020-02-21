#include<iostream>
#include<math.h>
#include<mpi/mpi.h>

#define ROOT 4
#define NODES 9
using namespace std;

int main(int argc,char **argv)
{

    int rank;
    int np;
    char M[100];

    int sursa;
    int destinatie;
    MPI_Status status;
    int tag=99;
    
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    //
    MPI_Comm graph;
    
    int edges[]={4,6,4,7,8,4,6,2,3,0,6,3,1,5,2,2};
    int index[]={1,2,5,7,10,11,14,15,16};
    if( np!=NODES)
    {
        cout<<"Nu pot \n";
        MPI_Finalize();
        return 0;
    }
    else
    {
        MPI_Graph_create(MPI_COMM_WORLD,NODES,index,edges,0,&graph);
        int nrVecini;
        MPI_Graph_neighbors_count(graph,rank,&nrVecini);
        int *vecini=new int[nrVecini];
        MPI_Graph_neighbors(graph,rank,nrVecini,vecini);

        //verific rankul
        //radacina -> trimit catre copilasi
        if(rank==ROOT)
        {
            strcpy(M,"\'De la radacina \'");
            for(int i=0;i<nrVecini;i++)
            {
                destinatie=vecini[i];
                MPI_Send(M,strlen(M)+1,MPI_CHAR,destinatie,tag,graph);
                cout<<"Radacina ["<<rank<<"] am trimis mesajul "<<M<<" fiului :"<<destinatie<<"\n";
            }
        }
        else
        {
            //sunt pe frunze sau nod interior;
            if(nrVecini>1)//nod interior
            {
                //astep mesaj de la tata
                sursa=vecini[0];
                MPI_Recv(M,100,MPI_CHAR,sursa,tag,graph,&status);
                cout<<"NI ["<<rank<<"] am primit mesajul "<<M<<" de la tatal "<<sursa<<"\n";

                //trimit urmatorilor mostenitori
                for(int i=1;i<nrVecini;i++)
                {
                    destinatie=vecini[i];
                    MPI_Send(M,strlen(M)+1,MPI_CHAR,destinatie,tag,graph);
                    cout<<rank<<" am trimis mesajul "<<M<<" fiului :"<<destinatie<<"\n";
                }
            }
            else
            {
                //sunt frunza
                sursa=vecini[0];
                MPI_Recv(M,100,MPI_CHAR,sursa,tag,graph,&status);
                cout<<"Frunza ["<<rank<<"] am primit mesajul "<<M<<" de la tatal "<<sursa<<"\n";
            }
            
        }
              
    }

    MPI_Comm_free(&graph);
    MPI_Finalize();
    
    return 0;
}
