#ifndef __DCS5231OBJECT_H__
#define __DCS5231OBJECT_H__
#include "lstorestruct.h"
//Define the basic class for a pair of key-value.

//The receive raw data type.
//In this implement, the raw data type is encoded as the same.
enum RAW_DATA_TYPE{L_INT,L_STRING,L_HASH_TABLE,L_VECTOR,L_SET,L_NULL};
enum OPRAND{SET,APPEND,REMOVE};

class dsc5231Object{
    // Virtual fucntion cannot have a deduced return type.
public:
    void* value;
    RAW_DATA_TYPE _t;
public:
    dsc5231Object(void* ptr=nullptr,const RAW_DATA_TYPE _type=L_NULL);
    dsc5231Object(const dsc5231Object& a);
    ~dsc5231Object();
    void free_value_space();
    const RAW_DATA_TYPE get_raw_data_type() const;
    void _append(const dsc5231Object& _append_data);
    void _remove();
    void _set(const dsc5231Object& _set_data);
public:
    void operator=(const dsc5231Object& a);
};

class valueObject{
public:
    dsc5231Object valueStorage;
    OPRAND operation_log;
public:
    valueObject(dsc5231Object a=dsc5231Object(),OPRAND b=OPRAND::REMOVE);
    valueObject(const valueObject& a);

    void operation_log_transfer(OPRAND op_log);
    const OPRAND get_operation_log() const;
    void append(const valueObject& value_appened);
    void remove();
    void set(const valueObject& value_seted);
public:
    void operator=(const valueObject & a);
};


//Example of the dsc5231 key-value type

#endif
