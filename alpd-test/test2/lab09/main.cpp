#include<iostream>
#include<mpi/mpi.h>
#define N 3

using namespace std;

void print(double * a, int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}
void print2D(double **a,int n)
{
    for(int i=0;i<n;i++)
    {
        print(a[i],n);
    }
}
int main(int argc,char **argv)
{

    int rank;
    int np;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    if(np!=N*N)
    {
        cout<<"NU se poate crea topoligia\n";
        MPI_Finalize();
    }
    else
    {
        
        double a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}; 
        double b[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        int dims[2]={N,N};
        int periods[2]={1,1};
        int sursa,destinatie;
        int cords[2];//coord proces curent in topologia carteziana;
        MPI_Comm cart_comm;

        MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,0,&cart_comm);
        MPI_Comm_rank(cart_comm,&rank);
        MPI_Cart_coords(cart_comm,rank,2,cords);

        int i=cords[0],j=cords[1];

        //initializez m1 si m2 pt fiecare proces
        double m1=a[i][j],m2=b[i][j],m3=0.0,v;

        //faza de aliniere
        int k;
        if(i>0)
        {
            MPI_Cart_shift(cart_comm,1,-i,&sursa,&destinatie);
            MPI_Sendrecv(&m1,1,MPI_DOUBLE,destinatie,99,&v,1,MPI_DOUBLE,sursa,99,cart_comm,MPI_STATUS_IGNORE);
            m1=v;
        }
        if(j>0)
        {
            MPI_Cart_shift(cart_comm,0,-j,&sursa,&destinatie);
            MPI_Sendrecv(&m2,1,MPI_DOUBLE,destinatie,99,&v,1,MPI_DOUBLE,sursa,99,cart_comm,MPI_STATUS_IGNORE);
            m2=v;
        }
        cout<<"Rank: "<<rank<<" -> "<<m1<<","<<m2<<"\n";
        //faza de inmultire
        for(k=0;k<N-1;k++)
        {
            m3+=m1*m2;
            MPI_Cart_shift(cart_comm,1,-1,&sursa,&destinatie);
            MPI_Sendrecv(&m1,1,MPI_DOUBLE,destinatie,99,&v,1,MPI_DOUBLE,sursa,99,cart_comm,MPI_STATUS_IGNORE);

            m1=v;
            MPI_Cart_shift(cart_comm,0,-1,&sursa,&destinatie);
            MPI_Sendrecv(&m2,1,MPI_DOUBLE,destinatie,99,&v,1,MPI_DOUBLE,sursa,99,cart_comm,MPI_STATUS_IGNORE);
            m2=v;
        }
        m3+=m1*m2;
        cout<<"Rank: "<<rank<<" calculeaza:"<<m3<<"\n";
        MPI_Finalize();
    }
    return 0;
}