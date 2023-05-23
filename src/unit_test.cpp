#include <iostream>
using namespace std;
#include "lsmtree.h"
#include "lstorestruct.h"
#include "dcs5231Object.h"

int32_t * a = new int32_t (32);
dsc5231Object* b = new dsc5231Object(a,RAW_DATA_TYPE::L_INT);
valueObject* c = new valueObject(*b,OPRAND::SET);

int main(){
    lsmtree database1 = lsmtree();
    lkey mytestkey1 = lkey("give me five");
    kvPair mytestkvpair = kvPair(mytestkey1,*c);
    database1.insert(mytestkvpair);
    auto mytestkvpair2 = database1.get(lkey("give me five"));
    cout<<*reinterpret_cast<int32_t*>((std::get<1>(mytestkvpair).valueStorage.value));
    return 0;
}