#include "msgpack.h"
#include <QDebug>

QVariantList MsgPack::deserialize(const QByteArray &data)
{
    QVariantList d;
    int i = 0;

    quint8 *p = (quint8 *)data.data();
    while (i < data.size()) {
        if (p[i] <= 127) { // positive fixint 0x00 - 0x7f
            quint32 val = (quint32)p[i];
            d.append(QVariant(val));
            i += 1;
        }
        if (p[i] >= 0xe0) { // negative fixint 0xe0 - 0xff
            quint8 val8 = (quint8)p[i];
            val8 &= ~((1 << 7) | (1 << 6) | (1 << 5));
            qint32 val = 31 - val8;
            val ^= 0xffffffff;
            d.append(QVariant((qint32)val));
            i += 1;
        }


    }

    return d;
}
