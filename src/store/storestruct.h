#ifndef __LDATASTRUCT_H__
#define __LDATASTRUCT_H__

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

using std::string;
typedef std::string key;
using lvector = std::vector<string>;
using lhash = std::unordered_map<key,string>;
using lset = std::map<key,string>;
enum RAW_DATA_TYPE{L_INT,L_STRING,L_HASH_TABLE,L_VECTOR,L_SET,L_NULL};
enum oprand{SET,APPEND,REMOVE};

class Value {
private: 
    string _value;
    oprand _log; // set or delete, we dont use append in fact;
public:
    Value() = default;
    
    Value(const string& a,oprand operation):_value(a),_log(operation){};
    
    Value(const Value& a):_value(a._value),_log(a._log){};
    
    Value& operator=(const Value &a){
        this->_value = a._value;
        this->_log = a._log;
        return *this;
    };
    ~Value(){};
    
    string get_value(){
        return this->_value;
    };
    
    oprand get_oprand(){
        return this->_log;
    };
};

#endif






