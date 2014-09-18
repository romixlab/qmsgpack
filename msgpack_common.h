#ifndef COMMON_H
#define COMMON_H

#include <QVariant>

namespace MsgPack {
typedef quint32 (*pack_user_f)(const QVariant &variant, QByteArray &data, bool write);
}

#endif // COMMON_H
