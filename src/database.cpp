#include "database.h"
#include "store/storestruct.h"
#include "lsmtree/sstable.h"
#include <string>
using std::string;

void database::set(const string& key, const string& value){
    Value insert_value(value,oprand::SET);
    kvPair insert_kv(key,insert_value);
    this->memtable->set(insert_kv);
    return;
}

void database::remove(const string& key){
    Value tmp(string(),oprand::REMOVE);
    kvPair remove_kv(key,tmp);
    this->memtable->set(remove_kv);
}

void database::update(const string& key, const string& value){
    this->set(key,value);
}

std::pair<Value,bool> database::find(const string& key){
    //search memtable
    std::pair<Value,bool> search_result = this->memtable->find(key);
    if(search_result.second){
        return search_result;
    }

    //search sstable
    else{
        return this->sstable_manager->find(key);
    }
    //search sstable
}


void database::mmtable_to_sstable(){
    //For asynchronous propose.
    this-> immutable_memtable = this->memtable;
    this->memtable = new lsmtree();
    this->sstable_manager->save_memtable(immutable_memtable);
    delete this->immutable_memtable;
    this->immutable_memtable = nullptr;

    return;
}
database::database():memtable(new lsmtree()),
                     immutable_memtable(nullptr),
                     sstable_manager(new sstable()) {}

database::~database(){
    delete this->memtable;
    delete this->immutable_memtable;
    delete this->sstable_manager;
}