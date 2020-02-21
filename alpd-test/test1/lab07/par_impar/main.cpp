#include<iostream>
#include<mpi/mpi.h>

#define ROOT 0
#define NP 8

using namespace std;

void print(int *a, int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

int main(int argc,char **argv)
{
    int *a;
    int n;

    int rank;
    
    MPI_Status status;

    int sursa;
    int destinatie;
    int tag=99;

    int r;
    int v;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&n);

    if(rank == ROOT)
    {
        //aloca date si initializez date;
        a=new int[NP]{3,2,3,8,5,6,4,1};
        cout<<"Afisare vector initial:\n";
        print(a,NP);
    }

    //trimit cate un el din buff catre fiecare proces
    MPI_Scatter(a,1,MPI_INT,&r,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    int faza;
    for(faza=1;faza<=n;faza++)
    {
        //i = rankul procesului curent p_i
        if(faza % 2 == 1 && rank >= 0 && rank <= 2 * (n / 2) - 1)
        {
            if(rank % 2 == 0)
            {
                destinatie=rank+1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                cout<<"proces ["<<rank<<"] trimite valoarea: "<<r<<", procesului: ["<<destinatie<<"]\n";
                sursa=rank+1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);
                cout<<"proces ["<<rank<<"] primeste valoarea: "<<v<<", de la procesul: ["<<sursa<<"]\n";
                
                r=(v < r) ? v:r;
            }
            else
            {
                destinatie=rank-1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                cout<<"proces ["<<rank<<"] trimite valoarea: "<<r<<", procesului: ["<<destinatie<<"]\n";
                
                sursa=rank-1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);
                cout<<"proces ["<<rank<<"] primeste valoarea: "<<v<<", de la procesul: ["<<sursa<<"]\n";
                
                r=(v > r) ? v:r;
            } 
        }
        if(faza % 2 == 0 && rank >= 1 && rank <= 2*((n-1)/2))
        {
            if(rank%2==1)
            {
                destinatie=rank+1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                cout<<"proces ["<<rank<<"] trimite valoarea: "<<r<<", procesului: ["<<destinatie<<"]\n";
                
                sursa=rank+1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);
                cout<<"proces ["<<rank<<"] primeste valoarea: "<<v<<", de la procesul: ["<<sursa<<"]\n";
                
                r=(v < r) ? v:r;
            }
            else
            {
                destinatie=rank-1;
                MPI_Send(&r,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                cout<<"proces ["<<rank<<"] trimite valoarea: "<<r<<", procesului: ["<<destinatie<<"]\n";
                
                sursa=rank-1;
                MPI_Recv(&v,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);
                cout<<"proces ["<<rank<<"] primeste valoarea: "<<v<<", de la procesul: ["<<sursa<<"]\n";
                
                r=(v > r) ? v:r;
            }
        }
    }


    MPI_Gather(&r,1,MPI_INT,a,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    if(rank==ROOT)
    {
        cout<<"afisez a sortat:\n";
        print(a,n);
        delete[] a;
    }
    MPI_Finalize();
    return 0;
}