#include "msgpack_p.h"

static quint8 MsgPackPrivate::data_sizes[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, // fixmap upto 15 elements

};
