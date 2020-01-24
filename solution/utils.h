#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define ROOT 0
#define MAX_VAL 50

#define DIRECTORY_MAPP "/home/stefan/Desktop/alpd-MapReduce/solution/dateMapper/"
#define DIRECTORY_IN "/home/stefan/Desktop/alpd-MapReduce/solution/dataIn/"

void showlist(list <string> g);
list<string> getFileNameFromDirectory(const char *filePath);
string tokenizing(string s);
vector<string> readFile(const char *filename);
char * stringToChar(string test);
string Mapper(const char *filename);