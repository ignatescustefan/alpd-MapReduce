
#include <string>
#include <stdio.h>
#include <mpi/mpi.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <list>
#include <vector>
#include<algorithm>

#include "utils.h"

using namespace std;

int main(int argc,char **argv)
{
    int rank;
    int np;
    MPI_Request req;
    MPI_Status status;
    // cout<<argc<<"\n";
    // for (int i = 0; i < argc; i++)
    // {
    //     cout<<argv[i]<<" ";
    // }
    // cout<<"\n";
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    //auto var = readFile("/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/1.txt");
    Mapper("/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/3.txt");
    // if(rank==ROOT)
    // {
    //     //iau numele fisirelor din folderul din linia de comanda
    //     list<string> list=getFileNameFromDirectory(argv[1]);
    //    // showlist(list);
    //    // cout<<list.size();
    //     int i=0;
    //     int count=0;
    //     while(i<np )
    //     {
    //         string docName=list.front();
    //         if(i!=ROOT)
    //         {
    //             char *buff=stringToChar(docName);
    //             list.pop_front();
    //             MPI_Isend(buff,docName.length(),MPI_CHAR,i,99,MPI_COMM_WORLD,&req);
    //             cout<<"Radacina "<<ROOT<<" a trimis mesajul: "<<buff<<" procesului "<<i<<"\n";
    //         }
    //         //send
    //         i++;
    //         if(i>=np && !list.empty())
    //         {
    //             i=0;
    //         }
    //         count++;
    //     }
    // }
    // else
    // {
        
    //     char buff[MAX_VAL];
    //     MPI_Recv(buff,MAX_VAL,MPI_CHAR,ROOT,99,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    //             cout<<"Procesul "<<rank<<" a primit "<<buff<<" de la RADACINA\n";
    //     //di
    // }
    
    MPI_Finalize();
    return 0;
}