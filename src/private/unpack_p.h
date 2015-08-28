#ifndef MSGPACK_P_H
#define MSGPACK_P_H

#include "../msgpackcommon.h"

#include <QHash>
#include <QVariant>
#include <QReadWriteLock>

namespace MsgPackPrivate
{
/* unpack functions:
 * quint8 * _type_(QVariant &v, quint8 *p);
 * parses some type, which data is stored at p
 * type data goes to v
 * return pointer to last byte + 1
 */
typedef quint8 * (* type_parser_f)(QVariant &v, quint8 *p);
extern type_parser_f unpackers[32];

bool register_unpacker(qint8 msgpack_type, MsgPack::unpack_user_f unpacker);
extern QHash<qint8, MsgPack::unpack_user_f> user_unpackers;
extern QReadWriteLock unpackers_lock;

// goes from p to end unpacking types with unpack_type function below
QVariant unpack(quint8 *p, quint8 *end);
// unpack some type, can be called recursively from other unpack functions
quint8 * unpack_type(QVariant &v, quint8 *p);

quint8 * unpack_nil(QVariant &v, quint8 *p);
quint8 * unpack_never_used(QVariant &v, quint8 *p);
quint8 * unpack_false(QVariant &v, quint8 *p);
quint8 * unpack_true(QVariant &v, quint8 *p);

quint8 * unpack_positive_fixint(QVariant &v, quint8 *p);
quint8 * unpack_negative_fixint(QVariant &v, quint8 *p);
quint8 * unpack_uint8(QVariant &v, quint8 *p);
quint8 * unpack_uint16(QVariant &v, quint8 *p);
quint8 * unpack_uint32(QVariant &v, quint8 *p);
quint8 * unpack_uint64(QVariant &v, quint8 *p);
quint8 * unpack_int8(QVariant &v, quint8 *p);
quint8 * unpack_int16(QVariant &v, quint8 *p);
quint8 * unpack_int32(QVariant &v, quint8 *p);
quint8 * unpack_int64(QVariant &v, quint8 *p);

quint8 * unpack_float32(QVariant &v, quint8 *p);
quint8 * unpack_float64(QVariant &v, quint8 *p);

quint8 * unpack_fixstr(QVariant &v, quint8 *p);
quint8 * unpack_str8(QVariant &v, quint8 *p);
quint8 * unpack_str16(QVariant &v, quint8 *p);
quint8 * unpack_str32(QVariant &v, quint8 *p);

quint8 * unpack_bin8(QVariant &v, quint8 *p);
quint8 * unpack_bin16(QVariant &v, quint8 *p);
quint8 * unpack_bin32(QVariant &v, quint8 *p);

quint8 * unpack_array_len(QVariant &v, quint8 *p, quint32 len);
quint8 * unpack_fixarray(QVariant &v, quint8 *p);
quint8 * unpack_array16(QVariant &v, quint8 *p);
quint8 * unpack_array32(QVariant &v, quint8 *p);

quint8 * unpack_map_len(QVariant &v, quint8 *p, quint32 len);
quint8 * unpack_fixmap(QVariant &v, quint8 *p);
quint8 * unpack_map16(QVariant &v, quint8 *p);
quint8 * unpack_map32(QVariant &v, quint8 *p);

quint8 * unpack_ext(QVariant &v, quint8 *p, qint8 type, quint32 len);
quint8 * unpack_fixext1(QVariant &v, quint8 *p);
quint8 * unpack_fixext2(QVariant &v, quint8 *p);
quint8 * unpack_fixext4(QVariant &v, quint8 *p);
quint8 * unpack_fixext8(QVariant &v, quint8 *p);
quint8 * unpack_fixext16(QVariant &v, quint8 *p);

quint8 * unpack_ext8(QVariant &v, quint8 *p);
quint8 * unpack_ext16(QVariant &v, quint8 *p);
quint8 * unpack_ext32(QVariant &v, quint8 *p);
}

#endif // MSGPACK_P_H
