#ifndef COMMON_H
#define COMMON_H

#include <QVariant>

namespace MsgPack {
/**
 * pack some variant to byte array data
 * when write == false only calculates and returns size
 * when write == true writes bytes to data, and returns the same size
 * return type size
 */
typedef quint32 (*pack_user_f)(const QVariant &variant, QByteArray &data, bool write);

typedef QVariant (*unpack_user_f)(QByteArray &data);
}

#endif // COMMON_H
