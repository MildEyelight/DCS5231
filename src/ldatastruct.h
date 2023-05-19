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

template<typename data_type>
using lvector = std::vector<data_type>;

template<typename data_type>
using lhash = std::unordered_map<data_type>;

//ordered set
template<typename data_type>
using lset = std::map<data_type>;

#endif






