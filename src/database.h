#ifndef _DATABASE_H__
#define _DATABASE_H__

#include "lsmtree/lsmtree.h"
#include "lsmtree/sstable.h"
#include <string>

using std::string;

class database{
private:
    lsmtree* memtable;
    lsmtree* immutable_memtable;
    sstable* sstable_manager;
    static const int32_t MAX_KVPAIR_COUNT;
private:
    void mmtable_to_sstable();
public:
    database();
    ~database();
public:
    //CRUD
    void set(const string& key, const string& value);
    void remove(const string& key);
    void update(const string& key, const string& value);
    // return the value;
    std::pair<Value,bool> find(const string& key);
};




#endif