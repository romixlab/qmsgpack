#ifndef QT_TYPES_P_H
#define QT_TYPES_P_H

#include <QVariant>
#include <QByteArray>

namespace MsgPackPrivate
{
bool register_qtype(QMetaType::Type q_type, quint8 msgpack_type);

#ifdef QT_GUI_LIB
quint32 pack_qcolor(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qcolor(const QByteArray &data);
#endif //QT_GUI_LIB

} // MsgPackPrivate

#endif // QT_TYPES_P_H