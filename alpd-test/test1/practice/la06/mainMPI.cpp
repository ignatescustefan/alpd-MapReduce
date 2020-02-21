#include<iostream>
#include<mpi/mpi.h>
#include<math.h>

#define ROOT 1
#define NP 16

using namespace std;

void print(int *a,int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}

int *a=new int[NP]{0,0,0,0,0,0,0,0,3,2,1,6,5,4,2,9};
int *b=new int[NP]{0};

int main(int argc,char **argv)
{
    int rank;
    int np;

    int sursa;
    int destinatie;
    int val;
    int valD,valS;
    int tag=99;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //fac comprimare
    if(np!=NP)
    {
        cout<<"Numarul de procese = cu 16!\n";
        delete[] a;
        delete[] b;
        MPI_Finalize();
    }
    else
    {
        MPI_Scatter(a,1,MPI_INT,&val,1,MPI_INT,ROOT,MPI_COMM_WORLD);
    
        if(rank==ROOT)
        {
            cout<<"Afisez A\n";
            print(a,NP);
        }


        if(rank>=np/2)
        {
            //frunze
            destinatie=rank/2;

            MPI_Send(&val,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
             cout<<"Frunza ["<<rank<<"] a trimis mesajul "<<val<<" catre parintele ["<<destinatie<<"]\n";
        }
        else if (rank!=0)
        {
            int sSt=2*rank;
            int sDr=2*rank+1;

            MPI_Recv(&valS,1,MPI_INT,sSt,tag,MPI_COMM_WORLD,&status);
              cout<<"Nod ["<<rank<<"] a primit mesajul "<<valS<<" de la fiul stanga ["<<sSt<<"]\n";
            MPI_Recv(&valD,1,MPI_INT,sDr,tag,MPI_COMM_WORLD,&status);
            cout<<"Nod ["<<rank<<"] a primit mesajul "<<valD<<" de la fiul dreapta ["<<sDr<<"]\n";
            
            int rez=valD+valS;
             cout<<"Nod ["<<rank<<"] a calculat: "<<rez<<"\n";
            a[rank]=rez;
            val=rez;
            //trimit rezultatul parintelui daca e diferit de root
            if(rank!=ROOT)
            {
                destinatie=rank/2;
                MPI_Send(&val,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                   cout<<"Nodul ["<<rank<<"] a trimis mesajul "<<val<<" catre parintele ["<<destinatie<<"]\n";
            }

        }
        MPI_Gather(&val,1,MPI_INT,a,1,MPI_INT,ROOT,MPI_COMM_WORLD);

        if(rank==ROOT)
        {
            cout<<"Tabloul dupa comprimare:\n";
            print(a,NP);
        }
        //calcul prefixe
        if(ROOT==rank)
        {
            int destinatieS=rank*2;
            int destinatieD=rank*2+1;

            //trimit
            MPI_Send(&val,1,MPI_INT,destinatieS,tag,MPI_COMM_WORLD);
            MPI_Send(&val,1,MPI_INT,destinatieD,tag,MPI_COMM_WORLD);
            
            cout<<"Radacina a trimis val: "<<val<<" fiilor: "<<destinatieS<<", "<<destinatieD<<"\n";
        }
        else if(rank!=0)
        {
            //nod interion //trebuie sa primesc
            sursa=rank/2;
            int valPrimita;
            MPI_Recv(&valPrimita,1,MPI_INT,sursa,tag,MPI_COMM_WORLD,&status);
            cout<<"Nod["<<rank<<"] am primit mesajul :"<<valPrimita<<" de la nodul parinte "<<sursa<<"\n";

            //verific rank%2
            if(rank%2==0)
            {
                int sursaDreapta=rank+1;
                MPI_Recv(&valD,1,MPI_INT,sursaDreapta,tag,MPI_COMM_WORLD,&status);
                val=valPrimita-valD;
            }
            else
            {
                destinatie=rank-1;
                MPI_Send(&val,1,MPI_INT,destinatie,tag,MPI_COMM_WORLD);
                val=valPrimita;//valoare pe nod;
            }
            if(rank<np/2)//nu e frunza -> trimit mai departe;
            {
                int destinatieS=rank*2;
                int destinatieD=rank*2+1;
                //trimit
                MPI_Send(&val,1,MPI_INT,destinatieS,tag,MPI_COMM_WORLD);
                MPI_Send(&val,1,MPI_INT,destinatieD,tag,MPI_COMM_WORLD);

                cout<<"Nodul "<<rank<<" a trimis val: "<<val<<" fiilor: "<<destinatieS<<", "<<destinatieD<<"\n";
            }
        }
        MPI_Gather(&val,1,MPI_INT,b,1,MPI_INT,ROOT,MPI_COMM_WORLD);
        if(rank==ROOT)
        {
            cout<<"Tablou b dupa calcul prefixe: \n";
            print(b,NP);
            delete[] a;
            delete[] b;
        }
    }
    
    MPI_Finalize();

    return 0;
}