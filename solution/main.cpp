
#include <string>
#include <stdio.h>
#include <mpi/mpi.h>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <list>


#define ROOT 0
#define MAX_VAL 50

using namespace std;

void showlist(list <string> g) 
{ 
    list <string> :: iterator it; 
    for(it = g.begin(); it != g.end(); ++it) 
        cout << *it<<"\n";
} 
list<string> getFileNameFromDirectory(const char *filePath)
{
    list<string> fileName;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (filePath)) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            // cout<<ent->d_name<<"\n";
            if(ent->d_type == DT_REG)
            {
                fileName.push_front(ent->d_name);
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        perror ("");
        return fileName;
    }
    return fileName;
}

void readFile(const char *filename)
{
    ifstream myReadFile;
    myReadFile.open(filename);
    char output[100];
    if(myReadFile.is_open())
    {
        while (!myReadFile.eof())
        {
            myReadFile>>output;
            cout<<output<<"\n";
        }
        
    }
}
char * stringToChar(string test)
{
    int n=test.length();
    char *array=new char[n+1];

    strcpy(array,test.c_str());
    return array;
}
int main(int argc,char **argv)
{
    int rank;
    int np;
    MPI_Request req;
    // cout<<argc<<"\n";
    // for (int i = 0; i < argc; i++)
    // {
    //     cout<<argv[i]<<" ";
    // }
    // cout<<"\n";
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    if(rank==ROOT)
    {
        //iau numele fisirelor din folderul din linia de comanda
        list<string> list=getFileNameFromDirectory(argv[1]);
       // showlist(list);
       // cout<<list.size();
        //readFile("/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/1.txt");
        int i=0;
        int count=0;
        while(i<np )
        {
            string docName=list.front();
            if(i!=ROOT)
            {
                char *buff=stringToChar(docName);
                list.pop_front();
                MPI_Send(buff,docName.length(),MPI_CHAR,i,99,MPI_COMM_WORLD);
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
    }
    else
    {
        
        char buff[MAX_VAL];
        MPI_Recv(buff,MAX_VAL,MPI_CHAR,ROOT,99,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                cout<<"Procesul "<<rank<<" a primit "<<buff<<" de la RADACINA\n";
        //di
    }
    
    MPI_Finalize();
    return 0;
}