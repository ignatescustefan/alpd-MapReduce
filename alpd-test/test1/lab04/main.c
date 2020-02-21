#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<mpi/mpi.h>

int mylog2(int x)
{
    int l=0;
    while (x !=1)
    {
        x>>= 1;
        l++;
    }
    return l;   
}

int main(int argc, char* argv[])
{
    
    int rank;
    int p;
    int sursa;
    int destinatiie;
    int tag=99;
    char M[100]="hello";
    unsigned int d;
    unsigned int id;
    unsigned int masca;
    int k;
    unsigned doi_la_k;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    //verific nr de procese

    if(p & (p - 1) != 0)
    {
        if(rank == 0)
        {
            fprintf(stderr,"nr de procese ^2");
        }
        MPI_Finalize();
        exit(0);
    }
    if(rank==0)
    {
        strcpy(M,"Hello world!");
    }

    d=(unsigned int)mylog2(p);
    masca=(unsigned int)(p-1);
    id=(unsigned int) rank;

    for(k=d-1;k>=0;k--)
    {
        //printf("k = %d\n",k);
        doi_la_k=1u<<k;
       // printf("doi_la_k = %d\n",doi_la_k);
        masca=masca^doi_la_k;
      //  printf("id = %d/n",id);
       // printf("masca = %d\n",masca);
        if((id & masca) == 0)
        {
            if( (id & doi_la_k) == 0)
            {
                destinatiie=id ^ doi_la_k;
        
                MPI_Send(M,(int)(strlen(M)+1),MPI_CHAR,destinatiie,tag,MPI_COMM_WORLD);
                printf("[%d] Am trimis mesajul: %s , la procesul [%d]\n",rank,M,destinatiie);
            }
            else
            {
                //printf("bula");
                sursa=id^doi_la_k;
                MPI_Recv(M,100,MPI_CHAR,sursa,tag,MPI_COMM_WORLD,&status);
                printf("[%d] Am primit mesajul: %s ,de la procesul [%d]\n",rank,M,sursa);
            }
        }
    }
    MPI_Finalize();
    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <mpi/mpi.h>

// // functie care calculeaza logaritmul in baza 2 dintr-un numar presupus putere a lui 2
// int mylog2(int x)
// {
//     int l = 0;
//     while (x != 1)
//     {
//         x >>= 1;
//         ++l;
//     }
//     return l;
// }

// int main(int argc, char* argv[])
// {
//     int rank; // rank-ul procesului curent
//     int p; // numarul de procese
//     int sursa; // rank-ul procesului sender
//     int destinatie; // rank-ul procesului reciever
//     int eticheta = 99; // eticheta pentru mesaje
//     char M[100] = "Hello world!"; // sirul de caractere ce alcatuiesc mesajul
//     unsigned int d; // dimensiunea hipercubului
//     unsigned int id; // identificatorul unitatii de procesare
//     unsigned int masca; // masca de biti
//     int k; // etapa curenta
//     unsigned int doi_la_k;
//     MPI_Status status; // starea de return pentru operatia de recieve

//     // pornire MPI
//     MPI_Init(&argc, &argv);

//     // aflam rank-ul pentru procesul curent
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     // aflam numarul de procese
//     MPI_Comm_size(MPI_COMM_WORLD, &p);

//     // testam daca numarul de procese este putere a lui 2
//     if (p & (p - 1) != 0)
//     {
//         if (rank == 0)
//         {
//             fprintf(stderr, "Numarul de procese trebuie sa fie putere a lui 2!\n");
//         }
//         MPI_Finalize();
//         exit(0);
//     }

//     if (rank == 0)
//     {
//         strcpy(M, "Hello world!");
//     }

//     d = (unsigned int)mylog2(p); // dimensiunea hipercubului
//     masca = (unsigned int)(p - 1); // masca = 2 ^ d - 1
//     id = (unsigned int)rank;

//     for (k = d - 1; k >= 0; --k)
//     {
//         doi_la_k = 1u << k;
//         masca ^= doi_la_k;

//         if ((id & masca) == 0)
//         {
//             if ((id & doi_la_k) == 0)
//             {
//                 destinatie = id ^ doi_la_k;

//                 // trimite M la destinatie
//                 MPI_Send(M, (int)(strlen(M) + 1), MPI_CHAR, destinatie, eticheta, MPI_COMM_WORLD);
//                 printf("[Proces %d] Am trimis mesajul \"%s\" catre destinatia %d.\n", rank, M, destinatie);
//             }
//             else
//             {
//                 sursa = id ^ doi_la_k;

//                 // receptioneaza M de la sursa
//                 MPI_Recv(M, 100, MPI_CHAR, sursa, eticheta, MPI_COMM_WORLD, &status);
//                 printf("[Proces %d] Am primit mesajul \"%s\" de la sursa %d.\n", rank, M, sursa);
//             }
//         }
//     }

//     // oprim MPI
//     MPI_Finalize();

//     return 0;
// }