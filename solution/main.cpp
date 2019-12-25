
#include <string>
#include <stdio.h>
#include <mpi/mpi.h>
#include <iostream>
#include <dirent.h>
#include <list>


#define ROOT 4

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
int main(int argc,char **argv)
{
    int rank;
    int np;

    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    if(rank==ROOT)
    {
        list<string> list=getFileNameFromDirectory("/home/stefan/Desktop/alpd-MapReduce/solution/dateIntrare");
        showlist(list);
    }
    MPI_Finalize();
    return 0;
}