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

void Test()
{
    Reducer(DIRECTORY_OUT);
}

int main(int argc,char **argv)
{
    int rank;
    int np;

    MPI_Request req;
    MPI_Status status;

    bool isMapped=false;
    bool isReduce=false;

    
    int *slaves;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);

    
    if(rank==ROOT)
    {
        //iau numele fisirelor din folderul din linia de comanda
        list<string> list=getFileNameFromDirectory(argv[1]);
        slaves=initSlaves(np,ROOT);
        // showlist(list);
        // cout<<list.size();
        int i=0;
        int count=0;
        while (!isMapped)
        {
            //trebuie sa trimit taskuri de mapare
            //eli        
            while(i<np )
            {
                string docName=list.front();
                if(i!=ROOT)
                {
                    char *buff=stringToChar(docName);
                    list.pop_front();
                    MPI_Isend(buff,docName.length()+1,MPI_CHAR,i,MAPPER_TAG,MPI_COMM_WORLD,&req);
                    cout<<"Radacina "<<ROOT<<" a trimis mesajul: "<<buff<<" procesului "<<i<<"\n";
                }
                //send
                i++;
                if(i>=np && !list.empty())
                {
                    i=0;
                }
                count++;
            }
            if(list.empty())
            {
                isMapped=true;
            }
        }
        //char buff[MAX_VAL];
       
        // for(int i=0;i<np;i++)
        // {
        //     if(slaves[i]!=ROOT)
        //     {
        //         MPI_Recv(buff,MAX_VAL,MPI_CHAR,slaves[i],MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        //         cout<<"Radacina am primit "<<buff<<" de la procesul"<<status.MPI_SOURCE<<"cu tagul:"<<status.MPI_TAG<<"\n";
        //     }
        // }
        
        //TRIMITE SLAVE SA REZOLVE SI ASTEAPTA RASPUNS;
        Reducer(DIRECTORY_OUT);
    }
    else
    {
        if(!isMapped)
        {
            char buff[MAX_VAL];
            MPI_Recv(buff,MAX_VAL,MPI_CHAR,ROOT,MAPPER_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            char filename[MAX_VAL];
            strcpy(filename,argv[1]);
            strcat(filename,buff);
            
            cout<<rank<<" "<<filename<<"\n";
            string mapper=Mapper(filename);
            char* response=stringToChar(mapper);
          //  cout<<"Procesul "<<rank<<" a primit "<<buff<<" de la RADACINA, si mapeaza in fisierul="<<filename<<" in "<<mapper<<"\n";
            //trimit la master ca am terminat cu de procesat fisierul;
          //  MPI_Send(response,strlen(response)+1,MPI_CHAR,ROOT,END_MAP_TAG,MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}