#include "lsmtree.h"
#include <tuple>

lsmtree::lsmtree():tree(lsmtreeStorage()),nodeCount(0){
}

lsmtree::lsmtree(const lsmtreeStorage& t,const int32_t count):tree(t),nodeCount(count){
}

lsmtree::lsmtree(const lsmtree& t){
    this->tree = t.get_tree();
    this->nodeCount = t.get_node_number();
}

void lsmtree::set(const kvPair& insert_kvpair){
    // std::pair<lsmtreeStorage::iterator,bool> ret;
    
    this->tree[insert_kvpair.first] = insert_kvpair.second;
    this->nodeCount = this->tree.size();
    // ret = this->tree.insert(insert_kvpair);
    // if(ret.second==false){
    //     // std::cout<< "Element " + insert_kvpair.first + " already exists";
    //     ret.first = insert_kvpair.second;
    // }

    // if(key_iter == this->tree.end()) this->tree.insert(insert_kvpair);
    // else key_iter->second.set(std::get<VALUE>(insert_kvpair));
    // return ;
}

// void lsmtree::remove(const lkey key){
    
//     auto key_iter = this->get_iter_by_key(key);
//     if(key_iter != this->tree.end()) key_iter->second.remove();
//     else ;//#TODO some special process
// }

// void lsmtree::append(const kvPair& append_kvpair){
//     auto key_iter = this->get_iter_by_key(std::get<KEY>(append_kvpair));
//     if(key_iter == this->tree.end());//#TODO some special process
    
//     valueObject& node_to_append = this->tree[std::get<KEY>(append_kvpair)];
//     node_to_append.append(std::get<VALUE>(append_kvpair));
// }

// void lsmtree::insert(const kvPair& receive_kvpair){
//     lkey key = std::get<KEY>(receive_kvpair);
//     valueObject value = std::get<VALUE>(receive_kvpair);
//     OPRAND operation = value.operation_log;
//     if(operation==OPRAND::SET) this->set(receive_kvpair);
//     if(operation==OPRAND::REMOVE) this->remove(key);
//     if(operation==OPRAND::APPEND) this->append(receive_kvpair);
// }

std::pair<Value,bool> lsmtree::find(const key& key) {
    auto iter = this->tree.find(key);
    if(iter!=this->tree.end()){
        return std::pair<Value,bool>(iter->second,true);
    }
    else{
        return std::pair<Value,bool>(Value(),false);
    }
    //Special process if not in the kvpair
    //later the bloom filter will be applied to this searching process.
}

int32_t lsmtree::get_node_number() const{
    return this->nodeCount;
}

lsmtreeStorage lsmtree::get_tree() const {
    return this->tree;
}