//
//  my_list_enums.h
//
//  Created by Александр on 28.04.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

#ifndef my_list_enums_h
#define my_list_enums_h

enum list_errors {
    OK         = 0,
    SIZE       = 1,
    CANARIES   = 2,
    LIST_HASH  = 3,
    DATA_HASH  = 4,
    NEXT_HASH  = 5,
    PREV_HASH  = 6,
    DATA_ELEMS = 7,
    FREE_ELEMS = 8,
    PREV_ELEMS = 9,
    HEAD_POINT = 10,
    TAIL_POINT = 11,
    FREE_POINT = 12,

};

enum fatal_errors {


};

#endif /* my_list_enums_h */
