// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "msgpack.h"
#include "private/unpack_p.h"
#include "private/pack_p.h"
#include "private/qt_types_p.h"

#include <QVector>

#ifdef QT_LOCATION_LIB
#include <QGeoCoordinate>
#endif

QVariant MsgPack::unpack(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    quint8 *end = p + data.size() - 1;

    return MsgPackPrivate::unpack(p, end);
}

QByteArray MsgPack::pack(const QVariant &variant)
{
    QVector<QByteArray> user_data;
    // first run, calculate size
    ptrdiff_t size = MsgPackPrivate::pack(variant, nullptr, false, user_data) -
        static_cast<quint8 *>(nullptr);
    QByteArray arr;
    arr.resize(size);

    // second run, pack it
    MsgPackPrivate::pack(variant, (quint8 *)arr.data(), true, user_data);

    return arr;
}

bool MsgPack::registerPacker(int qType, qint8 msgpackType, MsgPack::pack_user_f packer)
{
    return MsgPackPrivate::register_packer(qType, msgpackType, packer);
}

bool MsgPack::registerUnpacker(qint8 msgpackType, MsgPack::unpack_user_f unpacker)
{
    return MsgPackPrivate::register_unpacker(msgpackType, unpacker);
}

qint8 MsgPack::msgpackType(int qType)
{
    return MsgPackPrivate::msgpack_type((QMetaType::Type)qType);
}

bool MsgPack::registerType(QMetaType::Type qType, quint8 msgpackType)
{
    switch (qType) {
#ifdef QT_GUI_LIB
    case QMetaType::QColor:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qcolor);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qcolor);
        return true;
#endif // QT_GUI_LIB
    case QMetaType::QTime:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qtime);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qtime);
        return true;
    case QMetaType::QDate:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qdate);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qdate);
        return true;
    case QMetaType::QDateTime:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qdatetime);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qdatetime);
        return true;
    case QMetaType::QPoint:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qpoint);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qpoint);
        return true;
    case QMetaType::QSize:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qsize);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qsize);
        return true;
    case QMetaType::QRect:
        MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qrect);
        MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qrect);
        return true;
    default:
#ifdef QT_LOCATION_LIB
        if (int(qType) == qMetaTypeId<QGeoCoordinate>()) {
            MsgPackPrivate::register_packer(qType, msgpackType, MsgPackPrivate::pack_qgeocoordinate);
            MsgPackPrivate::register_unpacker(msgpackType, MsgPackPrivate::unpack_qgeocoordinate);
            return true;
        }
#endif // QT_LOCATION_LIB
        break;
    }

    qWarning("qmsgpack was built without metatype %d support.", int(qType));
    qWarning("Use MsgPack::registerPacker() and MsgPack::registerUnpacker() to register metatype %d manually.", int(qType));
    return false;
}

void MsgPack::setCompatibilityModeEnabled(bool enabled)
{
    MsgPackPrivate::compatibilityMode = enabled;
}
