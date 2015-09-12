#ifndef QT_TYPES_P_H
#define QT_TYPES_P_H

#include <QVariant>
#include <QByteArray>

namespace MsgPackPrivate
{
bool register_qtype(QMetaType::Type q_type, quint8 msgpack_type);

#ifdef QT_GUI_LIB
QByteArray pack_qcolor(const QVariant &variant);
QVariant unpack_qcolor(const QByteArray &data);
#endif //QT_GUI_LIB

#ifdef QT_LOCATION_LIB
QByteArray pack_qgeocoordinate(const QVariant &variant);
QVariant unpack_qgeocoordinate(const QByteArray &data);
#endif

// Date and Time
/**
 * @brief pack_qtime_raw internal: packs QTime to 4 or 2 bytes (with or without ms)
 * @param time QTime to pack
 * @param p pointer to preallocated array
 * format: (bits) hhhhmmmm mmssssss [------ms msmsmsms]
 */
/**
 * @brief unpack_qtime_raw internal: unpack 2 or 4 bytes to QTime
 * @param p data to unpack
 * @param with_ms true if array is 4 bytes (i.e. unpack with ms)
 * @return QTime
 */
QByteArray pack_qtime(const QVariant &variant);
QVariant unpack_qtime(const QByteArray &data);

/**
 * @brief pack_qdate_raw internal: pack QDate to 3 bytes
 * @param date QDate to pack
 * @param p pointer to preallocated array
 * format: (bits) d(5th bit)xyyyyyyyyyyyyyy, mmmmdddd
 */

QByteArray pack_qdate(const QVariant &variant);
QVariant unpack_qdate(const QByteArray &data);

QByteArray pack_qdatetime(const QVariant &variant);
QVariant unpack_qdatetime(const QByteArray &data);

// Points and Vectors
QByteArray pack_qpoint(const QVariant &variant);
QVariant unpack_qpoint(const QByteArray &data);
QByteArray pack_qsize(const QVariant &variant);
QVariant unpack_qsize(const QByteArray &data);
QByteArray pack_qrect(const QVariant &variant);
QVariant unpack_qrect(const QByteArray &data);

} // MsgPackPrivate

#endif // QT_TYPES_P_H
