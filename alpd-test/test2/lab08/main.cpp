#include<iostream>
#include<mpi/mpi.h>
#include<math.h>

#define ROOT 0
#define N 8

using namespace std;

void print(int *a,int n)
{
    for(int i=0;i<n;++i)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

int main(int argc,char ** argv)
{
    int rank;
    int np;

    int *a;
    MPI_Status status;
    MPI_Request req;
    int r,v;
    int tag=99;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    
    if(np!=N)
    {
        cout<<"Nu se poate rula\n";
        MPI_Finalize();
    }
    else
    {
        if(rank==ROOT)//radacina
        {
            //aloc memorie si afisez vector;
            a=new int[N]{10,20,5,9,3,8,12,14};
            cout<<"Vector initial: ";
            print(a,N);
        }
        MPI_Scatter(a,1,MPI_INT,&r,1,MPI_INT,ROOT,MPI_COMM_WORLD);

        int f,k,m;
        m=log2(np);
        for(f=1;f<=m;f++)
        {
            for(k=f-1;k>=0;k--)      
            {
                int dest=rank^(1<<k);//rank XOR 2 la k
                int sursa=rank^(1<<k);

                //Trimit neblocant

                MPI_Isend(&r,1,MPI_INT,dest,tag,MPI_COMM_WORLD,&req);
                //primesc
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);

                int bitT,bitK;
                bitT=(rank>>f)&1;
                bitK=(rank>>k)&1;
                if(bitT==bitK)
                {
                    r=max(v,r);
                }
                else
                {
                    r=min(v,r);
                }
                
            }
        }
        MPI_Gather(&r,1,MPI_INT,a,1,MPI_INT,ROOT,MPI_COMM_WORLD);
        if(rank==ROOT)
        {
            cout<<"Tablou sortat: ";
            print(a,N);
            delete[] a;
        }
    }
    

    MPI_Finalize();

    return 0;
}