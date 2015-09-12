#ifndef TIME_H
#define TIME_H

#include "../msgpack_export.h"
#include "../msgpackstream.h"

#include <QDateTime>

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QTime &time);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QTime &time);

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QDate &date);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QDate &date);

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QDateTime &dt);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QDateTime &dt);

namespace MsgPackPrivate {
void pack_qtime_raw(const QTime &time, quint8 *p);
QTime unpack_qtime_raw(quint8 *p, bool with_ms);
void pack_qdate_raw(const QDate &date, quint8 *p);
QDate unpack_qdate_raw(quint8 *p);
}

#endif // TIME_H

