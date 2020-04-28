//
// Created by Александр on 05.11.2019.
//

#ifndef LIST_MY_LIST_H
#define LIST_MY_LIST_H

#include "super_define.h"
#include "my_list_enums.h"
#include "MurMurHash.h"
#include "dump.h"

#define LIST(x) List x(#x);

//typedef int elem_t;

const long long Kanareyka_const = LONG_LONG_MAX;


//#define ASSERT_OK {                         \
//    if (errno_) {                           \
//        dump_(DUMP_INFO, "ASSERT_OK");      \
//        assert(ERROR);                      \
//    }                                       \
//    errno_ = is_ok();                       \
//    if (errno_) {                           \
//        dump_(DUMP_INFO, "ASSERT_OK");      \
//        assert(ERROR);                      \
//    }                                       \
//};                                          \
//
//#undef ASSERT_OK
#define ASSERT_OK

template <typename elem_t>
class List {
public:
    
    long long kanareyka1_= Kanareyka_const;

    static const int MAX_SIZE_ = 50000;
//    const elem_t     poison_val_ = INT_MAX;

    int size_ = 0;

    elem_t* data_ = nullptr;
    int* next_    = nullptr;
    int* prev_    = nullptr;

    int head_ = 0;
    int tail_ = 0;
    int free_ = 1;

    bool is_sorted_ = true;

    char* name_ = nullptr;
    int  errno_ = 0;

    long long kanareyka2_= Kanareyka_const;
    
    elem_t end_ = elem_t();

    size_t list_hash_ = 0;
    size_t data_hash_ = 0;
    size_t next_hash_ = 0;
    size_t prev_hash_ = 0;

    List(const char* name = "NO_NAME");

    ~List();

    void dump_(const char* file, const char* func, int line, const char* flag = "OK");

    void dump_picture(bool with_free = false);

    int add_first_elem(const elem_t& value);

    int push_back(const elem_t& value);

    int push_front(const elem_t& value);

    int insert_after(int pos, const elem_t& value);

    int insert_before(int pos, const elem_t& value);

    int pop_back();

    int pop_front();

    int erase(int del_pos);

    int find_index_by_value(const elem_t& value);
    
    elem_t& find_by_value(const elem_t& value);

    int front_index();

    int back_index();

    elem_t front();

    elem_t back();

    int size();

    int get_next_index(int pos);

    int get_prev_index(int pos);

    elem_t operator[](int num);

    void sort();

    int is_ok();

    void rehash();

};

/*!
 * Constructor for List
 *
 * @param[in] name name of List (For debug, default -> "NO_NAME")
 */

template <typename elem_t>
List<elem_t>::List(const char* name) {
    data_ = (elem_t*)calloc(MAX_SIZE_ + 1, sizeof(elem_t));
    next_ =    (int*)calloc(MAX_SIZE_ + 1, sizeof(int));
    prev_ =    (int*)calloc(MAX_SIZE_ + 1, sizeof(int));
    name_ =   (char*)calloc(strlen(name) + 1, sizeof(char));

    if (data_ == nullptr) {
        dump(DUMP_INFO, "Memory for data_ was not allocated!");
        assert(ERROR);
    }

    if (next_ == nullptr) {
        dump(DUMP_INFO, "Memory for next_ was not allocated!");
        assert(ERROR);
    }

    if (prev_ == nullptr) {
        dump(DUMP_INFO, "Memory for prev_ was not allocated!");
        assert(ERROR);
    }

    if (name_ == nullptr) {
        dump(DUMP_INFO, "Memory for name_ was not allocated!");
        assert(ERROR);
    }

    strcpy(name_, name);
    
    end_ = data_[0];

    // filling array for free pointer

    free_ = 1;
    for (int i = 1; i < MAX_SIZE_; i++)
        next_[i] = i + 1;
    next_[MAX_SIZE_] = 0;

//    for (int i = 1; i <= MAX_SIZE_; i++)
//        data_[i] = poison_val_;

    rehash();
}

/*!
 * Destructor for List
 *
 * includes security from double destructor
 */

template <typename elem_t>
List<elem_t>::~List() {
    if (data_ == errptr<elem_t>()) {
        dump(DUMP_INFO, "DOUBLE DESTRUCTOR! or double free of data_");
        assert(ERROR);
    }

    if (next_ == errptr<int>()) {
        dump(DUMP_INFO, "DOUBLE DESTRUCTOR! or double free of next_");
        assert(ERROR);
    }

    if (prev_ == errptr<int>()) {
        dump(DUMP_INFO, "DOUBLE DESTRUCTOR! or double free of prev_");
        assert(ERROR);
    }

    if (name_ == errptr<char>()) {
        dump(DUMP_INFO, "DOUBLE DESTRUCTOR! or double free of name_");
        assert(ERROR);
    }

    free(data_);
    free(next_);
    free(prev_);
    free(name_);

    poison(&data_);
    poison(&next_);
    poison(&prev_);
    poison(&name_);
}

