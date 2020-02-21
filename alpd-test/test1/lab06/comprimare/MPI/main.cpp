#include<iostream>
#include<mpi/mpi.h>

#define ROOT 1
#define NP 16

using namespace std;

void print(int *a,int dim)
{
    for(int i=0;i<dim;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}
 int *a=new int[NP]{0,0,0,0,0,0,0,0,3,2,1,6,5,4,2,9};
int main(int argc,char **argv)
{
    int rank;
    int np;
    MPI_Status status;
    int sursaStanga,sursaDreapta;
    int destinatie;
    int tag=99;
    int valS,valD,rezultatCalculat;
    int val;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    if(rank==ROOT)
    {
        
        //afisez
       
        cout<<"Tabloul: \n";
        print(a,NP);
    }
    if(rank == 0 && np != NP)
    {
        cout<<"Numarul de procese = cu 16!\n";
        delete[] a;
        MPI_Finalize();
    }
    //trimit valorile initiale tuturor proceselor;
    MPI_Scatter(a,1,MPI_INT,&val,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    if(rank>=np/2) //frunze
    {
        destinatie=rank/2;//parintele
        MPI_Send(&val,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
        cout<<"Frunza ["<<rank<<"] a trimis mesajul "<<val<<" catre parintele ["<<destinatie<<"]\n";
    }
    else if(rank != 0)//nod interior
    {
        sursaDreapta=2*rank+1;
        sursaStanga=2*rank;

        MPI_Recv(&valS,1,MPI_INT,sursaStanga,tag,MPI_COMM_WORLD,&status);
        cout<<"Nod ["<<rank<<"] a primit mesajul "<<valS<<" de la fiul stanga ["<<sursaStanga<<"]\n";
       
        MPI_Recv(&valD,1,MPI_INT,sursaDreapta,tag,MPI_COMM_WORLD,&status);
        cout<<"Nod ["<<rank<<"] a primit mesajul "<<valD<<" de la fiul dreapta ["<<sursaDreapta<<"]\n";
        
        //calcul
        rezultatCalculat=valD+valS;
        
        cout<<"Nod ["<<rank<<"] a calculat: "<<rezultatCalculat<<"\n";
        a[rank] = rezultatCalculat;;
        val = rezultatCalculat;
        
        if(rank!=ROOT) //daca nu e radacina trimit la ceilalti
        {
            destinatie=rank/2;
            MPI_Send(&val,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
            cout<<"Nodul ["<<rank<<"] a trimis mesajul "<<val<<" catre parintele ["<<destinatie<<"]\n";
        }

    }
        MPI_Gather(&val, 1, MPI_INT, a, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(rank==ROOT)
    {
        cout<<"Tabloul dupa comprimare: \n";
        print(a,NP);
        delete[] a;
    }

    MPI_Finalize();
    return 0;
}
