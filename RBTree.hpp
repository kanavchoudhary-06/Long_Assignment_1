#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <memory>
#include <string>
#include <vector>
#include "File.hpp"

template<typename K,typename V>
struct Node {
    K key;
    V val;
    int col;
    Node *lef, *righ, *par;
    Node(const K& k, const V& v, int c, Node<K,V>* p=nullptr){
        key = k;
        val = v;
        col = c;
        par = p;
        lef = righ = nullptr;
    }
    ~Node() {
        lef = righ = par = nullptr;
    }
};

template<typename K,typename V>
class Bucket_map {
    Node<K,V>* root;
    int sz;
    static int colOf(const Node<K,V>* n) { return n ? n->col : 0; }
    static void chng_cl(Node<K,V>* n, int c) { if (n) n->col = c; }
    static Node<K,V>* gp(const Node<K,V>* n) { return n && n->par ? n->par->par : nullptr; }
    
    void del_sub(Node<K,V>* n) {
        if(!n){ 
            return; 
        }
        if(n->lef){
            del_sub(n->lef);
        }
        if(n->righ){
            del_sub(n->righ);
        }
        delete n;
    }
    
    void inorder_collect(const Node<K,V>* n, std::vector<std::pair<K,V>>& result) const {
        if (!n) return;
        inorder_collect(n->lef, result);
        result.push_back({n->key, n->val});
        inorder_collect(n->righ, result);
    }
    
    Node<K,V>* look(const Node<K,V>* n, const K& k) const {
        if(!n){
            return nullptr;
        }
        if(k == n->key){
            return const_cast<Node<K,V>*>(n);
        }
        if(k < n->key){
            return look(n->lef, k);
        } 
        else {
            return look(n->righ, k);
        }
    }

    void rotate_l(Node<K,V>* x) {
        Node<K,V>* y = x->righ;
        x->righ = y->lef;
        if (y->lef) y->lef->par = x;
        y->par = x->par;
        if (!x->par) root = y;
        else if (x == x->par->lef) x->par->lef = y;
        else x->par->righ = y;
        y->lef = x;
        x->par = y;
    }

    void rotate_righ(Node<K,V>* x) {
        Node<K,V>* y = x->lef;
        x->lef = y->righ;
        if (y->righ) y->righ->par = x;
        y->par = x->par;
        if (!x->par) root = y;
        else if (x == x->par->righ) x->par->righ = y;
        else x->par->lef = y;
        y->righ = x;
        x->par = y;
    }

    void Rebalance(Node<K,V>* n) {
        while (n != root && colOf(n->par) == 1) {
            Node<K,V>* p = n->par;
            Node<K,V>* g = gp(n);
            if (g && p == g->lef) {
                Node<K,V>* u = g->righ;
                if (colOf(u) == 1) {
                    chng_cl(p, 0);
                    chng_cl(u, 0);
                    chng_cl(g, 1);
                    n = g;
                } else {
                    if (n == p->righ) {
                        n = p;
                        rotate_l(n);
                    }
                    chng_cl(p, 0);
                    chng_cl(g, 1);
                    rotate_righ(g);
                }
            } else if (g && p == g->righ) {
                Node<K,V>* u = g->lef;
                if (colOf(u) == 1) {
                    chng_cl(p, 0);
                    chng_cl(u, 0);
                    chng_cl(g, 1);
                    n = g;
                } else {
                    if (n == p->lef) {
                        n = p;
                        rotate_righ(n);
                    }
                    chng_cl(p, 0);
                    chng_cl(g, 1);
                    rotate_l(g);
                }
            }
        }
        chng_cl(root, 0);
    }

    Node<K,V>* insnode(const K& k, const V& v) {
        Node<K,V>* y = nullptr;
        Node<K,V>* x = root;
        while (x) {
            y = x;
            if (k < x->key) x = x->lef;
            else x = x->righ;
        }
        Node<K,V>* n = new Node<K,V>(k, v, 1, y);
        if (!y) root = n;
        else if (k < y->key) y->lef = n;
        else y->righ = n;
        ++sz;
        Rebalance(n);
        return n;
    }

    Node<K,V>* lookNode(const K& k) const {
        return look(root, k);
    }

public:
    bool contains(const K& k) const { 
        return lookNode(k) != nullptr; 
    }
    
    V get(const K& k) const {
        Node<K,V>* n = lookNode(k);
        return n ? n->val : V();
    }
    
    void insert(const K& k, const V& v) {
        insnode(k, v);
    }
    
    void get_all_elements(std::vector<std::pair<K,V>>& result) const {
        result.clear();
        inorder_collect(root, result); 
    }
    
    void clear() {
        del_sub(root);
        root = nullptr;
        sz = 0;
    }
    
    Bucket_map(){
        root = nullptr;
        sz = 0;
    }
    
    ~Bucket_map() {
        clear();
    }
};

#endif