/*!
 * Default dump for List
 *
 * prints file, func, line from call with flag (default -> "OK")
 * prints errno_ with description
 *
 * @param[in] file full name of file
 * @param[in] func name of function
 * @param[in] line line
 * @param[in] flag message of dump
 */

template <typename elem_t>
void List<elem_t>::dump_(const char* file, const char* func, int line, const char* flag) {
    printf("<<<-------- DUMP -------->>>\n");

    printf("File: %s\n", file);
    printf("Function: %s\n", func);
    printf("Line: %d\n", line);
    printf("List \"%s\"[%p] (%s) {\n", name_, this, flag);

    printf("Errno_ is: %d\n", errno_);
    printf("Message: \033[1;31m");

    printf("DUMP");

    printf("\033[0m\n");

    // TODO add print errno_ message

    printf("}\n");

    printf("<<<-----END OF DUMP ----->>>\n");
}

/*!
 * Picture dump
 *
 * compiles SVG file and opens it, you can change settings of command to get PNG file
 *
 * @param[in] with_free true if you also need to show free 'elements' (default -> false)
 */

template <typename elem_t>
void List<elem_t>::dump_picture(bool with_free) {
    FILE* out = fopen("/Users/alex/Desktop/pictures/list_uml.pu", "w");

    if (out == nullptr) {
        dump(DUMP_INFO, "file not found");
        return;
    }

    fprintf(out, "@startuml\n !define DARKORANGE\n !include style.puml\n class head_\n");

    fprintf(out, "note right: size_: %d\n", size_);

    fprintf(out, "class tail_\n");

    fprintf(out, "note left: free_: %d\n", free_);

    for (int i = head_, logic = 1; i != 0; i = next_[i], logic++) {
        fprintf(out, "class %d {\n", i);
        fprintf(out, "logic: %d\n", logic);
        fprintf(out, "--\n");
        fprintf(out, "value: %d\n", data_[i]);
        fprintf(out, "--\n");
        fprintf(out, "next: %d\n", next_[i]);
        fprintf(out, "--\n");
        fprintf(out, "prev: %d\n", prev_[i]);
        fprintf(out, "}\n");
    }

    fprintf(out, "head_ ..> %d\n", head_);

    for (int i = head_; i != tail_; i = next_[i]) {
        fprintf(out, "%d -> %d\n", i, next_[i]);
    }

    for (int i = tail_; i != head_; i = prev_[i]) {
        fprintf(out, "%d -> %d\n", i, prev_[i]);
    }

    fprintf(out, "tail_ ..> %d\n", tail_);

    if (with_free) {
        fprintf(out, "class free_\n");

        for (int i = free_; i != 0; i = next_[i]) {
            fprintf(out, "class %d {\n", i);
            fprintf(out, "next: %d\n", next_[i]);
            fprintf(out, "}\n");
        }

        fprintf(out, "free_ ..> %d\n", free_);
        for (int i = free_; next_[i] != 0; i = next_[i]) {
            fprintf(out, "%d -> %d\n", i, next_[i]);
        }

        fprintf(out, "%d -[hidden]-> free_\n", head_);
    }

    fprintf(out, "@enduml\n");

    fclose(out);
    system("java -jar --illegal-access=warn /Users/alex/plantuml.jar -tsvg /Users/alex/Desktop/pictures/list_uml.pu");
    system("open /Users/alex/Desktop/pictures/list_uml.svg");
}

/*!
 * Default libruary function to insert first element (if List is empty)
 *
 * @param[in] value value for inserting
 * @return phisical number of inserted element
 */

template <typename elem_t>
int List<elem_t>::add_first_elem(const elem_t& value) {
    ASSERT_OK

//    if (free_ == 0 || data_[free_] != poison_val_) {
//        dump(DUMP_INFO, "failed free_ pointer");
//        return -1;
//    }

    if (size_ != 0) {
        dump(DUMP_INFO, "impossible call for add_first_elem");
        return -1;
    }

    int new_pos = free_;
    free_ = next_[free_];

    head_ = tail_ = new_pos;

    next_[new_pos] = prev_[new_pos] = 0;

    data_[new_pos] = value;

    size_++;

    rehash();

    ASSERT_OK
    return new_pos;
}

