#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../msgpack_export.h"
#include "../msgpackstream.h"

#include <QRect>

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QPoint &point);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QPoint &point);

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QSize &sz);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QSize &sz);

MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QRect &rect);
MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QRect &rect);

#endif // GEOMETRY_H

