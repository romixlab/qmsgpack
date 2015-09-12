#include "geometry.h"
#include "../msgpack.h"
#include "../msgpackstream.h"

MsgPackStream &operator<<(MsgPackStream &s, const QPoint &point)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QPoint);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    if (point.isNull()) {
        quint8 p[1] = {0};
        out.writeBytes((const char *)p, 1);
    } else {
        out << point.x() << point.y();
    }
    s.writeExtHeader(ba.length(), msgpackType);
    s.writeBytes(ba.data(), ba.length());
    return s;
}

MsgPackStream &operator>>(MsgPackStream &s, QPoint &point)
{
    quint32 len;
    s.readExtHeader(len);
    if (len == 1) {
        point = QPoint();
        return s;
    }
    QByteArray ba;
    ba.resize(len);
    s.readBytes(ba.data(), len);
    MsgPackStream in(ba);
    int x, y;
    in >> x >> y;
    point = QPoint(x, y);
    return s;
}

MsgPackStream &operator<<(MsgPackStream &s, const QSize &sz)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QSize);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    if (!sz.isValid()) {
        quint8 p[1] = {0};
        out.writeBytes((const char *)p, 1);
    } else {
        out << sz.width() << sz.height();
    }
    s.writeExtHeader(ba.length(), msgpackType);
    s.writeBytes(ba.data(), ba.length());
    return s;
}

MsgPackStream &operator>>(MsgPackStream &s, QSize &sz)
{
    quint32 len;
    s.readExtHeader(len);
    if (len == 1) {
        sz = QSize();
        return s;
    }
    QByteArray ba;
    ba.resize(len);
    s.readBytes(ba.data(), len);
    MsgPackStream in(ba);
    int w, h;
    in >> w >> h;
    sz = QSize(w, h);
    return s;
}

MsgPackStream &operator<<(MsgPackStream &s, const QRect &rect)
{
    qint8 msgpackType = MsgPack::msgpackType(QMetaType::QRect);
    if (msgpackType == -1) {
        s.setStatus(MsgPackStream::WriteFailed);
        return s;
    }
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    if (!rect.isValid()) {
        quint8 p[1] = {0};
        out.writeBytes((const char *)p, 1);
    } else {
        out << rect.left() << rect.top() << rect.width() << rect.height();
    }
    s.writeExtHeader(ba.length(), msgpackType);
    s.writeBytes(ba.data(), ba.length());
    return s;
}

MsgPackStream &operator>>(MsgPackStream &s, QRect &rect)
{
    quint32 len;
    s.readExtHeader(len);
    if (len == 1) {
        rect = QRect();
        return s;
    }
    QByteArray ba;
    ba.resize(len);
    s.readBytes(ba.data(), len);
    MsgPackStream in(ba);
    int l, t, w, h;
    in >> l >> t >> w >> h;
    rect = QRect(l, t, w, h);
    return s;
}
