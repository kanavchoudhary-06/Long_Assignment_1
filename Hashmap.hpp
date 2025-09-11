#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "RBTree.hpp"
template<typename K, typename V>
struct buck{
    std::vector<std::pair<K,V>> fi;
    Bucket_map<K,V> fi_map;
    bool flag=false;
    bool tree_off=false;
    void push(const K& h, const V& f) {
        if(flag){
            fi_map.insert(h, f);
        }
        else{
            fi.push_back(std::make_pair(h,f));
            if(fi.size()>2 && !tree_off){
                flag=true;
                while(fi.size()){
                    std::pair<K,V> p=fi.back();
                    fi.pop_back();
                    fi_map.insert(p.first, p.second);
                }
            }
        }
    }
    bool contains(const K &g){
        if(flag){
            return fi_map.get(g)!=nullptr;
        }
        else{
            for (const auto &p : fi) {
                if (p.first == g) return true;
            }
        }
        return false;
    }
    V get(const K &g){
        if(flag){
            return fi_map.get(g);
        }
        else{
            for (const auto &p : fi) {
                if (p.first == g) return p.second;
            }
        }
        return nullptr;
    }
    void transfer_tree_to_vector() {
        if(flag && tree_off) {
            std::vector<std::pair<K,V>> tree_elements;
            fi_map.get_all_elements(tree_elements);
            for(const auto& elem : tree_elements) {
                fi.push_back(std::make_pair(elem.first, elem.second));
            }
            fi_map.clear();
            flag = false;
        }
    }
    ~buck(){}
};
template<typename K, typename V>
class FileHashMap {
    std::vector<buck<K,V>> buckets;
    int capacity;
    int count;
    int hashing(const K &s){
        unsigned int hash = 0;
        for (char c : s) {
            hash = (hash * 31 + c) % capacity;
        }
        return hash;
    }
    V get(const K &k){
        int idx = hashing(k);
        return buckets[idx].get(k);
    }
public:
    FileHashMap(){
        capacity = 1;
        count = 0;
        buckets.resize(capacity);
    }
    
    void tree_off(){
        for (auto &bucket : buckets) {
            bucket.tree_off=true;
            bucket.transfer_tree_to_vector();
        }
    }
    void rehash() {
        int old_capacity = capacity;
        capacity = capacity * 5 + 1;
        std::vector<buck<K,V>> old_buckets = std::move(buckets);
        buckets.clear();
        buckets.resize(capacity);
        count = 0;
        for(auto& bucket : old_buckets) {
            for(const auto& pair : bucket.fi) {
                insert_fi(pair.first, pair.second);
            }
            if(bucket.flag && !bucket.tree_off) {
                std::vector<std::pair<K,V>> tree_elements;
                bucket.fi_map.get_all_elements(tree_elements);
                for(const auto& elem : tree_elements) {
                    insert_fi(elem.first, elem.second);
                }
            }
        }
    }
    bool contains(const K &k){
        int idx = hashing(k);
        return buckets[idx].contains(k);
    }
    void insert_fi(const K &k, const V& v) {
        int idx = hashing(k);
        buckets[idx].push(k, v);
        count++;
        if(count > 2* capacity) {
            rehash();
        }
    }

    std::pair<int,int> insert(const K &fn, const K &data){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return {-1,-1};
        }
        V f = get(fn);
        f->file->insert(data);
        return {f->recent_heap, f->biggest_heap_idx};
    }
    std::pair<int,int> update(const K &fn,const K &data){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return {-1,-1};
        }
        V f = get(fn);
        f->file->update(data);
        return {f->recent_heap, f->biggest_heap_idx};
    }
    std::pair<int,int> snapshot(const K &fn,const K &msg){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return {-1,-1};
        }
        V f = get(fn);
        f->file->snapshot(msg);
        return {f->recent_heap, f->biggest_heap_idx};
    }

    void rollback(const K &fn, int id){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return; 
        }
        V f = get(fn);
        f->file->rollback(id);
    }
    void rollback(const K &fn){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return; 
        }
        V f = get(fn);
        f->file->rollback(); 
    }
    void history(const K &fn){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; 
            return; 
        }
        V f = get(fn);
        f->file->history();
    }
    void read(const K &fn){
        if(!get(fn)){ 
            std::cout<<"Error: file not found\n"; return; 
        }
        V f = get(fn);
        std::cout<<f->file->read()<<'\n';
    }
};
#endif 