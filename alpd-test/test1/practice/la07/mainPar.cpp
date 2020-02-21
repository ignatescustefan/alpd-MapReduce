#include<iostream>
#include<mpi/mpi.h>

#define ROOT 0
#define NP 8

using namespace std;

void print(int *a,int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

int main(int arc,char **argv)
{
    int *a;
    int rank;

    int n;
    
    int sursa;
    int destinatie;
    int r,v;
    int tag=99;

    MPI_Status status;

    MPI_Init(&arc,&argv);
    
    MPI_Comm_size(MPI_COMM_WORLD,&n);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank==ROOT)
    {
        a=new int[NP]{3,2,3,8,5,6,4,1};
        print(a,NP);
    }

    //trimit la toate procesele
    MPI_Scatter(a,1,MPI_INT,&r,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    int faza;
    for(faza=1;faza<=n;faza++)
    {
        int i=rank;
        if(faza%2==1 && i>=0 && i<=2*(n/2)-1)
        {
            if(i%2==0)
            {
                destinatie=i+1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                
                sursa=i+1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);

                //compar r<v?
                r= (r<v) ? r:v; 
            }
            else
            {
                destinatie=i-1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                
                sursa=i-1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);

                //compar r>v?
                r= (r>v) ? r:v; 
            }
        }
        if(faza%2==0 && i>=1 && i<=2*((n-1)/2))
        {
            if(i%2==1)
            {
                destinatie=i+1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                
                sursa=i+1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);

                //compar r<v?
                r= (r<v) ? r:v; 
            }
            else
            {
                destinatie=i-1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                
                sursa=i-1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);

                //compar r>v?
                r= (r>v) ? r:v; 
            }
        }
        
    }
    MPI_Gather(&r,1,MPI_INT,a,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    if(ROOT==rank)
    {
        cout<<"Sortat:\n";
        print(a,NP);
    }

    MPI_Finalize();
    return 0;
}