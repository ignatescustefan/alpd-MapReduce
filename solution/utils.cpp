#include "utils.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <dirent.h>

#include <stdio.h>
#include <cstring>

#include <list>
#include <vector>
#include <string>
#include <map>

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
string tokenizing(string s)
{
    string chars = "#!\"\'\\,.-?!";
 
    for (char c: chars) {
        s.erase(std::remove(s.begin(), s.end(), c), s.end());
    }
    return s;
}
vector<string> readFile(const char *filename)
{
    ifstream myReadFile;
    cout<<" sunt in read file cu filename="<<filename<<"\n";

    myReadFile.open(filename);
    char output[100];
    vector<string> text;
    if(myReadFile.is_open())
    {
        while (!myReadFile.eof())
        {
            myReadFile>>output;
            text.push_back(tokenizing(output));
   //         cout<<output<<"\n";
        } 
    }
    return text;
}
char * stringToChar(string test)
{
    int n=test.length();
    char *array=new char[n+1];
    
    strcpy(array,test.c_str());
    return array;
}

string Mapper(const char *filename)
{
    cout<<"Mapper cu filename="<<filename<<"\n";
    string mapfile(filename);
    std::size_t found = mapfile.find_last_of("/\\");
    mapfile=DIRECTORY_MAPP+mapfile.substr(found+1);
    
    auto text = readFile(filename);
    
    if( remove(stringToChar(mapfile)) == 0 ); //sterg fisierul daca exista

    ofstream file(mapfile); //open in constructor
    
    map<string,int> dictionary;
    for(int i=0;i<text.size();i++)
    {
        string word=text.at(i);
        if(dictionary.find(word)!=dictionary.end())
        {
            //exista cuvantul in dictionar
            dictionary[word]++;
        }
        else
        {
            dictionary.insert(pair<string,int>(word,1));
        }
    }

    map<string, int>::iterator itr; 
    for (itr = dictionary.begin(); itr != dictionary.end(); ++itr)
    { 
        file<<itr->first<<" "<< itr->second <<"\n";
    //   cout<<itr->first<<" "<< itr->second <<"\n";
    }
    file<<" \n";
    //cout<<" \n";
    file.close();
    return mapfile;

}