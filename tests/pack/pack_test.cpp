#include <QString>
#include <QByteArray>
#include <QtTest>
#include "msgpack.h"

class PackTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_bool();
    void test_fixint();
    void test_integer8();
    void test_integer16();
    void test_integer32();
    void test_integer64();
};

void PackTest::test_bool()
{
    QByteArray arr = MsgPack::pack(false);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0xc2);

    arr = MsgPack::pack(true);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0xc3);
}

void PackTest::test_fixint()
{
    QByteArray arr = MsgPack::pack(0);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0x00);

    arr = MsgPack::pack(127);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0x7f);

    arr = MsgPack::pack(-1);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0xff);

    arr = MsgPack::pack(-32);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 1);
    QVERIFY(p[0] == 0xe0);
}

void PackTest::test_integer8()
{
    QByteArray arr = MsgPack::pack(128);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 2);
    QVERIFY(p[0] == 0xcc);
    QVERIFY(p[1] == 0x80);

    arr = MsgPack::pack(255);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 2);
    QVERIFY(p[0] == 0xcc);
    QVERIFY(p[1] == 0xff);

    arr = MsgPack::pack(-33);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 2);
    QVERIFY(p[0] == 0xd0);
    QVERIFY(p[1] == 0xdf);

    arr = MsgPack::pack(-128);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 2);
    QVERIFY(p[0] == 0xd0);
    QVERIFY(p[1] == 0x80);
}

void PackTest::test_integer16()
{
    QByteArray arr = MsgPack::pack(256);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 3);
    QVERIFY(p[0] == 0xcd);
    QVERIFY(p[1] == 0x01);
    QVERIFY(p[2] == 0x00);

    arr = MsgPack::pack(65535);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 3);
    QVERIFY(p[0] == 0xcd);
    QVERIFY(p[1] == 0xff);
    QVERIFY(p[2] == 0xff);

    arr = MsgPack::pack(-129);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 3);
    QVERIFY(p[0] == 0xd1);
    QVERIFY(p[1] == 0xff);
    QVERIFY(p[2] == 0x7f);

    arr = MsgPack::pack(-32768);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 3);
    QVERIFY(p[0] == 0xd1);
    QVERIFY(p[1] == 0x80);
    QVERIFY(p[2] == 0x00);
}

void PackTest::test_integer32()
{
    QByteArray arr = MsgPack::pack(65536);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 5);
    QVERIFY(p[0] == 0xce);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);

    arr = MsgPack::pack((quint32)4294967295);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 5);
    QVERIFY(p[0] == 0xce);
    QVERIFY(p[1] == 0xff);
    QVERIFY(p[2] == 0xff);
    QVERIFY(p[3] == 0xff);
    QVERIFY(p[4] == 0xff);

    arr = MsgPack::pack(-32769);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 5);
    QVERIFY(p[0] == 0xd2);
    QVERIFY(p[1] == 0xff);
    QVERIFY(p[2] == 0xff);
    QVERIFY(p[3] == 0x7f);
    QVERIFY(p[4] == 0xff);

    arr = MsgPack::pack((qint32)-2147483648);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 5);
    QVERIFY(p[0] == 0xd2);
    QVERIFY(p[1] == 0x80);
    QVERIFY(p[2] == 0x00);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
}

void PackTest::test_integer64()
{
    QByteArray arr = MsgPack::pack((quint64)4294967296);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xcf);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x00);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x01);
    QVERIFY(p[5] == 0x00);
    QVERIFY(p[6] == 0x00);
    QVERIFY(p[7] == 0x00);
    QVERIFY(p[8] == 0x00);

    arr = MsgPack::pack((quint64)18446744073709551615U);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xcf);
    for (int i = 1; i < 9; ++i)
        QVERIFY(p[i] == 0xff);

    arr = MsgPack::pack((qint64)-2147483649);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xd3);
    QVERIFY(p[1] == 0xff);
    QVERIFY(p[2] == 0xff);
    QVERIFY(p[3] == 0xff);
    QVERIFY(p[4] == 0xff);
    QVERIFY(p[5] == 0x7f);
    QVERIFY(p[6] == 0xff);
    QVERIFY(p[7] == 0xff);
    QVERIFY(p[8] == 0xff);

    arr = MsgPack::pack((qint64)-9223372036854775808);
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xd3);
    QVERIFY(p[1] == 0x80);
    for (int i = 2; i < 9; ++i)
        QVERIFY(p[i] == 0x00);
}

QTEST_APPLESS_MAIN(PackTest)

#include "pack_test.moc"
