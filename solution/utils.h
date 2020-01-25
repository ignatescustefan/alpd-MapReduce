#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define ROOT 0
#define MAX_VAL 100

#define MAPPER_TAG 100
#define REDUCER_TAG 101
#define END_MAP_TAG 102

#define DIRECTORY_MAPP "/home/stefan/Desktop/alpd-MapReduce/solution/dateMapper/"
#define DIRECTORY_IN "/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/"
#define DIRECTORY_OUT "/home/stefan/Desktop/alpd-MapReduce/solution/dataOut/"
void showlist(list <string> g);
int *initSlaves(int noSlaves,int master);
list<string> getFileNameFromDirectory(const char *filePath);
string tokenizing(string s);
vector<string> readFile(const char *filename);
char * stringToChar(string test);
string Mapper(const char *filename);

void Reducer(const char *outputfolder);