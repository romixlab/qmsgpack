#ifndef MSGPACK_H
#define MSGPACK_H
#include <QByteArray>
#include <QVariantList>

namespace MsgPack
{

    QVariant deserialize(const QByteArray &data);
}

#endif // MSGPACK_H
