#include <QString>
#include <QByteArray>
#include <QtTest>
#include "msgpack.h"
#include <limits>

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
    void test_float();
    void test_str();
    void test_bin();
    void test_array();
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
    QByteArray arr = MsgPack::pack((quint64)std::numeric_limits<quint32>::max() + 1);
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

    arr = MsgPack::pack(std::numeric_limits<quint64>::max());
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xcf);
    for (int i = 1; i < 9; ++i)
        QVERIFY(p[i] == 0xff);

    arr = MsgPack::pack((qint64)std::numeric_limits<qint32>::min() - 1);
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

    arr = MsgPack::pack(std::numeric_limits<qint64>::min());
    p = (quint8 *)arr.data();
    QVERIFY(arr.size() == 9);
    QVERIFY(p[0] == 0xd3);
    QVERIFY(p[1] == 0x80);
    for (int i = 2; i < 9; ++i)
        QVERIFY(p[i] == 0x00);
}

void PackTest::test_float()
{
    QByteArray arr = MsgPack::pack(1.234567);
    QVERIFY(arr.size() == 9);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xcb);
    QVERIFY(p[1] == 0x3f);
    QVERIFY(p[2] == 0xf3);
    QVERIFY(p[3] == 0xc0);
    QVERIFY(p[4] == 0xc9);
    QVERIFY(p[5] == 0x53);
    QVERIFY(p[6] == 0x9b);
    QVERIFY(p[7] == 0x88);
    QVERIFY(p[8] == 0x87);
}

void PackTest::test_str()
{
    QString str = "msgpack rocks";
    QByteArray arr = MsgPack::pack(str);
    QVERIFY(arr.size() == 14);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == (0xa0 | str.length()));
    QVERIFY(memcmp(p + 1, str.toUtf8().data(), str.size()) == 0);

    str = QString(QByteArray(32, 'm'));
    arr = MsgPack::pack(str);
    QVERIFY(arr.size() == 32 + 2);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd9);
    QVERIFY(p[1] == 32);
    QVERIFY(memcmp(p + 2, str.toUtf8().data(), str.size()) == 0);

    str = QString(QByteArray(256, 's'));
    arr = MsgPack::pack(str);
    QVERIFY(arr.size() == 256 + 3);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xda);
    QVERIFY(p[1] == 0x01);
    QVERIFY(p[2] == 0x00);
    QVERIFY(memcmp(p + 3, str.toUtf8().data(), str.size()) == 0);

    str = QString(QByteArray(65536, 'g'));
    arr = MsgPack::pack(str);
    QVERIFY(arr.size() == 65536 + 5);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xdb);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
    QVERIFY(memcmp(p + 5, str.toUtf8().data(), str.size()) == 0);
}

void PackTest::test_bin()
{
    QByteArray ba = QByteArray("msgpack");
    QByteArray arr = MsgPack::pack(ba);
    QVERIFY(arr.size() == ba.size() + 2);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc4);
    QVERIFY(p[1] == ba.size());
    QVERIFY(memcmp(p + 2, ba.data(), ba.size()) == 0);

    ba = QByteArray(256, 'r');
    arr = MsgPack::pack(ba);
    QVERIFY(arr.size() == ba.size() + 3);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc5);
    QVERIFY(p[1] == 0x01);
    QVERIFY(p[2] == 0x00);
    QVERIFY(memcmp(p + 3, ba.data(), ba.size()) == 0);

    ba = QByteArray(65536, 'x');
    arr = MsgPack::pack(ba);
    QVERIFY(arr.size() == ba.size() + 5);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc6);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
    QVERIFY(memcmp(p + 5, ba.data(), ba.size()) == 0);
}

void PackTest::test_array()
{
    QVariantList list;
    list << 1 << 2 << 3;
    QByteArray arr = MsgPack::pack(list);
    QVERIFY(arr.size() == list.size() + 1);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == (0x90 | list.size()));

    for (int i = 0; i < 29; ++i)
        list << 4;
    arr = MsgPack::pack(list);
    QVERIFY(arr.size() == list.size() + 3);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xdc);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x20);

    for (int i = 0; i < 65504; ++i)
        list << 7;
    arr = MsgPack::pack(list);
    QVERIFY(arr.size() == list.size() + 5);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xdd);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
}

QTEST_APPLESS_MAIN(PackTest)

#include "pack_test.moc"
