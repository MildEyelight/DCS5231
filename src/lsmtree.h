#ifndef __LSMTREE_H_
#define __LSMTREE_H_

#include <map>
#include "lstorestruct.h"
#include "dcs5231Object.h"

#include <tuple>
enum KV {KEY,VALUE};
using kvPair = std::pair<lkey,valueObject>;

class lsmtree{
public:
    //main structure of the lsmtree.
    //It consists of a tree which is implemented by a Rb-Tree and a conunter to indicate the merge timing.
    std::map<lkey,valueObject> tree;
    int32_t nodeCount;
public:
    //Four main operations of the database.
    //The assert API use to provide a unified interface to deal the kvpair pass in
    //with different operation stored in the storeObject.operation_log.
    void insert(const kvPair& receive_kvpair);

    void set(const kvPair& insert_kvpair);
    void remove(const lkey);
    void append(const kvPair& append_kvpair);
    const kvPair& get(const lkey& key) ;
public:
    //Other utils functions
    auto get_iter_by_key(const lkey& key);
    bool is_key_exists(const lkey& key) const;
};
#endif