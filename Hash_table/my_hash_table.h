#ifndef hy_hash_table_h
#define hy_hash_table_h

#include "my_list.h"
#include <type_traits>
using namespace std;

template <typename K, typename V, size_t(*get_hash)(void* p, size_t size), size_t table_size>
class Hash_table {
public:
    
    struct LNode {
        
        K key_;
        V value_;
        
        LNode() : key_(nullptr), value_(0) {}
        LNode(K key, V value) : key_(key), value_(value) {}
        
        bool operator==(const LNode& N) {
            return key_ == N.key_;
        }
    };
    
    const size_t size_ = table_size;
    
    List<LNode>** table = nullptr;
    
//public:
    
    Hash_table() {
        
        table = new List<LNode>*[table_size];
        
        for (size_t i = 0; i < size_; i++)
            table[i] = new List<LNode>;
        
    }
    
    ~Hash_table() {
        
        for (size_t i = 0; i < size_; i++)
            delete table[i];
        
        delete [] table;
    }
    
    size_t size() {
        return size_;
    }
    
    V& operator[](const K& key) {
        
        size_t hash_tmp = 0;
        
        hash_tmp = get_hash((void*)key.str_, key.size_);
        
        size_t pos = hash_tmp % table_size;
        
        LNode to_find(key, 0);
        LNode& N = table[pos]->find_by_value(to_find);
        
        if (N == table[pos]->end_) {
            
            int pos_in_list = table[pos]->push_back(to_find);
            
            return table[pos]->data_[pos_in_list].value_;
        }
        
        return N.value_;
    }
    
};

#endif /* hy_hash_table_h */
