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

// Date and Time
void pack_qtime_raw(const QTime &time, quint8 *p);
QTime unpack_qtime_raw(quint8 *p);
quint32 pack_qtime(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qtime(const QByteArray &data);

void pack_qdate_raw(const QDate &date, quint8 *p);
QDate unpack_qdate_raw(quint8 *p);
quint32 pack_qdate(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qdate(const QByteArray &data);

quint32 pack_qdatetime(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qdatetime(const QByteArray &data);


} // MsgPackPrivate

#endif // QT_TYPES_P_H
