#ifndef __SSTABLE_H__
#define __SSTABLE_H__
#include <string>

//cpp17?
//#include <filesystem>

#include <array>
#include <vector>
#include "lsmtree.h"

#include <dirent.h>
#include <sys/stat.h>
using std::string;
using std::array;

//Consider Singleton ? 
class sstable{
private:
    static const string ss_file_path;
    static const string ss_file_extension;
    // Stored file name format 
    // level + _ + $(ss_level) + _ + $(order) + .sstable
    // example: level_0_1.sstable refer to the secode file in level 0.
    static const string file_prefix;

    static const int max_level;
    static const int max_file_num_per_level;
    static const int metadata_seg_length;
    std::vector<int> file_count_per_level;
private:
    string get_filename(int level, int order);
    bool check_mergeable(int level);
    void merge_sstable(int level);
    lsmtree* sstable_to_mmtable(int level, int order);
    std::pair<Value,bool> find_key_in_file(int level,int order);
public:
    sstable();//Initialize the control info of the sstable
    sstable(const sstable& a) = delete;
    std::pair<Value,bool> find(const string& key);
    
    //Save the lsmtree as the sstable

    /*
       |      data segment   | key segment | meta data  |
       |      data + oprand  | key + index | meta data  | 
       data info      : length(4B-char) + data (?B-char) + oprand(1B-char)
       key info       : length(4B-uint32_t) + key  (?B-char) + data_ptr(4B-uint32_t) 
       meta_data info : node count(4B-uint32_t) + key segment begin (4B-uint32_t)
    */
    void save_memtable(lsmtree* tree_to_save, int level = 0);

};

// write in config
const string sstable::ss_file_path = "./sstable/";
const string sstable::ss_file_extension = ".sstable";
const string sstable::file_prefix = "level";
const int sstable::max_level = 3;
const int sstable::max_file_num_per_level = 4;
const int sstable::metadata_seg_length = 8; 

#endif