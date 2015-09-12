#ifdef QT_LOCATION_LIB

#include "location.h"
#include "../msgpack.h"

MsgPackStream& operator>>(MsgPackStream& s, QGeoCoordinate &coordinate)
{
    quint32 len;
    s.readExtHeader(len);
    if (len != 18) {
        s.setStatus(MsgPackStream::ReadCorruptData);
        return s;
    }
    double x;
    s >> x;
    coordinate.setLatitude(x);
    s >> x;
    coordinate.setLongitude(x);
    return s;
}

MsgPackStream &operator<<(MsgPackStream& s, const QGeoCoordinate &coordinate)
{
    qint8 msgpackType = MsgPack::msgpackType((QMetaType::Type)qMetaTypeId<QGeoCoordinate>());
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    s.writeExtHeader(18, msgpackType);
    s << coordinate.latitude() << coordinate.longitude();
    return s;
}

#endif
