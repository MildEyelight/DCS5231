#ifndef __LSMTREE_H_
#define __LSMTREE_H_

#include <map>
#include "store/storestruct.h"
#include <tuple>

using std::map;
using std::pair;
enum KV {KEY,VALUE};
using kvPair = std::pair<key,Value>;
using lsmtreeStorage = map<key,Value>;

class lsmtree{
private:
    //main structure of the lsmtree.
    //It consists of a tree which is implemented by a Rb-Tree and a conunter to indicate the merge timing.
    lsmtreeStorage tree;
    int32_t nodeCount;

    void insert(const kvPair& insert_kvpair);
public:
    lsmtree();
    lsmtree(const lsmtreeStorage& t,const int32_t count=0);
    lsmtree(const lsmtree& t);
    //Four main operations of the database.
    //The assert API use to provide a unified interface to deal the kvpair pass in
    //with different operation stored in the storeObject.operation_log.
    
    //CRUD
    void set(const kvPair& set_kvpair);
    
    //the lsmtree is a append only tree, the remove update is actually all replaced by set.
    // void remove(const key& remove_key);
    // void update(const kvPair& append_kvpair);
    
    std::pair<Value,bool> find(const key& key);
    
    int32_t get_node_number() const;
    lsmtreeStorage get_tree() const;
public:
    //Other utils functions
    bool is_key_exists(const key& key) const;
};
#endif