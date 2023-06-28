#include <iostream>
#include "database.h"


int main(){
    database Mydatabase;
    Mydatabase.set("liyh","smart");
    auto result = Mydatabase.find("liyh");
    std::cout<<(result.first).get_value() <<std::endl;


    return 0;
}