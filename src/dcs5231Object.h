#ifndef __DCS5231OBJECT_H__
#define __DCS5231OBJECT_H__
#include "ldatastruct.h"
//Define the basic class for a pair of key-value.
template<typename data_type>

class dsc5231Object{
public:
    lkey key;
    data_type value;
};

typedef dsc5231Object<lvector<int>> dsc5231KeyIntVector;
typedef dsc5231Object<lstring> dsc5231KeyString;
typedef dsc5231Object<int> dsc5231KeyInt;

#endif
