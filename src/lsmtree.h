#ifndef __LSMTREE_H_
#define __LSMTREE_H_

#include <map>
#include "lstorestruct.h"
#include "dcs5231Object.h"

#include <tuple>
enum KV {KEY,VALUE};
using kvPair = std::pair<lkey,valueObject>;
using lsmtreeStorage = std::map<lkey,valueObject>;

class lsmtree{
private:
    //main structure of the lsmtree.
    //It consists of a tree which is implemented by a Rb-Tree and a conunter to indicate the merge timing.
    lsmtreeStorage tree;
    int32_t nodeCount;
public:
    lsmtree();
    lsmtree(const lsmtreeStorage& t,const int32_t count);
    lsmtree(const lsmtree&);
    //Four main operations of the database.
    //The assert API use to provide a unified interface to deal the kvpair pass in
    //with different operation stored in the storeObject.operation_log.
    void insert(const kvPair& receive_kvpair);
    void set(const kvPair& insert_kvpair);
    void remove(const lkey);
    void append(const kvPair& append_kvpair);
    kvPair get(const lkey& key) ;
    int32_t get_node_number() const;
    lsmtreeStorage get_tree() const;
public:
    //Other utils functions
    auto get_iter_by_key(const lkey& key);
    bool is_key_exists(const lkey& key) const;
};
#endif