/*!
 * Pushs the element at the end of List
 *
 * @param[in] value value for adding
 * @return phisical number of added element
 */

template <typename elem_t>
int List<elem_t>::push_back(const elem_t& value) {
    ASSERT_OK

    if (free_ == 0) {
        dump(DUMP_INFO, "List is full");
        return -1;
    }

    if (size_ == 0) {
        return add_first_elem(value);
    }

//    if (data_[free_] != poison_val_) {
//        dump(DUMP_INFO, "failed free_ pointer");
//        return -1;
//    }

    is_sorted_ = false;

    int new_pos = free_;
    free_ = next_[free_];

    next_[tail_] = new_pos;
    prev_[new_pos] = tail_;
    next_[new_pos] = 0;
    tail_ = new_pos;

    data_[new_pos] = value;

    size_++;

    rehash();

    ASSERT_OK
    return new_pos;
}

/*!
 * Pushs the element at the front of List
 *
 * @param[in] value value for adding
 * @return phisical number of added element
 */

template <typename elem_t>
int List<elem_t>::push_front(const elem_t& value) {
    ASSERT_OK

    if (free_ == 0) {
        dump(DUMP_INFO, "List is full");
        return -1;
    }

    if (size_ == 0) {
        return add_first_elem(value);
    }

//    if (data_[free_] != poison_val_) {
//        dump(DUMP_INFO, "failed free_ pointer");
//        return -1;
//    }

    is_sorted_ = false;

    int new_pos = free_;
    free_ = next_[free_];

    prev_[head_] = new_pos;
    next_[new_pos] = head_;
    prev_[new_pos] = 0;
    head_ = new_pos;

    data_[new_pos] = value;

    size_++;

    rehash();

    ASSERT_OK
    return new_pos;
}

/*!
 * Pushs the element after the set position in the List
 *
 * @param[in] value value for inserting
 * @return phisical number of inserted element
 */

template <typename elem_t>
int List<elem_t>::insert_after(int pos, const elem_t& value) {
    ASSERT_OK

    if (pos <= 0 || pos > MAX_SIZE_/* || data_[pos] == poison_val_ */) {
        dump(DUMP_INFO, "undefined element with index: pos");
        return -1;
    }

    if (free_ == 0) {
        dump(DUMP_INFO, "List is full");
        return -1;
    }

    if (pos == tail_) {
        return push_back(value);
    }

//    if (data_[free_] != poison_val_) {
//        dump(DUMP_INFO, "failed free_ pointer");
//        return -1;
//    }

    is_sorted_ = false;

    int new_pos = free_;
    free_ = next_[free_];

    prev_[next_[pos]] = new_pos;
    prev_[new_pos] = pos;
    next_[new_pos] = next_[pos];
    next_[pos] = new_pos;

    data_[new_pos] = value;

    size_++;

    rehash();

    ASSERT_OK
    return new_pos;
}

/*!
 * Pushs the element before the set position in the List
 *
 * @param[in] value value for inserting
 * @return phisical number of inserted element
 */

template <typename elem_t>
int List<elem_t>::insert_before(int pos, const elem_t& value) {
    ASSERT_OK

    if (pos <= 0 || pos > MAX_SIZE_/* || data_[pos] == poison_val_*/) {
        dump(DUMP_INFO, "undefined element with index: pos");
        return -1;
    }

    if (free_ == 0) {
        dump(DUMP_INFO, "List is full");
        return -1;
    }

    if (pos == head_) {
        return push_front(value);
    }

//    if (data_[free_] != poison_val_) {
//        dump(DUMP_INFO, "failed free_ pointer");
//        return -1;
//    }

    return insert_after(get_prev_index(pos), value);

    is_sorted_ = false;

    int new_pos = free_;
    free_ = next_[free_];

    next_[prev_[pos]] = new_pos;
    next_[new_pos] = pos;
    prev_[new_pos] = prev_[pos];
    prev_[pos] = new_pos;

    data_[new_pos] = value;

    size_++;

    rehash();

    ASSERT_OK
    return new_pos;
}

/*!
 * Pops the element from the end of the List
 *
 * @return phisical number of erasing element
 */

template <typename elem_t>
int List<elem_t>::pop_back() {
    ASSERT_OK

    int del_pos = tail_;
    if (del_pos == 0) {
        dump(DUMP_INFO, "pop_back was called from empty List");
        return -1;
    }

    is_sorted_ = false;

    tail_ = prev_[del_pos];
    next_[prev_[del_pos]] = 0; // hide if you want to show all

//    data_[del_pos] = poison_val_;

    next_[del_pos] = free_;
    free_ = del_pos;

    size_--;

    if (size_ == 0)
        head_ = 0;

    rehash();

    ASSERT_OK
    return 0;
}

