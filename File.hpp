#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <memory>
#include <iostream>
#include "Densemap.hpp"

class TreeNode{
    private:
        int version_id ;
        std::string content;
        std::string message; 
        time_t created_timestamp ;
        time_t snapshot_timestamp ; 
        TreeNode* parent;
        std::vector<TreeNode*> children;
    public:
        TreeNode(int vi, const std::string &cnt, TreeNode* par){
            version_id = vi;
            created_timestamp = time(nullptr);
            snapshot_timestamp = -1;
            parent = par;
            content = cnt;
            if(vi==0){
                snapshot_timestamp=time(nullptr);
                message="Root node of file";
            }
            else{
                parent = par;
            }
        }
        ~TreeNode(){
            std::cout<<"Version : "<< version_id <<" deleted\n";
            parent = nullptr;
            children.clear();
        }
        std::string access() const {
            return content;
            }
        bool is_snapshot() const {
            return snapshot_timestamp != -1;
            }
        int get_version() const {
            return version_id;
        }
        void access_snapshot() const {
            char read_time[80];
            strftime(read_time, sizeof(read_time), "%Y-%m-%d %H:%M:%S", localtime(&snapshot_timestamp));
            std::cout<<"id: "<< version_id<<" Snapshot Time: "<<read_time<<" Message: "<<message<<"\n";
        }
        void change_data(const std::string &data){
            content=data;
        }
        void add_data(std::string data) {
            content += data;
        }
        void push(TreeNode* node ){
            children.push_back(node);
        }
        void update(const std::string &message){
            this->message = message;
            this->snapshot_timestamp = time(nullptr);
        }
        void take_snap() {
            this->snapshot_timestamp = time(nullptr);
        }
        TreeNode* get_par(){
            return parent;
        }
};
class File{
    private:
        TreeNode* root;
        TreeNode* active_version ;
        Densemap<TreeNode*> version_map ;
        int total_versions ;
        time_t recent;
        std::string name;
    public:
        File(const std::string& name){
            root = new TreeNode(0,"", nullptr);
            active_version = root;
            this->name = name;
            total_versions = 1;
            recent = time(nullptr);
            version_map.set(0, root);
        }
        ~File(){
            std::cout<<"Deleting file: "<<name<<"\n";
            root=nullptr;
            active_version=nullptr;
        }
        time_t Last_Change() const {
            return recent;
        }
        std::string read() const {
            return active_version->access();
        }
        bool is_snapshot() const {
            return active_version->is_snapshot();
        }
        std::string get_name() const {
            return name;
        }
        void about() const {
            time_t t = recent;
            char read_time[80];
            strftime(read_time, sizeof(read_time), "%Y-%m-%d %H:%M:%S", localtime(&t));
            std::cout <<"Name : "<< name<<" versions: "<<total_versions<<" last changed: "<<read_time << "\n";
        }
        int getSize() const {
            return total_versions;
        }
        int get_id() const {
            return active_version->get_version();
        }
        void insert(const std::string &data){
            if(is_snapshot()){
                std::cout<<"New version "<<total_versions<<" created\n";
                TreeNode* node = new TreeNode(total_versions, read()+ data, active_version);
                active_version = node;
                version_map.set(node->get_version(), node);
                total_versions++;
            } 
            else {
                active_version->add_data(data);
            }
            recent = time(nullptr);
            std::cout<<"Inserted "<<data<<" into "<<name<<"\n";
        }
        void update(const std::string &data){
            if(active_version->is_snapshot()){
                std::cout<<"New version "<<total_versions<<" created\n";
                TreeNode* node = new TreeNode(total_versions, data, active_version);
                active_version = node;
                version_map.set(node->get_version(), node);
                total_versions++;
            } 
            else {
                active_version->change_data(data);
            }
            recent = time(nullptr);
            std::cout<<"Updated "<<data<<" in "<<name<<"\n";
        }
        void snapshot(const std::string &msg){
            if(active_version->is_snapshot()){
                std::cout<<"Error: version is already a snapshot\n";
            }
            else{
                active_version->update(msg);
                active_version->take_snap();
                recent = time(nullptr);// I have updated the recent time after snapshot missed the clarification message very simple fix by commenting this line all upstream updates would be nullified
                std::cout<<"Snapshot created of "<<name<<" with Message: "<<msg<<"\n";
            }
        }
        void rollback(int id=-1){
            if(id==-1){
                if(active_version==root){
                    std::cout<<"Error: cannot rollback root version\n";
                    return;
                }
                else{
                    active_version = active_version->get_par();
                    std::cout<<"Rolled back "<<name<<" to previous version\n";
                }
            }
            else if(id<total_versions && id>=0) {
                active_version = version_map.get(id);
                std::cout<<"Rolled back "<<name<<" to version "<<id<<"\n";
            }
            else{
                std::cout<<"Error: invalid version id\n";
            }
        }
        void history(){
            TreeNode* y=active_version;
            std::vector<TreeNode*> stack;
            while(y){
                if(y->is_snapshot()){
                    stack.push_back(y);
                }
                y = y->get_par();
            }
            while(!stack.empty()){
                stack.back()->access_snapshot();
                stack.pop_back();
            }
        }
};
#endif