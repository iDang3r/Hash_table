#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

extern "C" int  myStrlen(const char* s);
extern "C" char myStrcmp(const char* s1, const char* s2);

#define strlen myStrlen
#define strcmp myStrcmp

//#define ASMlen
//#define ASMcmp

#include "my_hash_table.h"
#include "file_lib.h"

class Str_on_poiners {
public:
    
    char* str_ = nullptr;
    int size_ = 0;
    
    Str_on_poiners(char* s = nullptr) {
        if (s == nullptr)
            return;
        
        str_ = s;
        
#ifdef ASMlen
        
        __asm__ volatile (
             
             "mov %1, %%rdi\n"
             "xor %%rcx, %%rcx\n"
             "mov %%rdi, %%rax\n"
             "1:\n"
             "movb (%%rax), %%cl\n"
             "inc %%rax\n"
             "cmp $0, %%ecx\n"
             "jne 1b\n"
             "sub %%rdi, %%rax\n"
             "dec %%rax\n"
             "mov %%eax, %0\n"
             "\n"
             "\n"

        :"=r"(size_)
        :"r"(s)
        : "%rdi", "%rcx", "%rax", "memory"
        );
        
#else
        size_ = (int)strlen(s);
#endif
    }
    
    Str_on_poiners(const Str_on_poiners& s) {
        
        if (s.size_ == 0) {
            
            size_ = 0;
            
        } else {
            
            str_ = s.str_;
            
            size_ = s.size_;
        }
        
    }
    
    Str_on_poiners& operator=(const Str_on_poiners& s) {
        
        if (this == &s) {
            return *this;
        }
        
        if (s.size_ == 0) {
            
            size_ = 0;
            
        } else {
        
            str_ = s.str_;
            
            size_ = s.size_;
        }
        
        return *this;
    }
    
    bool operator==(const Str_on_poiners& s) {
        if (size_ == 0 && s.size_ == 0)
            return true;
        if (size_ == 0 || s.size_ == 0)
            return false;
        return !strcmp(str_, s.str_);
    }
    
    ~Str_on_poiners() {
        str_ = nullptr;
    }
    
};

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
        if (size_ == 0 & s.size_ == 0)
            return true;
        if (size_ == 0 | s.size_ == 0)
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
    assert(size != 0);
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
    
    const int MAX_TABLE_SIZE = 5001;
    Hash_table<Str_on_poiners, size_t, MurMurHash2, MAX_TABLE_SIZE> Q;
    
    char* buff = nullptr;
    size_t buff_size = get_buffer("/Users/alex/Desktop/DeDD 2.txt", &buff);
    w(buff_size);
    
    if (buff == nullptr) {
        cout << "Input file was not open" << endl;
        return 0;
    }
    
    clock_t timer = clock();
    
    char* last = buff;
    
    for (size_t i = 0; i < buff_size; i++) {
        if (*(buff + i) == '\n') {
            *(buff + i) = '\0';
            Q[last] = i + 1;
            last = buff + i + 1;
        }
    }
    
//    istringstream in(buff);
    
    cout << "Fill time: " << double(clock() - timer) / CLOCKS_PER_SEC << endl;
    timer = clock();
    
    char* end = buff + buff_size;
    int c = 0;
    
    for (int k = 0; k < 10; k++) {
        
        last = buff;
        
        while (last < end) {
            
//            if (Q[last] == 0)
//                throw runtime_error("pops");
            Q[last];
            
#ifdef ASMlen
            
            asm volatile (
             
             "mov %1, %%rdi\n"
             "xor %%rcx, %%rcx\n"
             "mov %%rdi, %%rax\n"
             "repeat_:\n"
             "movb (%%rax), %%cl\n"
             "inc %%rax\n"
             "cmp $0, %%ecx\n"
             "jne repeat_\n"
             "sub %%rdi, %%rax\n"
             "mov %%eax, %0\n"
             "\n"
             "\n"

            :"=r"(c)
            :"r"(last)
            : "%rdi", "%rcx", "%rax"
            );
            
            last += c;
#else
            last += strlen(last) + 1;
#endif
            
        }
        
    }
    
    free(buff);
    
    cout << "Find time: " << double(clock() - timer) / CLOCKS_PER_SEC << endl;
    timer = clock();
    
//    ifstream in("/Users/alex/Desktop/DeDD 2.txt");
    ofstream out("/Users/alex/Desktop/Hash.csv");
        
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
//        cout << i << ": " << Q.table[i]->size_ << endl;
        out << Q.table[i]->size_ << "; ";
    }
    
    cout << "Out time: " << double(clock() - timer) / CLOCKS_PER_SEC << endl;
    
//    in.close();
    out.close();
    return 0;
}
