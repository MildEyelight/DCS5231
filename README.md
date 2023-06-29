# Big Data Storage Technology(DSC5231)
This is the lesson design for dsc5231 of Sun-Yat sen University(Spring). The key function of this project implement a KV database with key and value both in string. The main data structure of storing these KV is LSM-Tree stored both in memory and disk.

By Using a LSM-Tree based KV-database. One can have a fast write-in response for it stores the operation log instead of changing the data directly.
## Requirments
1. C++ 14 or plus
2. Linux (I compile this project using Ubuntu20.04)
3. CMake make g++
## Usage
1. clone this repos
2. mkdir ./build
3. cd ./build
4. cmake.. && make
5. run main

