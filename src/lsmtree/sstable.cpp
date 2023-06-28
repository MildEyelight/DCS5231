#include "sstable.h"
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <string>
#include "lsmtree.h"

#include <fstream>

using std::vector;
using std::string;


// write in config
const string sstable::ss_file_path = "./sstable/";
const string sstable::ss_file_extension = ".sstable";
const string sstable::file_prefix = "level";
const int sstable::max_level = 3;
const int sstable::max_file_num_per_level = 4;
const int sstable::metadata_seg_length = 8;

string sstable::get_filename(int level, int order){
    string filename = sstable::ss_file_path + 
                       sstable::file_prefix + "_" + std::to_string(level) + "_" + std::to_string(order) +
                       sstable::ss_file_extension;
    return filename;
};

bool sstable::check_mergeable(int level){
    if(level == sstable::max_level) return false;
    return this->file_count_per_level[level] == sstable::max_file_num_per_level;
};

std::pair<uint32_t,uint32_t> sstable::get_metadata(std::ifstream& file){
    file.seekg(sstable::metadata_seg_length,std::ios::end);
    char* s = new char[sstable::metadata_seg_length];
    file.read(s,sizeof(uint32_t)+sizeof(uint32_t));
    uint32_t* node_count = reinterpret_cast<uint32_t*>(s);
    uint32_t* key_seg = reinterpret_cast<uint32_t*> (s+sizeof(uint32_t));
    std::pair<uint32_t,uint32_t> a(*node_count,*key_seg);
    delete[] s;
    return a;
}


sstable::sstable():file_count_per_level(std::vector<int>(4)){
    for (int i = 0; i <= sstable::max_level ; i++){
        string target = "level_" + std::to_string(i);
        int count = 0;
        
        DIR* dir = opendir(sstable::ss_file_path.c_str());
        // create the folder
        if(dir == nullptr){
            int result = mkdir(sstable::ss_file_path.c_str(),0766);
        }
        
        dir = opendir(sstable::ss_file_path.c_str());

        dirent* entry;
        while((entry = readdir(dir)) != nullptr){
            string filename = entry->d_name;
            if(filename.find(target) != string::npos){
                count ++;
            }
        }
        // cpp17
        // for(const auto& entry: std::filesystem::directory_iterator(sstable::ss_file_path)){
        //     string filename = entry.path().filename().string();
        //     if(filename.find(target) != string::npos){
        //         count ++;
        //     }
        // }
        this->file_count_per_level[i] = count;
    }
};

lsmtree* sstable::sstable_to_mmtable(int level,int order){
    lsmtree* restored_sstable = new lsmtree();
    string file_name = this->get_filename(level,order);
    std::ifstream sstable_file(file_name,std::ios::in);

    //read file by the format of .sstable file.
    //get meta data
    std::pair<uint32_t,uint32_t> metadata = this->get_metadata(sstable_file);
    uint32_t node_count = metadata.first;
    uint32_t key_seg = metadata.second;
    
    sstable_file.seekg(0,std::ios::beg);
    for(int i = 0; i < node_count;i++){
        //get key 
        char _key_length[sizeof(uint32_t)];
        sstable_file.read(_key_length,sizeof(uint32_t));
        uint32_t key_length = *(reinterpret_cast<uint32_t*>(_key_length));
        char* _key = new char [key_length + 1];
        _key[key_length] = '\0';
        sstable_file.read(_key,key_length);
        string key((_key));
        delete[] _key;

        //get value
        char _value_length[sizeof(uint32_t)];
        sstable_file.read(_value_length,sizeof(uint32_t));
        uint32_t value_length = *(reinterpret_cast<uint32_t*>(_value_length));
        char* _value = new char [value_length + 1];
        _value[value_length] = '\0';
        sstable_file.read(_value,value_length);
        string value((_value));
        delete[] _value;

        char _operation_log;
        sstable_file.read(&_operation_log,sizeof(uint8_t));
        uint8_t operation_log = *(reinterpret_cast<uint8_t*>(&_operation_log));
        oprand log;
        if(operation_log == 0){
            log = oprand::SET;
        }
        else if(operation_log == 2){
            log = oprand::REMOVE;
        }
        Value tmp(value,log);
        std::pair<string,Value> insert_pair(key,tmp);
        restored_sstable->set(insert_pair);
    }
    sstable_file.close();
    return restored_sstable; 
}

void sstable::merge_sstable(int level){
    // merge the level-th level .sstable file.
    vector<lsmtree*> tree_to_merge(sstable::max_file_num_per_level);
    for(int i = 0; i<sstable::max_file_num_per_level; i++){
        tree_to_merge[i] = this->sstable_to_mmtable(level,i);
    }
    for(int i = 1; i<sstable::max_file_num_per_level; i++){
        auto tree = tree_to_merge[i]->get_tree();
        for(auto itr = tree.begin(); itr!=tree.end(); itr++){
            tree_to_merge[0]->set(*itr);
        }
    }
    this->save_memtable(tree_to_merge[0],level + 1);

    //delete merged file.
    this->file_count_per_level[level] = 0;
    for(int i = 0; i< sstable::max_file_num_per_level; i++){
        string file_name = this->get_filename(level,i);
        std::remove(file_name.c_str());
    }
    for(auto a : tree_to_merge){
        delete a;
    }

    return;
}

