#ifndef DATA_STRUCT_WRAPPER_HPP
#define DATA_STRUCT_WRAPPER_HPP
#include "File.hpp"
#include <functional>
struct Val{
    File* file;
    int recent_heap;
    int biggest_heap_idx;
    std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* time_comp;
    std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* size_comp;
    Val(){
        file = nullptr;
        recent_heap = -1;
        biggest_heap_idx = -1;
        time_comp = nullptr;
        size_comp = nullptr;
    }
    Val(File* &f, int rec_in, int big_id, std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* &time_comp, std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* &size_comp){
        file=f;
        recent_heap=rec_in;
        biggest_heap_idx=big_id;
        this->time_comp=time_comp;
        this->size_comp=size_comp;
    }
    ~Val(){
        delete file;
        file = nullptr;
        time_comp = nullptr;
    }
    void update(int id, std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* &comp){
        if(comp==time_comp) recent_heap = id;
        else if(comp==size_comp) biggest_heap_idx = id;
    }
};
#endif