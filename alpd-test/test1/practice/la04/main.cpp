#include<iostream>
#include<math.h>
#include<mpi/mpi.h>

using namespace std;

int main(int argc,char **argv)
{
    int rank;
    int np;
    MPI_Status status;
    int sursa;
    int destinatie;
    int tag=99;
    int masca;
    char M[100];
    int k;
    int id;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    if(np & (np-1)!=0)
    {
        cout<<"Nr de procese ^2\n";
        exit(EXIT_FAILURE);
        MPI_Finalize();
    }
    if(rank==0)
    {
        //parinte -> pun mesaj
        strcpy(M,"Mesaj");
    }
    int d=log2(np);//dim hipercub
    masca=(1<<d)-1;
    id=rank;

    for(k=d-1;k>=0;k--)
    {
        int aux=1<<k;
        masca=masca^aux;
        if((id&masca)==0)
        {
            if((id&aux)==0)
            {
                destinatie=id^aux;
                MPI_Send(M,strlen(M)+1,MPI_CHAR,destinatie,tag,MPI_COMM_WORLD);
                cout<<rank<<" am trimis mesajul "<<M<<" la procesul: "<<destinatie<<"\n";
            }
            else
            {
                sursa=id^aux;
                MPI_Recv(M,100,MPI_CHAR,sursa,tag,MPI_COMM_WORLD,&status);
                cout<<rank<<" am primit mesajul: "<<M<<" de la procesul: "<<sursa<<"\n";
            }
        }
    }
    MPI_Finalize();
    return 0;
}