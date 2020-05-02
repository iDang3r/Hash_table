//
//  file_lib.h
//  Hash_table
//
//  Created by Александр on 28.04.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef file_lib_h
#define file_lib_h

#include "dump.h"

/*!
 * gets the information from file, puts it into buffer
 *
 * @param[in] name_of_input_file input file name
 * @param[out] r_buffer pinter on bubber
 * @warning use free(r_buffer) !
 * @return size of buffer
 */

size_t  get_buffer(const char* name_of_input_file, char* *r_buffer);

/*!
 * Puts data array to output file
 *
 * @param[in] name_of_output_file output file name
 * @param[in] data executable code
 * @param[in] data_size size of data
 */

void put_data_in_file(const char* name_of_output_file, char* data, size_t data_size);



size_t get_buffer(const char* name_of_input_file, char* *r_buffer) {
    if (name_of_input_file == nullptr) {
        dump(DUMP_INFO, "name of file is NULL");
        *r_buffer = nullptr;
        return 0;
    }

    FILE* input_file = fopen(name_of_input_file, "r");
    if (input_file == nullptr) {
        dump(DUMP_INFO, "incorrect name of input file");
        *r_buffer = nullptr;
        return 0;
    }

    fseek(input_file, 0, SEEK_END);
    size_t input_file_size = ftell(input_file);
    rewind(input_file);

    char* buffer = (char*)calloc(input_file_size + 2, sizeof(char));
    if (buffer == nullptr) {
        dump(DUMP_INFO, "calloc did not allocate memory");
        return 0;
    }

    *r_buffer = buffer;

    size_t buffer_size = fread(buffer, sizeof(char), input_file_size, input_file);
    if (buffer_size != input_file_size) {
        cout << input_file_size << " " << buffer_size << endl;
        dump(DUMP_INFO, "size of file and size of buffer are not equal");
        *r_buffer = nullptr;
        return 0;
    }

    fclose(input_file);
    return buffer_size;
}

size_t get_buffer(FILE* in, char* *r_buffer) {

    assert(in);

    fseek(in, 0, SEEK_END);
    size_t input_file_size = ftell(in);
    rewind(in);

    char* buffer = (char*)calloc(input_file_size + 2, sizeof(char));
    if (buffer == nullptr) {
        dump(DUMP_INFO, "calloc did not allocate memory");
        return 0;
    }

    *r_buffer = buffer;

    size_t buffer_size = fread(buffer, sizeof(char), input_file_size, in);
    if (buffer_size != input_file_size) {
        cout << input_file_size << " " << buffer_size << endl;
        dump(DUMP_INFO, "size of file and size of buffer are not equal");
        *r_buffer = nullptr;
        return 0;
    }

    return buffer_size;
}

void put_data_in_file(const char* name_of_output_file, char* data, size_t data_size) {
    if (name_of_output_file == nullptr) {
        dump(DUMP_INFO, "name of output file is null");
        return;
    }
    if (data == nullptr) {
        dump(DUMP_INFO, "data pointer is null");
        return;
    }

    FILE* output_file = fopen(name_of_output_file, "wb");
    size_t code = fwrite(data, sizeof(char), data_size, output_file);
    if (code != data_size) {
        dump(DUMP_INFO, "Error writing output file");
        return;
    }

    fclose(output_file);

    free(data);
}

#endif /* file_lib_h */
