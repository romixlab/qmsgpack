#include <QString>
#include <QtTest>
#include <QDebug>
#include <msgpack.h>
#include <limits>
#include <msgpack_ext.h>

class ExtText : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_upto_quint();
    void test_upto_qint();
    void test_upto_qint_to_quint();
    void test_fail();
};

void ExtText::test_upto_quint()
{
    QByteArray packed;
    bool success;
    quint8 u8;
    quint16 u16;
    quint32 u32;
    quint64 u64;

    packed = MsgPack::pack(0);
    MsgPack::Ext::unpack_upto_quint8(&u8, (quint8 *)packed.data(), &success);
    QVERIFY(u8 == 0);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_quint16(&u16, (quint8 *)packed.data(), &success);
    QVERIFY(u16 == 0);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_quint32(&u32, (quint8 *)packed.data(), &success);
    QVERIFY(u32 == 0);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_quint64(&u64, (quint8 *)packed.data(), &success);
    QVERIFY(u64 == 0);
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint8>::max());
    MsgPack::Ext::unpack_upto_quint8(&u8, (quint8 *)packed.data(), &success);
    QVERIFY(u8 == std::numeric_limits<quint8>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint16>::max());
    MsgPack::Ext::unpack_upto_quint16(&u16, (quint8 *)packed.data(), &success);
    QVERIFY(u16 == std::numeric_limits<quint16>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint32>::max());
    MsgPack::Ext::unpack_upto_quint32(&u32, (quint8 *)packed.data(), &success);
    QVERIFY(u32 == std::numeric_limits<quint32>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint64>::max());
    MsgPack::Ext::unpack_upto_quint64(&u64, (quint8 *)packed.data(), &success);
    QVERIFY(u64 == std::numeric_limits<quint64>::max());
    QVERIFY(success);
}

void ExtText::test_upto_qint()
{
    QByteArray packed;
    bool success;
    qint8 i8;
    qint16 i16;
    qint32 i32;
    qint64 i64;

    packed = MsgPack::pack(-32);
    MsgPack::Ext::unpack_upto_qint8(&i8, (quint8 *)packed.data(), &success);
    QVERIFY(i8 == -32);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_qint16(&i16, (quint8 *)packed.data(), &success);
    QVERIFY(i16 == -32);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_qint32(&i32, (quint8 *)packed.data(), &success);
    QVERIFY(i32 == -32);
    QVERIFY(success);
    MsgPack::Ext::unpack_upto_qint64(&i64, (quint8 *)packed.data(), &success);
    QVERIFY(i64 == -32);
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint8>::min() - 1);
    MsgPack::Ext::unpack_upto_qint8(&i8, (quint8 *)packed.data(), &success);
    QVERIFY(i8 == std::numeric_limits<quint8>::min() - 1);
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint16>::min() - 1);
    MsgPack::Ext::unpack_upto_qint16(&i16, (quint8 *)packed.data(), &success);
    QVERIFY(i16 == std::numeric_limits<quint16>::min() - 1);
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint32>::min() - 1);
    MsgPack::Ext::unpack_upto_qint32(&i32, (quint8 *)packed.data(), &success);
    QVERIFY(i32 == std::numeric_limits<quint32>::min() - 1);
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<quint64>::min() - 1);
    MsgPack::Ext::unpack_upto_qint64(&i64, (quint8 *)packed.data(), &success);
    QVERIFY(i64 == std::numeric_limits<quint64>::min() - 1);
    QVERIFY(success);
}

void ExtText::test_upto_qint_to_quint()
{
    QByteArray packed;
    bool success;
    qint8 i8;
    qint16 i16;
    qint32 i32;
    qint64 i64;

    packed = MsgPack::pack(std::numeric_limits<qint8>::max());
    MsgPack::Ext::unpack_upto_qint8(&i8, (quint8 *)packed.data(), &success);
    QVERIFY(i8 == std::numeric_limits<qint8>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<qint16>::max());
    MsgPack::Ext::unpack_upto_qint16(&i16, (quint8 *)packed.data(), &success);
    QVERIFY(i16 == std::numeric_limits<qint16>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<qint32>::max());
    MsgPack::Ext::unpack_upto_qint32(&i32, (quint8 *)packed.data(), &success);
    QVERIFY(i32 == std::numeric_limits<qint32>::max());
    QVERIFY(success);

    packed = MsgPack::pack(std::numeric_limits<qint64>::max());
    MsgPack::Ext::unpack_upto_qint64(&i64, (quint8 *)packed.data(), &success);
    QVERIFY(i64 == std::numeric_limits<qint64>::max());
    QVERIFY(success);
}

void ExtText::test_fail()
{
	quint8 p[] = {0xd9};
	quint8 u8;
	bool ok;
	quint8 *p2 = MsgPack::Ext::unpack_upto_quint8(&u8, p, &ok);
	QVERIFY(!ok);
	QVERIFY(p2 - p == 0);
}

QTEST_APPLESS_MAIN(ExtText)

#include "ext_test.moc"
