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
    int st=1;
    char message[]="Gata maparea";
    char buff[MAX_VAL];

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
        int noFiles=list.size();
        int noMappeFile;
        slaves=initSlaves(np,ROOT); 
        //showlist(list);
        //cout<<list.size();
        int i=0;
        //faza de mapare
        //golesc folderul de mapare
        deleteFilesFromDirectory(DIRECTORY_MAPP);
        deleteFilesFromDirectory(argv[2]);
        while (!isMapped)
        {
            //trebuie sa trimit taskuri de mapare       
            while(i<np )
            {
                string docName=list.front();
                if(i!=ROOT)
                {
                    char buff[MAX_VAL];
                    char *temp=stringToChar(docName);
                    strcpy(buff,temp);
                    list.pop_front();
                    //send
                    MPI_Isend(buff,docName.length()+1,MPI_CHAR,i,MAPPER_TAG,MPI_COMM_WORLD,&req);
                    cout<<"Radacina "<<ROOT<<" a trimis mesajul: "<<buff<<" procesului "<<i<<"\n";
                }
                i++;
                if(i>=np && !list.empty())
                {
                    i=0;
                }
                slaves[i]++;//
            }
            if(list.empty())
            {
                isMapped=true;
            }
        }
                      
        //print(slaves,np);

        for(int j=1;j<np;j++)
        {
            MPI_Isend(message,strlen(message)+1,MPI_CHAR,j,END_MAP_TAG,MPI_COMM_WORLD,&req);
            //cout<<"Trimit la "<<j<<" "<<message<<" tag="<<END_MAP_TAG<<"\n";
        }

        for(int j=1;j<np;j++)
        {
            while(slaves[j]-->0)
            {
                MPI_Recv(buff,MAX_VAL,MPI_CHAR,j,MAPPER_TAG,MPI_COMM_WORLD,&status);
                cout<<"Radacina am primit "<<buff<<" de la procesul "<<status.MPI_SOURCE<<" cu tagul:"<<status.MPI_TAG<<"\n";
            }   
        }
        i++;
        if(i>=np)
            i=1;

        for(int j=1;j<np;j++)
        {
            if(j==i)
            {
                MPI_Isend(argv[2],strlen(argv[2])+1,MPI_CHAR,i,REDUCER_TAG,MPI_COMM_WORLD,&req);
                cout<<"Trimit redurece procesului "<<i<<"\n";
            }
            else
            {
                char red[]="Sunt liber";
                MPI_Isend(red,strlen(red)+1,MPI_CHAR,j,CHECK_MESSAGE,MPI_COMM_WORLD,&req);
                cout<<"Trimit la "<<j<<" "<<red<<" libertate tag="<<CHECK_MESSAGE<<"\n";
            }
        }
        MPI_Recv(buff,MAX_VAL,MPI_CHAR,i,REDUCER_TAG,MPI_COMM_WORLD,&status);
        cout<<"Am finalizat reducearea in "<<buff<<" de la procesul "<<i<<"\n"; 
        delete[] slaves;      
    }
    else
    {
        while(true)
        {
            MPI_Recv(buff,MAX_VAL,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            
            if(status.MPI_TAG==MAPPER_TAG)
            {
                char filename[MAX_VAL];
                strcpy(filename,argv[1]);
                strcat(filename,buff);
                cout<<rank<<" "<<filename<<"\n";
                string mapper=Mapper(filename);
                
                char* response=stringToChar(mapper);

                cout<<"Procesul "<<rank<<" a primit "<<buff<<" de la RADACINA, si mapeaza in fisierul="<<filename<<" in "<<mapper<<"\n";
                //trimit la master ca am terminat cu de procesat fisierul;
                MPI_Send(response,strlen(response)+1,MPI_CHAR,ROOT,MAPPER_TAG,MPI_COMM_WORLD);
                
            }
            else if(status.MPI_TAG==END_MAP_TAG)
            {
                //cout<<buff<<" proces "<<rank<<"\n";
                break;
            }
        }
        MPI_Recv(buff,MAX_VAL,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        if(status.MPI_TAG==REDUCER_TAG)
        {
            cout<<"Sunt procesul: "<<rank<<" si fac reducerea"<<buff<<"\n";
            string reduce=Reducer(buff);
            //cout<<reduce<<"\n\n";
            MPI_Send(stringToChar(reduce),reduce.size()+1,MPI_CHAR,ROOT,REDUCER_TAG,MPI_COMM_WORLD);
        }
        else
        {
            cout<<"Sunt procesul: "<<rank<<" si sunt liber cu mesajul "<<buff<<"\n";
        }
    }
    MPI_Finalize();
    return 0;
}