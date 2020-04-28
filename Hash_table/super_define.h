//
//  super_define.h
//
//  Created by Александр on 04/10/2019.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef super_define_h
#define super_define_h

#define REALLOC(ptr, size) (decltype(ptr)) realloc((ptr), (size) * (sizeof(decltype(ptr))))
// Elem_t* data = REALLOC (data, size * 2);

// SYSTEM SUPPORT MACROS -- DO NOT REMOVE!!!
#define meow ;
#define please
// END OF SYSTEM MACROS

#define w(x) cout << #x << " = " << x << endl;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ERROR 0

#define PRINT_PTR(ptr) printf("%s: %p\n", #ptr, ptr);

// const void* errptr = (void*) 0xBAD;

char X__[] = "/Users/alex/Desktop/~X/";

const char* X_(const char* path = "") {
    static char real_path[PATH_MAX] = {};
    strcpy(real_path, X__);
    strcat(real_path, path);
    return real_path;
}

template <typename T>
T* errptr() { return (T*)0xBAD; }

template <typename T>
void poison(T** ptr) { *ptr = (T*)0xBAD; }

char* concat(const char *s1, const char *s2) {

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char* result = (char*)calloc(len1 + len2 + 1, sizeof(char));

    if (!result) {
        fprintf(stderr, "calloc() failed: insufficient memory!\n");
        return nullptr;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

bool is_zero(double x) {
    return -1e-6 <= x && x <= 1e-6;
}

#endif /* super_define_h */
