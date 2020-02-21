#include<iostream>
#include<mpi/mpi.h>

using namespace std;

int main(int argc,char **argv)
{
    int rank,size;

    int id_buff;
    int ndims=1;
    int started=0;
    int left,right;
    int leader;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int mdims[]{size};
    int periods[]={1};
    MPI_Comm comm;

    MPI_Cart_create(MPI_COMM_WORLD,ndims,mdims,periods,0,&comm);
    MPI_Cart_shift(comm,1,1,&left,&right);
    
    while (1)
    {
        if(!started)
        {
            MPI_Send(&rank,1,MPI_INT,right,1,comm);//tag=1;
            started=1;
        }
        else if(id_buff>rank)
        {
            MPI_Send(&id_buff,1,MPI_INT,right,1,comm);
        }
        else if(id_buff==rank)
        {
            //eu sunt lider
            leader=id_buff;
            MPI_Send(&leader,1,MPI_INT,right,2,comm);//tag=2
            break;
        }
        MPI_Recv(&id_buff,1,MPI_INT,left,MPI_ANY_TAG,comm,&status);
        cout<<"Procesul: "<<rank<<" primesc mesajul: "<<id_buff<<" de la vecinul: "<<left<<"\n";
        if (status.MPI_TAG==2)
        {
            leader=id_buff;
            cout<<"Leaderul este: "<<leader<<"\n";
            MPI_Send(&leader,1,MPI_INT,right,2,comm);
            break;
        }
        
    }
    MPI_Comm_free(&comm);
    MPI_Finalize();
    return 0;
}