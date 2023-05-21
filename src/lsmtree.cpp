#include "lsmtree.h"
#include "dcs5231Object.h"
#include <tuple>

auto lsmtree::get_iter_by_key(const lkey& key) {
    return this->tree.find(key);
}
void lsmtree::set(const kvPair& insert_kvpair){
    auto key_iter = this->get_iter_by_key(std::get<KEY>(insert_kvpair));
    if(key_iter == this->tree.end()) this->tree.insert(insert_kvpair);
    else key_iter->second.set(std::get<VALUE>(insert_kvpair));
    return ;
}

void lsmtree::remove(const lkey key){
    auto key_iter = this->get_iter_by_key(key);
    if(key_iter != this->tree.end()) key_iter->second.remove();
    else ;//#TODO some special process
}

void lsmtree::append(const kvPair& append_kvpair){
    auto key_iter = this->get_iter_by_key(std::get<KEY>(append_kvpair));
    if(key_iter == this->tree.end());//#TODO some special process
    
    valueObject& node_to_append = this->tree[std::get<KEY>(append_kvpair)];
    node_to_append.append(std::get<VALUE>(append_kvpair));
}

void lsmtree::insert(const kvPair& receive_kvpair){
    lkey key = std::get<KEY>(receive_kvpair);
    valueObject value = std::get<VALUE>(receive_kvpair);
    OPRAND operation = value.operation_log;
    if(operation==OPRAND::SET) this->set(receive_kvpair);
    if(operation==OPRAND::REMOVE) this->remove(key);
    if(operation==OPRAND::APPEND) this->append(receive_kvpair);
}

const kvPair& lsmtree::get(const lkey& key) {
    auto iter = this->get_iter_by_key(key);
    if(iter!=this->tree.end()) return *iter;
    //Special process if not in the kvpair
    //later the bloom filter will be applied to this searching process.
}