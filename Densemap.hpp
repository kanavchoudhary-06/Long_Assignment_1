#ifndef DENSEMAP_HPP
#define DENSEMAP_HPP

#include <vector>

template <typename T>
class Densemap{
    /** When using the hashmap to store the version of the file
     * as there are no deletions and the versions are assigned sequentially
     * the optimal way is to just use a vector as the hashmap and the
     * version id as the index.
     * This will give O(1) time complexity for get and set operations.
     * The space complexity will be O(n) where n is the number of versions.
     * This type of trivial implementation of Hashmap is for dense keys only.
     */
    private:
        std::vector<T> version_map;
    public:
        T get(int version) const {
            if (version<version_map.size()) {
                return version_map[version];
            }
            return nullptr;
        }
        void set(int version, T &node) {
            if (version < version_map.size()) {
                version_map[version] = node;
            }
            else{
                version_map.push_back(node);
            }
        }
        int size() const {
            return version_map.size();
        }
        ~Densemap(){
            while(!version_map.empty()){
                delete version_map.back();
                version_map.pop_back();
            }
        }
};
#endif