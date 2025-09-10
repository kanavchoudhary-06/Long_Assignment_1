#ifndef HEAP_HPP
#define HEAP_HPP
#include <vector>
#include <memory>
#include <iostream>
#include "File.hpp"
#include "Data_struct_wrapper.hpp"
template<typename T>
class FileHeap {
    private:
    std::vector<T> heap;
    std::function<bool(T,T)>* comp_func;
    void bubble(int node) {
        while(node>0){
            int par =(node-1)/2;
            if ((*comp_func)(heap[node], heap[par])) {
                std::swap(heap[node], heap[par]);
                heap[par]->update(par, comp_func);
                heap[node]->update(node, comp_func);
                node=par;
            }
            else break;
        }
    }
    void drown(int node) {
        while (true) {
            int l=2*node+1;
            int r=2*node+2;
            int replace = node;
            if (l<heap.size() && (*comp_func)(heap[l],heap[replace])){ 
                replace=l;
            }
            if (r<heap.size() && (*comp_func)(heap[r],heap[replace])) { 
                replace=r;
            }
            if (replace != node) {
                std::swap(heap[node], heap[replace]);
                heap[replace]->update(replace, comp_func);
                heap[node]->update(node, comp_func);
                node=replace;
            } 
            else break;
        }
    }
    public:
        FileHeap(std::function<bool(T,T)>* comp_func){
            this->comp_func = comp_func;
        }
        ~FileHeap(){
            heap.clear();
            comp_func = nullptr;
        }
        void push(T x) {
            if (!x) return;
            heap.push_back(x);
            int idx = (int)heap.size() - 1;
            heap[idx]->update(idx, comp_func);
            bubble(idx);
        }
        void update(int y) {
            if((*comp_func)(heap[y], heap[(y-1)/2])){
                bubble(y);
            }
            else{drown(y);}
        }
        T pop(){
            if (empty()){
                return nullptr;
            }
            T r = heap[0];
            std::swap(heap[0], heap[heap.size()-1]);
            heap[0]->update(0, comp_func);
            heap[heap.size()-1]->update(heap.size()-1, comp_func);
            heap.pop_back();
            if (!empty()){
                drown(0);
            }
            return r;
        }
        T top() const {
            if (heap.size() == 0){
                return nullptr;
            }
            return heap[0];
        }
        bool empty() const {
            return heap.size() == 0;
        }
        int size() const { 
            return heap.size(); 
        }
        void ascending(int x){
            std::vector<T> popped;
            while(x-- && !empty()){
                T f = pop();
                f->file->about();
                popped.push_back(f);
            }
            while(!popped.empty()) {
                push(popped.back());
                popped.pop_back();
            }
        }
        void ascending(){
            ascending(heap.size());
        }
};
#endif