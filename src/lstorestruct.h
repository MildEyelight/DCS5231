#ifndef __LDATASTRUCT_H__
#define __LDATASTRUCT_H__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

//Define the type of value: string,integer,dictionary,vector 
//All of this data structure use the STL in C++.

typedef std::string lkey;

typedef std::string lstring;

using lvector = std::vector<lstring>;

using lhash = std::unordered_map<lkey,lstring>;

using lset = std::map<lkey,lstring>;

#endif






