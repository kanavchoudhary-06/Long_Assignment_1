#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <memory>
#include <iostream>
#include "Hashmap.hpp"
#include "Heap.hpp"

std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)> time_comp = [](std::shared_ptr<Val> a, std::shared_ptr<Val> b) {
    if(a->file->Last_Change() == b->file->Last_Change()) {
        return a->file->get_id() < b->file->get_id();
    }
    return a->file->Last_Change() > b->file->Last_Change();
};
std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)> size_comp = [](std::shared_ptr<Val> a, std::shared_ptr<Val> b) {
    if(a->file->getSize() == b->file->getSize()) {
        return a->file->get_id() < b->file->get_id();
    }
    return a->file->getSize() > b->file->getSize();
};
std::string clean_space(std::stringstream &ss){
    std::string name;
    std::getline(ss, name);
    int j=0;
    while(name[j]==' '){
        j++;
    }
    int k=name.size()-1;
    while(name[k]==' '){
        k--;
    }
    name = name.substr(j, k - j + 1);
    return name;
}
int main() {
    std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* time_comp_func = &time_comp;
    std::function<bool(std::shared_ptr<Val>, std::shared_ptr<Val>)>* size_comp_func = &size_comp;
    FileHashMap<std::string,std::shared_ptr<Val>> files;
    FileHeap<std::shared_ptr<Val>> recentFiles(time_comp_func);
    FileHeap<std::shared_ptr<Val>> biggestFiles(size_comp_func);
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        try{
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "CREATE") {
            std::string name= clean_space(ss);
            if (files.contains(name)) {
                std::cout << "Error: File already exists\n";
            } 
            else {
                File* file = new File(name);
                std::shared_ptr<Val> f = std::make_shared<Val>(file, recentFiles.size(), biggestFiles.size(), time_comp_func, size_comp_func);
                files.insert_fi(name, f);
                recentFiles.push(f);
                biggestFiles.push(f);
                std::cout << "File created: " << name << "\n";
            }
        }

        else if (cmd == "READ") {
            std::string name=clean_space(ss); 
            files.read(name);
        }

        else if (cmd == "INSERT") {
            std::string name, content="";
            std::string name_cont=clean_space(ss);
            int j=0;
            while(name_cont[j]!=' '){
                j++;
            }
            name= name_cont.substr(0,j);
            int k=j;
            while(name_cont[k]==' '){
                k++;
            }
            if(k < name_cont.size()) content=name_cont.substr(k, name_cont.size() - k);
            std::pair<int,int> id= files.insert(name, content);
            if(id.first != -1) recentFiles.update(id.first);
            if(id.second != -1) biggestFiles.update(id.second);
        }

        else if (cmd == "UPDATE") {
            std::string name, content="";
            std::string name_cont=clean_space(ss);
            int j=0;
            while(name_cont[j]!=' '){
                j++;
            }
            name= name_cont.substr(0,j);
            int k=j;
            while(name_cont[k]==' '){
                k++;
            }
            if(k < name_cont.size()) content=name_cont.substr(k, name_cont.size() - k);
            std::pair<int,int> id= files.update(name, content);
            if(id.first != -1) recentFiles.update(id.first);
            if(id.second != -1) biggestFiles.update(id.second);
        }
        else if (cmd == "SNAPSHOT") {
            std::string name;
            std::string msg=" ";
            std::string name_msg=clean_space(ss);
            int j=0;
            while(name_msg[j]!=' '){
                j++;
            }
            name= name_msg.substr(0,j);
            int k=j;
            while(name_msg[k]==' '){
                k++;
            }
            if(k < name_msg.size()) msg=name_msg.substr(k, name_msg.size() - k);
            files.snapshot(name, msg);
            std::pair<int,int> id = files.get_ids(name);
            if(id.first != -1) recentFiles.update(id.first);
        }
        else if (cmd == "ROLLBACK") {
            std::string name;
            std::string versionId;
            std::string name_ver=clean_space(ss);

            if(name_ver[name_ver.size()-1]>='0' && name_ver[name_ver.size()-1]<='9'){
                int k=name_ver.size()-1;
                while(name_ver[k]!=' '){
                    k--;
                }
                versionId = name_ver.substr(k+1, name_ver.size() - k - 1);
                int j=k;
                while(name_ver[j]==' '){
                    j--;
                }
                name=name_ver.substr(0,j+1);
                files.rollback(name, std::stoi(versionId));
            }
            else{
                files.rollback(name);
            }
        }

        else if (cmd == "HISTORY") {
            std::string name=clean_space(ss);
            files.history(name);
        }
        else if (cmd=="RECENT_FILES"){
            int d;
            if(ss>>d){
                recentFiles.ascending(d);
            }
            else{
                recentFiles.ascending();
            }
        }
        else if(cmd=="BIGGEST_TREES"){
            int d;
            if(ss>>d){
                biggestFiles.ascending(d);
            }
            else{
                biggestFiles.ascending();
            }
        }
        else if (cmd == "EXIT") {
            std::cout << "Exiting system and Deleting all files.\n";
            break;
        }
        else {
            std::cout << "Unknown command: " << cmd << "\n";
            std::cout << "Available commands are: INSERT, UPDATE, SNAPSHOT, ROLLBACK, HISTORY, RECENT_FILES, BIGGEST_TREES, EXIT\n";
        }
    }        
    catch (...) {
            std::cout << "Unknown error while parsing command\n";
        }
    }
}