#cmake file for project association 
#author: liyh for dsc5231
#created: 2023/5/21

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(DCS5231)

#编译类型
SET(CMAKE_BUILD_TYPE DEBUG)

#语言版本
SET(CMAKE_CXX_STANDARD 14)

#编译选项
# add_compile_options()
# set(CMAKE_CXX_FLAGS )

#添加全局宏定义
# add_definitions()


#头文件搜索目录
INCLUDE_DIRECTORIES(./src)
INCLUDE_DIRECTORIES(./src/lsmtree/)
INCLUDE_DIRECTORIES(./src/store)

#编译静态库
# file(GLOB_RECURSE STORE_LIB ./src/store/*.cpp)
# add_library(store STATIC ${STORE_LIB})

# file(GLOB_RECURSE LSMTREE_LIB ./src/lsmtree/*.cpp)
# add_library(lsmtree STATIC ${LSMTREE_LIB})

AUX_SOURCE_DIRECTORY(./src DIR_SRC)
aux_source_directory(./src/lsmtree LSM_SRC)
aux_source_directory(./src/store STORE_SRC)

# add_executable(main ${DIR_SRC})
# ADD_EXECUTABLE(main ${DIR_SRC} ${LSM_SRC} ${STORE_SRC})

add_executable(main ${DIR_SRC} ${LSM_SRC} ${STORE_SRC})
# target_link_libraries(main )
