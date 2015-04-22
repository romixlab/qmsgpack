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
/**
 * @brief pack_qtime_raw internal: packs QTime to 4 or 2 bytes (with or without ms)
 * @param time QTime to pack
 * @param p pointer to preallocated array
 * format: (bits) hhhhmmmm mmssssss [------ms msmsmsms]
 */
void pack_qtime_raw(const QTime &time, quint8 *p); // return 2 - without ms, 4 with ms
/**
 * @brief unpack_qtime_raw internal: unpack 2 or 4 bytes to QTime
 * @param p data to unpack
 * @param with_ms true if array is 4 bytes (i.e. unpack with ms)
 * @return QTime
 */
QTime unpack_qtime_raw(quint8 *p, bool with_ms);
quint32 pack_qtime(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qtime(const QByteArray &data);

/**
 * @brief pack_qdate_raw internal: pack QDate to 3 bytes
 * @param date QDate to pack
 * @param p pointer to preallocated array
 * format: (bits) d(5th bit)xyyyyyyyyyyyyyy, mmmmdddd
 */
void pack_qdate_raw(const QDate &date, quint8 *p);
/// @brief internal: unpack bytes to QDate
QDate unpack_qdate_raw(quint8 *p);
quint32 pack_qdate(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qdate(const QByteArray &data);

quint32 pack_qdatetime(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qdatetime(const QByteArray &data);

// Points and Vectors
quint8 pack_two_integers(qint32 a, qint32 b, quint8 *to, bool write);
quint32 pack_qpoint(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qpoint(const QByteArray &data);
quint32 pack_qsize(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qsize(const QByteArray &data);
quint32 pack_qrect(const QVariant &variant, QByteArray &data, bool write);
QVariant unpack_qrect(const QByteArray &data);

} // MsgPackPrivate

#endif // QT_TYPES_P_H
