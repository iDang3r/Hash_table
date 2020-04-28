//
//  dump.h
//  Hash_table
//
//  Created by Александр on 28.04.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef dump_h
#define dump_h

#define DUMP_INFO __FILE__, __PRETTY_FUNCTION__, __LINE__

/*!
 * Poor dump with message
 *
 * @param[in] "DUMP_INFO"
 * @param[in] flag message
 */

void dump(const char* file, const char* func, int line, const char* flag = "ok") {
    printf("\n\033[1;31mATTENTION\033[0m\n");
    printf("File: %s\n", file);
    printf("Function: %s\n", func);
    printf("Line: %d\n", line);
    printf("\033[1;31mFlag is:->\033[0m %s\n\n", flag);
};

#endif /* dump_h */
