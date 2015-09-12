#include <QString>
#include <QtTest>
#include <QDebug>
#include <msgpackstream.h>
#include <msgpack.h>
#include <stream/time.h>
#include <stream/geometry.h>
#include <limits>

class StreamTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_unpack_integers();
    void test_pack_integers();
    void test_unpack_string();
    void test_pack_string();
    void test_float();
    void test_double();
    void test_bin();
    void test_array();
    void test_ext();
    void test_time();
    void test_geometry();
};

void StreamTest::test_unpack_integers()
{
    QByteArray ints = QByteArray::fromBase64("AH//4cyAzP/Q39CAzQEAzf//0f9/0YAAz"
                                             "gABAADO/////9L//3//0oAAAADPAAAAAQ"
                                             "AAAADP///////////T/////3/////TgAA"
                                             "AAAAAAAA=");
    MsgPackStream stream(ints);
    quint8 u8;
    quint16 u16;
    quint32 u32;
    quint64 u64;
    qint8 i8;
    qint16 i16;
    qint32 i32;
    qint64 i64;

    stream >> u8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u8 == 0);
    stream >> u8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u8 == 127);
    stream >> i8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i8 == -1);
    stream >> i8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i8 == -31);
    stream >> u8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u8 == 128);
    stream >> u8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u8 == 255);
    stream >> i8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i8 == -33);
    stream >> i8;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i8 == -128);
    stream >> u16;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u16 == 256);
    stream >> u32;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u32 == 65535);
    stream >> i16;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i16 == -129);
    stream >> i16;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i16 == -32768);
    stream >> u32;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u32 == 65536);
    stream >> u32;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u32 == 4294967295);
    stream >> i32;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i32 == -32769);
    stream >> i32;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i32 == -2147483648);
    stream >> u64;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u64 == 4294967296);
    stream >> u64;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(u64 == std::numeric_limits<quint64>::max());
    stream >> i64;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i64 == -2147483649);
    stream >> i64;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(i64 == std::numeric_limits<qint64>::min());
}

void StreamTest::test_pack_integers()
{
    QByteArray packed;
    MsgPackStream stream(&packed, QIODevice::WriteOnly);
    stream << 0 << 127 << -1 << -31 << 128 << 255 << -33 << -128 << 256;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << 65535 << -129 << -32768 << 65536;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << (quint32)4294967295 << -32769 << (qint32)-2147483648;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << (quint64)4294967296;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << std::numeric_limits<quint64>::max();
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << (qint64)-2147483649;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    stream << std::numeric_limits<qint64>::min();
    QVERIFY(stream.status() == MsgPackStream::Ok);

    QVariantList l = MsgPack::unpack(packed).toList();
    QVERIFY(l[0].toInt() == 0);
    QVERIFY(l[1].toInt() == 127);
    QVERIFY(l[2].toInt() == -1);
    QVERIFY(l[3].toInt() == -31);
    QVERIFY(l[4].toInt() == 128);
    QVERIFY(l[5].toInt() == 255);
    QVERIFY(l[6].toInt() == -33);
    QVERIFY(l[7].toInt() == -128);
    QVERIFY(l[8].toInt() == 256);
    QVERIFY(l[9].toInt() == 65535);
    QVERIFY(l[10].toInt() == -129);
    QVERIFY(l[11].toInt() == -32768);
    QVERIFY(l[12].toInt() == 65536);
    QVERIFY(l[13].toUInt() == 4294967295);
    QVERIFY(l[14].toInt() == -32769);
    QVERIFY(l[15].toInt() == -2147483648);
    QVERIFY(l[16].toLongLong() == 4294967296);
    QVERIFY(l[17].toULongLong() == std::numeric_limits<quint64>::max());
    QVERIFY(l[18].toLongLong() == -2147483649);
    QVERIFY(l[19].toLongLong() == std::numeric_limits<qint64>::min());
}

