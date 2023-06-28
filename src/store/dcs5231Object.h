#ifndef __DCS5231OBJECT_H__
#define __DCS5231OBJECT_H__
#include "storestruct.h"
//Define the basic class for a pair of key-value.

//The receive raw data type.
//In this implement, the raw data type is encoded as the same.

class dsc5231Object{
public:
    void* value;
    RAW_DATA_TYPE _t;
public:
    dsc5231Object();
    dsc5231Object(void* ptr,const RAW_DATA_TYPE _type);
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
    oprand operation_log;
public:
    valueObject(dsc5231Object a=dsc5231Object(),oprand b=oprand::REMOVE);
    valueObject(const valueObject& a);

    void operation_log_transfer(oprand op_log);
    const oprand get_operation_log() const;
    void append(const valueObject& value_appened);
    void remove();
    void set(const valueObject& value_seted);
public:
    void operator=(const valueObject & a);
};


//Example of the dsc5231 key-value type



#endif
