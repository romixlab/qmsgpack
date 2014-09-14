#ifndef MSGPACK_H
#define MSGPACK_H
#include <QByteArray>
#include <QVariantList>

namespace MsgPack
{
    QVariant unpack(const QByteArray &data);
    QByteArray pack(const QVariant &variant);
}

#endif // MSGPACK_H