void StreamTest::test_unpack_string()
{
    QString str = QStringLiteral("msgpack rocks");
    QByteArray packed = MsgPack::pack(str);
    QString str2;

    {
        MsgPackStream stream(packed);
        stream >> str2;
        QVERIFY(str == str2);
    }
    {
        str = QString(QByteArray(32, 'm'));
        packed = MsgPack::pack(str);
        MsgPackStream stream(packed);
        stream >> str2;
        QVERIFY(str == str2);
    }
    {
        str = QString::fromUtf8(QByteArray(256, 's'));
        packed = MsgPack::pack(str);
        MsgPackStream stream(packed);
        stream >> str2;
        QVERIFY(str == str2);
    }
    {
        str = QString(QByteArray(65536, 'g'));
        packed = MsgPack::pack(str);
        MsgPackStream stream(packed);
        stream >> str2;
        QVERIFY(str == str2);
    }
}

void StreamTest::test_pack_string()
{
    QByteArray packed;
    MsgPackStream stream(&packed, QIODevice::WriteOnly);
    QStringList strs;
    stream << "abc";
    strs << "abc";
    QString s;
    for (int i = 0; i < 8; ++i)
        s += "xy";
    stream << s;
    strs << s;
    s = QString();
    for (int i = 0; i < 64; ++i)
        s += "msgp";
    stream << s;
    strs << s;
    s = QString();
    for (int i = 0; i < 4096; ++i)
        s += "messagepack test";
    stream << s;
    strs << s;
    stream << "";

    QStringList l = MsgPack::unpack(packed).toStringList();
    QVERIFY(l[0] == strs[0]);
    QVERIFY(l[1] == strs[1]);
    QVERIFY(l[2] == strs[2]);
    QVERIFY(l[3] == strs[3]);
    QVERIFY(l[4].isEmpty());
}

