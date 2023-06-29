#include <iostream>
#include "database.h"
#include <string>
using std::string;
using std::cin;

int main(){
    //test program
    database Mydatabase;
    
    
    //Insert 5000 pairs KV
    //This process will trigger the following functions:
    /*
       1. Memtable to SStable
       2. Merge of SStable (level0 -> level 1)
       3. find key in sstable
    */
    for(int i=0;i<6000;i++){
        string key = "haha" + std::to_string(i);
        string value = "haha" + std::to_string(i);
        Mydatabase.set(key,value);
    }
    auto result = Mydatabase.find("haha1000"); // key in sstable level 1
    auto result1 = Mydatabase.find("haha4800"); // key in sstable level 0
    auto result2 = Mydatabase.find("haha5500"); // key in mmtable
    auto result3 = Mydatabase.find("haha6001"); // key which does not exist
    std::cout<<(result.second)<<std::endl;
    std::cout<<(result.first).get_value() <<std::endl;
    
    std::cout<<(result1.second)<<std::endl;
    std::cout<<(result1.first).get_value() <<std::endl;
    
    std::cout<<(result2.second)<<std::endl;
    std::cout<<(result2.first).get_value() <<std::endl;

    std::cout<<(result2.second)<<std::endl;
    std::cout<<(result2.first).get_value() <<std::endl;
    
    return 0;
}