#ifndef MSGPACK_P_H
#define MSGPACK_P_H
#include <QVariant>

/* parser functions:
 * QVariant _type_(quint8 *p, int &i);
 * parses some type, which data is stored at p
 *
 */

namespace MsgPackPrivate
{
/* data sizes for types from 0x7f to 0xe0
 * (without positive and negative fixint)
 ** fixed size types:
 * possible values: 1, 2, 3, 4, 5, 6, 9, 10, 18 - bytes
 * 254 - size is in next 1 byte
 * 253 - 2
 * 251 - 4
 ** ext formats:
 * 126 - size is in next byte + 1 type byte
 * 125 - 2 byte size + 1 type byte
 * 123 - 4 byte size + 1 type byte
 */

    static quint8 data_sizes[96];

    QVariant positive_fixint(quint8 *p);


}

#endif // MSGPACK_P_H
