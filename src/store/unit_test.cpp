#include <iostream>
using namespace std;
#include "lsmtree.h"
#include "storestruct.h"
#include "dcs5231Object.h"
#include <memory>

// int main(){
//     void* a = new int32_t(32); // This resource will be mananged by the distructor of dsc5231Object.
//     unique_ptr<dsc5231Object> b(new dsc5231Object(a,RAW_DATA_TYPE::L_INT));
//     unique_ptr<valueObject> c(new valueObject(*b,OPRAND::SET));

//     lsmtree database1 = lsmtree();
//     lkey mytestkey1 = lkey("give me five");
//     kvPair mytestkvpair = kvPair(mytestkey1,*c);
//     database1.insert(mytestkvpair);
//     auto mytestkvpair2 = database1.get(lkey("give me five"));
//     cout<<*reinterpret_cast<int32_t*>((std::get<1>(mytestkvpair).valueStorage.value))<<endl;
//     return 0;
// }