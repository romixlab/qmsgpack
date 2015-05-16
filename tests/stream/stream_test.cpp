#include <QString>
#include <QtTest>
#include <QDebug>
#include <stream.h>
#include <msgpack.h>
#include <limits>

class StreamTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_unpack_integers();
    void test_pack_integers();

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


QTEST_APPLESS_MAIN(StreamTest)

#include "stream_test.moc"
