#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define ROOT 0
#define MAX_VAL 100
//debug -> deprecated
#define REDUCCER_MASTER 0

#define MAPPER_TAG 100
#define REDUCER_TAG 101
#define END_MAP_TAG 102
#define CHECK_MESSAGE 103

#define DIRECTORY_MAPP "/home/stefan/Desktop/alpd-MapReduce/solution/dateMapper/"
#define DIRECTORY_IN "/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/"
#define DIRECTORY_OUT "/home/stefan/Desktop/alpd-MapReduce/solution/dataOut/"

char *stringToChar(string test);
void showlist(list <string> g);
int *initSlaves(int noSlaves,int master);
list<string> getFileNameFromDirectory(const char *filePath);
void deleteFilesFromDirectory(const char * dirPath);

vector<string> readFile(const char *filename);
string Mapper(const char *filename);

string Reducer(const char *outputfolder);
void print(int *a,int n);