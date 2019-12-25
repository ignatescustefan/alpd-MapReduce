
#include <string>
#include <iostream>
#include <dirent.h>
#include <list>
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
            fileName.push_front(ent->d_name);
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

    

    list<string> list=getFileNameFromDirectory("/home/stefan/Desktop/alpd-MapReduce/dateIntrare");
    showlist(list);
}