#include "msgpack.h"
#include <QDebug>
#include "private/msgpack_p.h"

QVariant MsgPack::deserialize(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    quint8 *end = p + data.size() - 1;
    qDebug() << "deserialize size:" << data.size();

    MsgPackPrivate::unpack(p, end);

    return MsgPackPrivate::unpack(p, end);
}
