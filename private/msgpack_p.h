#ifndef MSGPACK_P_H
#define MSGPACK_P_H
#include <QVariant>



namespace MsgPackPrivate
{
/* unpack functions:
 * QVariant _type_(quint8 *p, quint32 &sz);
 * parses some type, which data is stored at p
 * fill sz with type size (sz is there at the moment of calling
 * parser function)
 */
typedef QVariant (* type_parser_f)(quint8 *p, quint32 &sz);
extern type_parser_f unpackers[32];

// goes from p to end unpacking types with unpack_type function below
QVariant unpack(quint8 *p, quint8 *end);
// unpack some type, can be called recursively from other unpack functions
QVariant unpack_type(quint8 *p, quint32 &sz);

QVariant unpack_map_len(quint8 *p, quint32 &sz, quint32 len);
QVariant unpack_array_len(quint8 *p, quint32 &sz, quint32 len);
quint32 unpack_uint8(quint8 *p);
quint32 unpack_uint16(quint8 *p);
quint32 unpack_uint32(quint8 *p);


QVariant unpack_positive_fixint(quint8 *p, quint32 &sz);
QVariant unpack_negative_fixint(quint8 *p, quint32 &sz);
QVariant unpack_fixmap(quint8 *p, quint32 &sz);
QVariant unpack_fixarray(quint8 *p, quint32 &sz);
QVariant unpack_fixstr(quint8 *p, quint32 &sz);
QVariant unpack_nil(quint8 *p, quint32 &sz);
QVariant unpack_never_used(quint8 *p, quint32 &sz);
QVariant unpack_false(quint8 *p, quint32 &sz);
QVariant unpack_true(quint8 *p, quint32 &sz);
QVariant unpack_bin8(quint8 *p, quint32 &sz);
QVariant unpack_bin16(quint8 *p, quint32 &sz);
QVariant unpack_bin32(quint8 *p, quint32 &sz);
QVariant unpack_ext8(quint8 *p, quint32 &sz);
QVariant unpack_ext16(quint8 *p, quint32 &sz);
QVariant unpack_ext32(quint8 *p, quint32 &sz);
QVariant unpack_float32(quint8 *p, quint32 &sz);
QVariant unpack_float64(quint8 *p, quint32 &sz);
QVariant unpack_uint8(quint8 *p, quint32 &sz);
QVariant unpack_uint16(quint8 *p, quint32 &sz);
QVariant unpack_uint32(quint8 *p, quint32 &sz);
QVariant unpack_uint64(quint8 *p, quint32 &sz);
QVariant unpack_int8(quint8 *p, quint32 &sz);
QVariant unpack_int16(quint8 *p, quint32 &sz);
QVariant unpack_int32(quint8 *p, quint32 &sz);
QVariant unpack_int64(quint8 *p, quint32 &sz);
QVariant unpack_fixext1(quint8 *p, quint32 &sz);
QVariant unpack_fixext2(quint8 *p, quint32 &sz);
QVariant unpack_fixext4(quint8 *p, quint32 &sz);
QVariant unpack_fixext8(quint8 *p, quint32 &sz);
QVariant unpack_fixext16(quint8 *p, quint32 &sz);
QVariant unpack_str8(quint8 *p, quint32 &sz);
QVariant unpack_str16(quint8 *p, quint32 &sz);
QVariant unpack_str32(quint8 *p, quint32 &sz);
QVariant unpack_array16(quint8 *p, quint32 &sz);
QVariant unpack_array32(quint8 *p, quint32 &sz);
QVariant unpack_map16(quint8 *p, quint32 &sz);
QVariant unpack_map32(quint8 *p, quint32 &sz);
}

#endif // MSGPACK_P_H