/*!
 * Pops the element from the front of the List
 *
 * @return phisical number of erasing element
 */

template <typename elem_t>
int List<elem_t>::pop_front() {
    ASSERT_OK

    int del_pos = head_;
    if (del_pos == 0) {
        dump(DUMP_INFO, "pop_front was called from empty List");
        return -1;
    }

    is_sorted_ = false;

    head_ = next_[del_pos];
    prev_[next_[del_pos]] = 0;

//    data_[del_pos] = poison_val_;

    next_[del_pos] = free_;
    free_ = del_pos;

    size_--;

    if (size_ == 0)
        tail_ = 0;

    rehash();

    ASSERT_OK
    return 0;
}

/*!
 * Pops the element from the set position of the List
 *
 * param[in] del_pos phisical number of erasing element
 * @return code of error
 */

template <typename elem_t>
int List<elem_t>::erase(int del_pos) {
    ASSERT_OK

    if (del_pos <= 0 ||del_pos > MAX_SIZE_/* || data_[del_pos] == poison_val_*/) {
        dump(DUMP_INFO, "undefined element with index: del_pos");
        return -1;
    }

    if (del_pos == head_)
        return pop_front();

    if (del_pos == tail_)
        return pop_back();

    is_sorted_ = false;

    next_[prev_[del_pos]] = next_[del_pos];
    prev_[next_[del_pos]] = prev_[del_pos];

    next_[del_pos] = free_;
    free_ = del_pos;

//    data_[del_pos] = poison_val_;

    size_--;

    rehash();

    ASSERT_OK
    return 0;
}

/*!
 * Finds physical index of element in the List by the value
 *
 * @param[in] value item value
 * @return code of error
 */

template <typename elem_t>
int List<elem_t>::find_index_by_value(const elem_t& value) {
    ASSERT_OK

//    if (value == poison_val_) {
//        dump(DUMP_INFO, "value is equal to poison_val_");
//        return -1;
//    }

    for (int i = head_; i != 0; i = next_[i])
        if (data_[i] == value)
            return i;

    ASSERT_OK
    return 0;
}

/*!
 * Finds  element in the List by the value
 *
 * @param[in] value item value
 * @return code of error
 */

template <typename elem_t>
elem_t& List<elem_t>::find_by_value(const elem_t& value) {
    ASSERT_OK

//    if (value == poison_val_) {
//        dump(DUMP_INFO, "value is equal to poison_val_");
//        return end_;
//    }

    for (int i = head_; i != 0; i = next_[i])
        if (data_[i] == value)
            return data_[i];

    ASSERT_OK
    return end_;
}

/*!
 * Gets physical index of the 1st element
 *
 * @return physical index
 */

template <typename elem_t>
int List<elem_t>::front_index() {
    ASSERT_OK
    return head_;
}

/*!
 * Gets physical index of the last element
 *
 * @return physical index
 */

template <typename elem_t>
int List<elem_t>::back_index() {
    ASSERT_OK
    return tail_;
}

/*!
 * Gets value of the 1st element
 *
 * @return item value
 */

template <typename elem_t>
elem_t List<elem_t>::front() {
    ASSERT_OK
    return data_[head_];
}

/*!
 * Gets value of the last element
 *
 * @return item value
 */

template <typename elem_t>
elem_t List<elem_t>::back() {
    ASSERT_OK
    return data_[tail_];
}

/*!
 * Gets size of the List
 *
 * @return size
 */

template <typename elem_t>
int List<elem_t>::size() {
    ASSERT_OK
    return size_;
}

/*!
 * Gets physical index of the next element
 *
 * @param[in] pos physical index of during element
 * @return physical index
 */

template <typename elem_t>
int List<elem_t>::get_next_index(int pos) {
    ASSERT_OK

//    if (pos <= 0 || pos > MAX_SIZE_ || data_[pos] == poison_val_) {
//        dump(DUMP_INFO, "undefined element with index: pos");
//        return -1;
//    }

    return next_[pos];
}

/*!
 * Gets physical index of the previous element
 *
 * @param[in] pos physical index of during element
 * @return physical index
 */

template <typename elem_t>
int List<elem_t>::get_prev_index(int pos) {
    ASSERT_OK

    if (pos <= 0 || pos > MAX_SIZE_/* || data_[pos] == poison_val_*/) {
        dump(DUMP_INFO, "undefined element with index: pos");
        return -1;
    }

    return prev_[pos];
}

