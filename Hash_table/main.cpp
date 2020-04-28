#include <iostream>
#include <fstream>
using namespace std;

#include "my_hash_table.h"

class Str {
public:
    
    char* str_ = nullptr;
    size_t size_ = 0;
    
    Str(const char* s = nullptr) {
        str_ = (char*)calloc(64, 1);
        if (s == nullptr)
            return;
        
        size_ = strlen(str_);
        memcpy(str_, s, size_ + 1);
    }
    
    Str(const Str& s) {
        
        if (s.size_ == 0) {
            
            size_ = 0;
            
        } else {
            
            if (str_ == nullptr)
                str_ = (char*)calloc(64, 1);
        
            memcpy(str_, s.str_, s.size_ + 1);
            size_ = s.size_;
        }
        
    }
    
    Str& operator=(const Str& s) {
        
        if (this == &s) {
            return *this;
        }
        
        if (s.size_ == 0) {
            
            size_ = 0;
            
        } else {
        
            if (str_ == nullptr)
                str_ = (char*)calloc(64, 1);
            
            memcpy(str_ , s.str_, s.size_ + 1);
            size_ = s.size_;
        }
        
        return *this;
    }
    
    bool operator==(const Str& s) {
        if (size_ == 0 && s.size_ == 0)
            return true;
        if (size_ == 0 || s.size_ == 0)
            return false;
        return !strcmp(str_, s.str_);
    }
    
    ~Str() {
        if (str_ != nullptr) {
            free(str_);
            str_ = nullptr;
        }
    }
    
    friend std::ostream& operator<<(std::ostream& out, const Str& s);
    friend std::istream& operator>>(std::istream& in, Str& s);
};

std::ostream& operator<<(std::ostream& out, const Str& s) {
    if (s.str_ != nullptr)
        out << s.str_;
    return out;
}

std::istream& operator>>(std::istream& in, Str& s) {
    if (s.str_ == nullptr)
        s.str_ = (char*)calloc(64, 1);
    in >> s.str_;
    s.size_ = strlen(s.str_);
    return in;
}

size_t hash_pointer(void* p, size_t size) {
    return (size_t)p;
}

size_t hash_lenght(void* p, size_t size) {
    return size;
}

size_t special_ded(void* p, size_t size) {
    size_t res = 0;
    for (size_t i = 0; i < size; i++)
        res += *((char*)p + i);
    return res / size;
}

size_t specspecspecial(void* p, size_t size) {
    size_t res = 0;
    for (int i = 0; i < size; i += 4) {
        res ^= *(size_t*)p;
    }
    return res;
}

int main(int argc, const char* argv[]) {
    
    const int MAX_TABLE_SIZE = 101;
    Hash_table<Str, size_t, hash_pointer, MAX_TABLE_SIZE> Q;
    
    ifstream in("/Users/alex/Desktop/DeDD 2.txt");
    ofstream out("/Users/alex/Desktop/Hash.csv");
    
    Str s;
    
    for (int i = 0; in >> s && i < 1000; i++) {
        
        if (i % 50'000 == 0)
            cout << i << endl;
//        in >> s;
        Q[s] = rand();
        
//        cout << s << endl;
    }
    
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        cout << i << ": " << Q.table[i]->size_ << endl;
        out << Q.table[i]->size_ << "; ";
    }
    
    in.close();
    out.close();
    return 0;
}
