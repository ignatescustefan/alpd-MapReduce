#include<iostream>
#include<mpi/mpi.h>

#define N 2

using namespace std;

void print(double* a,int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

void print2D(double** a, int n)
{
    for(int i=0;i<n;i++)
    {
        print(a[i],n);
    }
}

double a[2][2]={{6, 3},{20, -2}};
double b[2]={9,24};

int main(int argc,char ** argv)
{
    int rank;
    int np;
    double abuff[3];
    double bbuff;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    if(np!=N)
    {
        cout<<"Nu pot rezolva ec cu "<<np<<"procese\n";
        MPI_Finalize();
        return 0;
    }
    else
    {
        int k;
        for(k=0;k<N;k++)
        {
            if(rank==k)
            {
                //normalizez matricea
                for(int j=k+1;j<N;j++)
                {
                    a[k][j]/=a[k][k];
                }
                b[k]/=a[k][k];
                a[k][k]=1;
                //salvez linia "rank" din A in abuff, b[rank] -> bbuff fac Bcast;
                for(int j=0;j<N;j++)
                {
                    abuff[j]=a[rank][j];
                }
                bbuff=b[rank];
            }
            MPI_Bcast(abuff,N,MPI_DOUBLE,k,MPI_COMM_WORLD);
            MPI_Bcast(&bbuff,1,MPI_DOUBLE,k,MPI_COMM_WORLD);
            if(rank>k)
            {
                int i=rank,j;
                for(j=k+1;j<N;j++)
                {
                    a[i][j]-=a[i][k]*abuff[j];
                }
                b[i]-=a[i][k]*bbuff;
                a[rank][k]=0;
            }
        }
        cout<<"Procesul "<<rank<<" : ";
        print(a[rank],N);
        cout<<"b "<<b[rank]<<"\n";
    }
    MPI_Finalize();
    return 0;
}