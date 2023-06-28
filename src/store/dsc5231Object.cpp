#include "dcs5231Object.h"


void dsc5231Object::free_value_space(){
    if(this->value!=nullptr) {
        if(this->_t == RAW_DATA_TYPE::L_INT){delete reinterpret_cast<int32_t*>(this->value);}
        else if(this->_t == RAW_DATA_TYPE::L_STRING){delete reinterpret_cast<string*>(this->value);}
        else if(this->_t == RAW_DATA_TYPE::L_VECTOR){delete reinterpret_cast<lvector*>(this->value);}
        else if(this->_t == RAW_DATA_TYPE::L_HASH_TABLE){delete reinterpret_cast<lhash*>(this->value);}
        else if(this->_t == RAW_DATA_TYPE::L_SET){delete reinterpret_cast<lset*>(this->value);}
    }
    this->value=nullptr;
}
dsc5231Object::dsc5231Object():value(nullptr),_t(RAW_DATA_TYPE::L_NULL){

}
dsc5231Object::dsc5231Object(void* ptr,const RAW_DATA_TYPE _type){
    //这里的实现也有些问题。)
    if(_type == RAW_DATA_TYPE::L_INT){this->value = new int32_t(*reinterpret_cast<int32_t*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_STRING){this->value = new string(*reinterpret_cast<string*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_VECTOR){this->value = new lvector(*reinterpret_cast<lvector*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_HASH_TABLE){this->value = new lhash(*reinterpret_cast<lhash*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_SET){this->value = new lset(*reinterpret_cast<lset*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_NULL){this->value = nullptr;}
    this->_t = _type;
}
dsc5231Object::dsc5231Object(const dsc5231Object& a){
    //A deep copy cause a pointer within dsc5231Object.
    
    auto ptr = a.value;
    auto _type = a._t;
    if(_type == RAW_DATA_TYPE::L_INT){this->value = new int32_t(*reinterpret_cast<int32_t*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_STRING){this->value = new string(*reinterpret_cast<string*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_VECTOR){this->value = new lvector(*reinterpret_cast<lvector*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_HASH_TABLE){this->value = new lhash(*reinterpret_cast<lhash*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_SET){this->value = new lset(*reinterpret_cast<lset*>(ptr));}
    else if(_type == RAW_DATA_TYPE::L_NULL){this->value = nullptr;}
    this->_t = _type;
}
dsc5231Object::~dsc5231Object(){
    this->free_value_space();
}


void dsc5231Object::operator=(const dsc5231Object& a) {
    if(this->value!=nullptr) this->free_value_space();
    //这里的实现也有些问题。
    if(a._t == RAW_DATA_TYPE::L_INT){this->value = new int32_t(*reinterpret_cast<int32_t*>(a.value));}
    else if(a._t == RAW_DATA_TYPE::L_STRING){this->value = new string(*reinterpret_cast<string*>(a.value));}
    else if(a._t == RAW_DATA_TYPE::L_VECTOR){this->value = new lvector(*reinterpret_cast<lvector*>(a.value));}
    else if(a._t == RAW_DATA_TYPE::L_HASH_TABLE){this->value = new lhash(*reinterpret_cast<lhash*>(a.value));}
    else if(a._t == RAW_DATA_TYPE::L_SET){this->value = new lset(*reinterpret_cast<lset*>(a.value));}
    this->_t = a._t;
}
void dsc5231Object::_remove(){
    if(this->value!=nullptr) this->free_value_space();
    this->_t = L_NULL;
}

void dsc5231Object::_set(const dsc5231Object& _set_data){
    this->operator=(_set_data);
}

void dsc5231Object::_append(const dsc5231Object& _append_data){
    if(this->_t!=_append_data._t);//Special process.
    if(this->_t == RAW_DATA_TYPE::L_INT){
        auto current_data_ptr = reinterpret_cast<int32_t*>(this->value);
        auto append_data_ptr = reinterpret_cast<int32_t*>(_append_data.value);
        *current_data_ptr = *current_data_ptr + *append_data_ptr;
    }
    else if(this->_t == RAW_DATA_TYPE::L_NULL);//Special process  
    else if(this->_t == RAW_DATA_TYPE::L_STRING){
        auto current_data_ptr = reinterpret_cast<string*>(this->value);
        auto append_data_ptr = reinterpret_cast<string*>(_append_data.value);
        current_data_ptr->append(*append_data_ptr);
    }
    else if(this->_t == RAW_DATA_TYPE::L_VECTOR){
        auto current_data_ptr = reinterpret_cast<lvector*>(this->value);
        auto append_data_ptr = reinterpret_cast<lvector*>(_append_data.value);
        for(auto iter = append_data_ptr->begin();iter!=append_data_ptr->end();iter++){
            current_data_ptr->push_back(*iter);
        }
    }
    else if(this->_t == RAW_DATA_TYPE::L_HASH_TABLE){
        auto current_data_ptr = reinterpret_cast<lhash*>(this->value);
        auto append_data_ptr = reinterpret_cast<lhash*>(_append_data.value);
        for(auto iter = append_data_ptr->begin();iter!=append_data_ptr->end();iter++){
            current_data_ptr->insert(*iter);
        }
    }
    else if(this->_t == RAW_DATA_TYPE::L_SET){
        auto current_data_ptr = reinterpret_cast<lset*>(this->value);
        auto append_data_ptr = reinterpret_cast<lset*>(_append_data.value);
        for(auto iter = append_data_ptr->begin();iter!=append_data_ptr->end();iter++){
            current_data_ptr->insert(*iter);
        }
    }
}

valueObject::valueObject(dsc5231Object a,OPRAND b)
    :valueStorage(a),operation_log(b){}
valueObject::valueObject(const valueObject& a){
    this->valueStorage = a.valueStorage;
    this->operation_log = a.operation_log;
}

const OPRAND valueObject::get_operation_log() const{
    return this->operation_log;
}

void valueObject::operation_log_transfer(OPRAND current_operation){
    //The state transition of the database operation.
    OPRAND last_operation_log = this->get_operation_log();
    if(last_operation_log==OPRAND::APPEND){
        if(current_operation==OPRAND::APPEND);
        else if(current_operation==OPRAND::REMOVE) this->operation_log = current_operation;
        else if(current_operation==OPRAND::SET) this->operation_log = current_operation;
    }
    else if(last_operation_log==OPRAND::REMOVE){
        if(current_operation==OPRAND::APPEND);//Special process
        else if(current_operation==OPRAND::REMOVE);//Special process
        else if(current_operation==OPRAND::SET) this->operation_log = current_operation;
    }
    else if(last_operation_log==OPRAND::SET){
        if(current_operation==OPRAND::APPEND) this->operation_log = OPRAND::SET;
        else if(current_operation==OPRAND::REMOVE)this->operation_log = current_operation;
        else if(current_operation==OPRAND::SET);
    }
    return;
}

void valueObject::operator=(const valueObject& a){
    this->valueStorage = a.valueStorage;
    this->operation_log = a.operation_log;
    // this->operation_log_transfer(a.operation_log);
}

void valueObject::remove(){
    this->operation_log_transfer(OPRAND::REMOVE);
    this->valueStorage._remove();
}

void valueObject::append(const valueObject& value_appended){
    this->operation_log_transfer(OPRAND::APPEND);
    const dsc5231Object& append_data = value_appended.valueStorage;
    this->valueStorage._append(append_data);
}

void valueObject::set(const valueObject& value_seted){
    this->valueStorage = value_seted.valueStorage;
    this->operation_log_transfer(value_seted.operation_log);
}