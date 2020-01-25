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
    string chars = "#!\"\'\\,.-?!;:";
 
    for (char c: chars) {
        s.erase(std::remove(s.begin(), s.end(), c), s.end());
    }
    return s;
}
vector<string> readFile(const char *filename)
{
    ifstream myReadFile;
   // cout<<" sunt in read file cu filename="<<filename<<"\n";

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
   // cout<<"Mapper cu filename="<<filename<<"\n";
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
    //cout<<" \n";
    file.close();
    return mapfile;

}

string Reducer(const char* outputfolder)
{
    list<string> files=getFileNameFromDirectory(DIRECTORY_MAPP);
    map<string,map<string,int>> reverseIndex;
    map<string,int> directIndex;
    ifstream myReadFile;

    string outfile(outputfolder);
    outfile+="output.txt";
    
    
    if( remove(stringToChar(outfile)) == 0 ); //sterg fisierul daca exista

    ofstream outStream(outfile); //open in constructor
    //deschid fiecare fisier

    for(string file:files)
    {
        string temp=DIRECTORY_MAPP;
        temp+=file;
        myReadFile.open(temp);
        if(myReadFile.is_open())
        {
            //citesc datele si le pun in indexul direct
           // cout<<"Am deschis fisierul: "<<temp<<"\n";
            while (!myReadFile.eof())
            {
                string key;
                int value;
                myReadFile>>key>>value;
               // cout<<key<<" "<<value<<"\n";
                directIndex.insert(pair<string,int>(key,value));
            }
            //scot spatiun 
            auto it = directIndex.begin();
            directIndex.erase(it);
            //parcurg indexul direct
            map<string, int>::iterator itr; 
            for (itr = directIndex.begin(); itr != directIndex.end(); ++itr)
            { 
                string term=itr->first;
                int value=itr->second;
                string doc=file;
                if(reverseIndex.find(term)!=reverseIndex.end())
                {
                    //exista termenul in dictionar
                    map<string,int> docCounter;
                    docCounter[doc]=value;
                    auto element=pair<string,int>(doc,value);
                    reverseIndex[term].insert(element);
                }
                else
                {
                    // map<string,int> docCounter;
                    // docCounter[doc]=value;

                    // list<map<string,int> > docCounterList;
                    // docCounterList.push_front(docCounter);
                    // auto element=pair<string,list<map<string,int>>>(term,docCounterList);
                    
                    map<string,int> docCounter;
                    docCounter[doc]=value;
                    auto element=pair<string,int>(doc,value);
                    
                    reverseIndex.insert(pair<string,map<string,int>>(term,docCounter));
                    //nu exista
                }
                
            } 
           //
            myReadFile.close();
            directIndex.clear();
            
        }
        else
        {
            cout<<"NU am deschis fisierul: "<<temp<<"\n";

        }        
    }

    //afisez
    auto it=reverseIndex.begin();
    for (it = reverseIndex.begin(); it != reverseIndex.end(); ++it)
    {
        string term=it->first;
        auto mapTerm=it->second;
        //cout<<term<<": {\n";
        outStream<<term<<": {\n";        
        auto itMap=mapTerm.begin();
        for(itMap = mapTerm.begin(); itMap != mapTerm.end(); ++itMap)
        {
          //  cout<<"\t"<<itMap->first<<" : "<<itMap->second<<"\n";
            outStream<<"\t"<<itMap->first<<" : "<<itMap->second<<"\n";
        }
       // cout<<"}\n";
        outStream<<"}\n";
    }
    outStream.close();
    return outfile;
}

int * initSlaves(int noSlaves,int master)
{
    int *slaves=new int[noSlaves];
    for(int i=0;i<noSlaves;i++)
    {
        if(i==master)
        {
            slaves[i]=-1;
        }
        else
        {
            slaves[i]=0;
        }
        
    }
    return slaves;
}
//master;

void DeleteFilesFromDirectory(const char * dirPath)
{
    list<string> files=getFileNameFromDirectory(dirPath);
    string path(dirPath);
    for(auto el:files)
    {
        el=dirPath+el;
        if( remove(stringToChar(el)) == 0 );
    }
}
void print(int *a,int n)
{
    cout<<"Functia de print:\n";
    for(int i=0;i<n;i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
}