void sstable::save_memtable(lsmtree* tree_to_save, int level){
    //Can be written in multithread: merge and save
    if(this->check_mergeable(level)){
        this->merge_sstable(level);   
    }
    int insert_target_level = level;
    int insert_file_order = this->file_count_per_level[insert_target_level];
    this->file_count_per_level[insert_target_level]++;
    string saved_filename = this->get_filename(insert_target_level,insert_file_order);

    std::ofstream outfile(saved_filename,std::ios::out);
    
    
    //Save data segment and data_ptr
    uint32_t node_count = tree_to_save->get_node_number();
    vector<uint32_t> data_length(node_count);
    map<string,Value> t = tree_to_save->get_tree();
    int i = 0;
    for(auto a = t.begin(); a != t.end(); a++,i++){
        string key = a->first;
        Value value = a->second;
        string value_str = value.get_value();
        uint32_t key_length = key.size();
        uint32_t value_length = value_str.size();
        uint8_t value_oprand = value.get_oprand();

        outfile.write(reinterpret_cast<char*> (&key_length), sizeof(key_length));
        outfile.write(key.c_str(), key_length);
        outfile.write(reinterpret_cast<char*> (&value_length), sizeof(value_length));
        outfile.write(value_str.c_str(), value_length);
        outfile.write(reinterpret_cast<char*> (&value_oprand), sizeof(value_oprand));
        data_length[i] = 4 + key_length + 4 + value_length + 1;
    }
    
    //Save key segment
    uint32_t seg_ptr = 0;
    i = 0;
    for(auto a = t.begin(); a!=t.end(); a++,i++){
        string key = a->first;
        uint32_t key_length = key.size();
        uint32_t data_ptr = seg_ptr;
        seg_ptr += data_length[i];

        outfile.write(reinterpret_cast<char*> (&key_length), sizeof(key_length));
        outfile.write(key.c_str(),key_length);
        outfile.write(reinterpret_cast<char*> (&data_ptr),sizeof(data_ptr));
    
    }

    //Save meta data
    outfile.write(reinterpret_cast<char*> (&node_count), sizeof(node_count));
    outfile.write(reinterpret_cast<char*> (&seg_ptr),sizeof(&seg_ptr));

    outfile.close();
    return;
}


std::pair<Value,bool> sstable::find_key_in_file(int level,int order,const std::string& search_key){
    string filename = this->get_filename(level,order);
    std::ifstream search_file(filename,std::ios::in);
    std::pair<uint32_t,uint32_t> metadata = this->get_metadata(search_file);
    uint32_t node_count = metadata.first;
    uint32_t key_seg = metadata.second;

    search_file.seekg(key_seg,std::ios::beg);
    char _key_length[sizeof(uint32_t)];
    char _data_ptr[sizeof(uint32_t)];
    for(int i = 0; i < node_count;i++){
        //get key 
        search_file.read(_key_length,sizeof(uint32_t));
        uint32_t key_length = *(reinterpret_cast<uint32_t*>(_key_length));
        char* _key = new char [key_length + 1];
        _key[key_length] = '\0';
        search_file.read(_key,key_length);
        string key((_key));
        if(search_key==key) {
            search_file.read(_data_ptr,sizeof(uint32_t));
            uint32_t data_ptr = *(reinterpret_cast<uint32_t*>(_data_ptr));
            search_file.seekg(data_ptr,std::ios::beg);
            char _value_length[sizeof(uint32_t)];

            //Copy from ... consider fileparser.
            search_file.read(_value_length,sizeof(uint32_t));
            uint32_t value_length = *(reinterpret_cast<uint32_t*>(_value_length));
            char* _value = new char [value_length + 1];
            _value[value_length] = '\0';
            search_file.read(_value,value_length);
            string value((_value));
            delete[] _value;

            char _operation_log;
            search_file.read(&_operation_log,sizeof(uint8_t));
            uint8_t operation_log = *(reinterpret_cast<uint8_t*>(&_operation_log));
            oprand log;
            if(operation_log == 0){
                log = oprand::SET;
            }
            else if(operation_log == 2){
                log = oprand::REMOVE;
            }
            Value tmp(value,log);
            delete[] _key;
            return std::pair<Value,bool>(tmp,true) ;
        }
    }
    return std::pair<Value,bool>(Value(),false);
}

std::pair<Value,bool> sstable::find(const string& key){
    std::pair<Value,bool> result(Value(),false);
    for(int level = 0; level< sstable::max_level;level ++){
        for(int order = this->file_count_per_level[level] - 1 ; order>=0;order--){
            result = this->find_key_in_file(level,order,key);
            if(result.second) break;
        }
        if(result.second) break;
    }
    //Not found
    return result;
}