void StreamTest::test_float()
{
    QByteArray packed;
    {
        MsgPackStream stream(&packed, QIODevice::WriteOnly);
        stream << -0.0f << 0.0f << -1.0f << 1.0f << -32767.0f << 32767.0f;
        QVERIFY(packed.size() == 6 * 5);
        QVERIFY(stream.status() == MsgPackStream::Ok);
    }

    MsgPackStream stream(packed);
    float f;

    stream >> f;
    QVERIFY(f == -0.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> f;
    QVERIFY(f == 0.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> f;
    QVERIFY(f == -1.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> f;
    QVERIFY(f == 1.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> f;
    QVERIFY(f == -32767.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> f;
    QVERIFY(f == 32767.0f);
    QVERIFY(stream.status() == MsgPackStream::Ok);
}

void StreamTest::test_double()
{
    QByteArray packed;
    {
        MsgPackStream stream(&packed, QIODevice::WriteOnly);
        stream << -0.0 << 0.0 << -1.0 << 1.0 << -32767.0 << 32767.0;
        QVERIFY(packed.size() == 6 * 9);
        QVERIFY(stream.status() == MsgPackStream::Ok);
    }

    MsgPackStream stream(packed);
    double d;

    stream >> d;
    QVERIFY(d == -0.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> d;
    QVERIFY(d == 0.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> d;
    QVERIFY(d == -1.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> d;
    QVERIFY(d == 1.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> d;
    QVERIFY(d == -32767.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> d;
    QVERIFY(d == 32767.0);
    QVERIFY(stream.status() == MsgPackStream::Ok);
}

void StreamTest::test_bin()
{
    QByteArray ba1("msgpack"), ba2(256, 'r'), ba3(65536, 'x');
    QByteArray packed;
    {
        MsgPackStream stream(&packed, QIODevice::WriteOnly);
        stream << ba1 << ba2 << ba3;
    }
    QVERIFY(packed.size() == 7+2 + 256+3 + 65536+5);

    MsgPackStream stream(packed);
    QByteArray ba;

    stream >> ba;
    QVERIFY(ba == ba1);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> ba;
    QVERIFY(ba == ba2);
    QVERIFY(stream.status() == MsgPackStream::Ok);

    stream >> ba;
    QVERIFY(ba == ba3);
    QVERIFY(stream.status() == MsgPackStream::Ok);
}

void StreamTest::test_array()
{
{
    QList<qint64> list;
    list << 0 << 127 << -1 << -31 << 128 << 255 << -33 << -128 << 256;
    list << 65535 << -129 << -32768 << 65536;
    list << -32769 << (qint32)-2147483648;
    list << (qint64)-2147483649;
    list << std::numeric_limits<qint64>::min();
    QByteArray packed;
    {
        MsgPackStream stream(&packed, QIODevice::WriteOnly);
        stream << list;
        QVERIFY(stream.status() == MsgPackStream::Ok);
    }

    QVariantList list2 = MsgPack::unpack(packed).toList();
    QVERIFY(list.size() == list2.size());
    for (int i = 0; i < list.size(); ++i)
        QVERIFY(list[i] == list2[i]);

    packed = MsgPack::pack(list2);
    MsgPackStream stream(packed);
    QList<qint64> list3;
    stream >> list3;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(list2.size() == list3.size());
    for (int i = 0; i < list2.size(); ++i)
        QVERIFY(list2[i] == list3[i]);
}
{ 
    QList<quint64> list;
    list << 6;
    list << std::numeric_limits<quint64>::min();
    list << std::numeric_limits<quint64>::max();
    list << -4;
    QByteArray packed;
    {
        MsgPackStream stream(&packed, QIODevice::WriteOnly);
        stream << list;
        QVERIFY(stream.status() == MsgPackStream::Ok);
    }

    QVariantList list2 = MsgPack::unpack(packed).toList();
    QVERIFY(list.size() == list2.size());
    for (int i = 0; i < list.size(); ++i)
        QVERIFY(list[i] == list2[i]);

    packed = MsgPack::pack(list2);
    MsgPackStream stream(packed);
    QList<quint64> list3;
    stream >> list3;
    QVERIFY(stream.status() == MsgPackStream::Ok);
    QVERIFY(list2.size() == list3.size());
    for (int i = 0; i < list2.size(); ++i)
        QVERIFY(list2[i] == list3[i]);
}
}

void StreamTest::test_ext()
{
    {
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out.writeExtHeader(1, 7);
        QVERIFY(out.status() == MsgPackStream::Ok);
        quint8 *p = (quint8 *)ba.data();
        QVERIFY(ba.length() == 2);
        QVERIFY(p[0] == 0xd4);
        QVERIFY(p[1] == 7);

        MsgPackStream in(ba);
        quint32 len;
        QVERIFY(in.readExtHeader(len));
        QVERIFY(in.status() == MsgPackStream::Ok);
        QVERIFY(len == 1);
        QVERIFY(in.device()->pos() == 2);
    }
    {
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out.writeExtHeader(17, 7);
        QVERIFY(out.status() == MsgPackStream::Ok);
        quint8 *p = (quint8 *)ba.data();
        QVERIFY(ba.length() == 3);
        QVERIFY(p[0] == 0xc7);
        QVERIFY(p[2] == 7);

        MsgPackStream in(ba);
        quint32 len;
        QVERIFY(in.readExtHeader(len));
        QVERIFY(in.status() == MsgPackStream::Ok);
        QVERIFY(len == 17);
        QVERIFY(in.device()->pos() == 3);
    }
    {
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out.writeExtHeader(256, 7);
        QVERIFY(out.status() == MsgPackStream::Ok);
        quint8 *p = (quint8 *)ba.data();
        QVERIFY(ba.length() == 4);
        QVERIFY(p[0] == 0xc8);
        QVERIFY(p[3] == 7);

        MsgPackStream in(ba);
        quint32 len;
        QVERIFY(in.readExtHeader(len));
        QVERIFY(in.status() == MsgPackStream::Ok);
        QVERIFY(len == 256);
        QVERIFY(in.device()->pos() == 4);
    }
    {
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out.writeExtHeader(65536, 7);
        QVERIFY(out.status() == MsgPackStream::Ok);
        quint8 *p = (quint8 *)ba.data();
        QVERIFY(ba.length() == 6);
        QVERIFY(p[0] == 0xc9);
        QVERIFY(p[5] == 7);

        MsgPackStream in(ba);
        quint32 len;
        QVERIFY(in.readExtHeader(len));
        QVERIFY(in.status() == MsgPackStream::Ok);
        QVERIFY(len == 65536);
        QVERIFY(in.device()->pos() == 6);
    }
}

void StreamTest::test_time()
{
    MsgPack::registerType(QMetaType::QTime, 3);
    MsgPack::registerType(QMetaType::QDate, 4);
    MsgPack::registerType(QMetaType::QDateTime, 5);
    {
        QTime t;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << t;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QTime t2;
        in >> t2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(t == t2);
    }
    {
        QTime t = QTime(12, 01, 01, 0);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << t;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 4);

        MsgPackStream in(ba);
        QTime t2;
        in >> t2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(t == t2);
    }
    {
        QTime t = QTime(12, 59, 59, 0);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << t;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 4);

        MsgPackStream in(ba);
        QTime t2;
        in >> t2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(t == t2);
    }
    {
        QTime t = QTime(12, 34, 56, 789);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << t;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 6);

        MsgPackStream in(ba);
        QTime t2;
        in >> t2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(t == t2);
    }
    {
        QDate d;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << d;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QDate d2;
        in >> d2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(d == d2);
    }
    {
        QDate d = QDate(1234, 12, 1);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << d;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 6);

        MsgPackStream in(ba);
        QDate d2;
        in >> d2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(d == d2);
    }
    {
        QDate d = QDate(9999, 1, 31);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << d;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 6);

        MsgPackStream in(ba);
        QDate d2;
        in >> d2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(d == d2);
    }
    {
        QDateTime dt;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << dt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QDateTime dt2;
        in >> dt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(dt == dt2);
    }
    {
        QDateTime dt(QDate(1234, 12, 1), QTime(12, 34, 56));
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << dt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 8);

        MsgPackStream in(ba);
        QDateTime dt2;
        in >> dt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(dt == dt2);
    }
    {
        QDateTime dt(QDate(1234, 12, 1), QTime(12, 34, 56, 789));
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << dt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 10);

        MsgPackStream in(ba);
        QDateTime dt2;
        in >> dt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(dt == dt2);
    }
}

void StreamTest::test_geometry()
{
    MsgPack::registerType(QMetaType::QPoint, 6);
    MsgPack::registerType(QMetaType::QSize, 7);
    MsgPack::registerType(QMetaType::QRect, 8);
    {
        QPoint pt;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << pt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QPoint pt2;
        in >> pt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(pt == pt2);
    }
    {
        QPoint pt(1, 2);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << pt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 4);

        MsgPackStream in(ba);
        QPoint pt2;
        in >> pt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(pt == pt2);
    }
    {
        QPoint pt(1234, 5678);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << pt;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 9);

        MsgPackStream in(ba);
        QPoint pt2;
        in >> pt2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(pt == pt2);
    }
    {
        QSize sz;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << sz;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QSize sz2;
        in >> sz2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(sz == sz2);
    }
    {
        QSize sz(1, 2);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << sz;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 4);

        MsgPackStream in(ba);
        QSize sz2;
        in >> sz2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(sz == sz2);
    }
    {
        QSize sz(1234, 5678);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << sz;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 9);

        MsgPackStream in(ba);
        QSize sz2;
        in >> sz2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(sz == sz2);
    }
    {
        QRect rect;
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << rect;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 3);

        MsgPackStream in(ba);
        QRect rect2;
        in >> rect2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(rect == rect2);
    }
    {
        QRect rect(1, 2, 3, 4);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << rect;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 6);

        MsgPackStream in(ba);
        QRect rect2;
        in >> rect2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(rect == rect2);
    }
    {
        QRect rect(0, 0, 65536, 65536);
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly);
        out << rect;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(ba.length() == 15);

        MsgPackStream in(ba);
        QRect rect2;
        in >> rect2;
        QVERIFY(out.status() == MsgPackStream::Ok);
        QVERIFY(rect == rect2);
    }
}

QTEST_APPLESS_MAIN(StreamTest)
#include "stream_test.moc"