// List* getList() { return NULL; }
// getList()->get_next_index(7)

/*!
 * Returns value of element by it's logical index
 *
 * @param[in] num logical index
 * @return value of element
 */

template <typename elem_t>
elem_t List<elem_t>::operator[](int num) {
    ASSERT_OK

    if (num <= 0 || num > size_) {
        dump(DUMP_INFO, "false logical number for List");
        assert(ERROR);
        return data_[0];
    }

    if (is_sorted_)
        return data_[num];

    int index = head_;

    for (int i = 1; i < num; i++)
        index = next_[index];

    ASSERT_OK
    return data_[index];
}

/*!
 * Sorts the List, as you want to get value by logical index in O(1)
 */

template <typename elem_t>
void List<elem_t>::sort() {
    ASSERT_OK

    if (is_sorted_)
        return;

    if (size_ <= 0)
        return;

    elem_t* data_sorting = (elem_t*)calloc(MAX_SIZE_ + 1, sizeof(elem_t));

    if (data_sorting == nullptr) {
        dump(DUMP_INFO, "memory for List sort was not allocated");
        assert(ERROR);
        return;
    }

    data_sorting[0] = 0;

    for (int i = 1; head_ != 0; i++, head_ = next_[head_])
        data_sorting[i] = data_[head_];

//    for (int i = size_ + 1; i <= MAX_SIZE_; i++)
//        data_sorting[i] = poison_val_;

    for (int i = 1; i < size_; i++)
        next_[i] = i + 1;
    next_[size_] = 0;

    // for free_
    for (int i = size_ + 1; i < MAX_SIZE_; i++)
        next_[i] = i + 1;

    next_[MAX_SIZE_] = 0;

    head_ = 1;
    tail_ = size_;
    free_ = size_ + 1;

    if (size_ == MAX_SIZE_)
        free_ = 0;

    for (int i = size_; i > 1; i--)
        prev_[i] = i - 1;
    prev_[1] = 0;

    free(data_);
    data_ = data_sorting;

    rehash();

    ASSERT_OK
}

/*!
 * function for verification of the List
 *
 * uses 'list_errors' from 'my_list_enums' file
 *
 * @return number of ERROR
 */

template <typename elem_t>
int List<elem_t>::is_ok() {

    if (0 > size_ || size_ > MAX_SIZE_)
        return SIZE;

    if (0 > head_ || head_ > MAX_SIZE_)
        return HEAD_POINT;

    if (0 > tail_ || tail_ > MAX_SIZE_)
        return TAIL_POINT;

    if (0 > free_ || free_ > MAX_SIZE_)
        return FREE_POINT;

    if (kanareyka1_ != Kanareyka_const || kanareyka2_ != Kanareyka_const)
        return CANARIES;

    if (list_hash_ != MurMurHash2((char*)this, sizeof(List) - 4 * sizeof(size_t)))
        return LIST_HASH;

    if (data_hash_ != MurMurHash2((char*)data_, (MAX_SIZE_ + 1) * sizeof(elem_t)))
        return DATA_HASH;

    if (next_hash_ != MurMurHash2((char*)next_, (MAX_SIZE_ + 1) * sizeof(int)))
        return NEXT_HASH;

    if (prev_hash_ != MurMurHash2((char*)prev_, (MAX_SIZE_ + 1) * sizeof(int)))
        return PREV_HASH;

//    for (int i = head_; i != 0; i = next_[i]) {
//        if (data_[i] == poison_val_)
//            return DATA_ELEMS;
//    }
//
//    for (int i = free_; i != 0; i = next_[i]) {
//        if (data_[i] != poison_val_)
//            return FREE_ELEMS;
//    }
//
//    for (int i = tail_; i != 0; i = prev_[i]) {
//        if (data_[i] == poison_val_)
//            return PREV_ELEMS;
//    }

    return OK;
}

/*!
 * Function for calculating Hashes
 *
 * Writes the results in fields of List stucture
 */

template <typename elem_t>
void List<elem_t>::rehash() {
    list_hash_ = MurMurHash2((char*)this, sizeof(List) - 4 * sizeof(size_t));
    data_hash_ = MurMurHash2((char*)data_, (MAX_SIZE_ + 1) * sizeof(elem_t));
    next_hash_ = MurMurHash2((char*)next_, (MAX_SIZE_ + 1) * sizeof(int));
    prev_hash_ = MurMurHash2((char*)prev_, (MAX_SIZE_ + 1) * sizeof(int));
}

#endif //LIST_MY_LIST_H
