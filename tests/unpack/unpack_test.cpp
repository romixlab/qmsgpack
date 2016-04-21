#include <QString>
#include <QtTest>
#include <QDebug>
#include <msgpack.h>
#include <limits>

class UnpackTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_booleans();
    void test_integers();
    void test_floats();
    void test_strings();
    void test_binary();
    void test_array();
    void test_map();
};


void UnpackTest::test_booleans()
{
    QByteArray pack = MsgPack::pack(QVariantList() << true << false);
    QVariantList u = MsgPack::unpack(pack).toList();
    QVERIFY(u[0] == true);
    QVERIFY(u[1] == false);
}

void UnpackTest::test_integers()
{
    QByteArray ints = QByteArray::fromBase64("3AAUAH//4cyAzP/Q39CAzQEAzf//0f9/"
                                             "0YAAzgABAADO/////9L//3//0oAAAADP"
                                             "AAAAAQAAAADP///////////T/////3//"
                                             "///TgAAAAAAAAAA=");
    QVariantList l = MsgPack::unpack(ints).toList();
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

    QByteArray pack = MsgPack::pack((qint64)1111111111);
    qint64 u = MsgPack::unpack(pack).toLongLong();
    QVERIFY(u == 1111111111);

    pack = MsgPack::pack((qint64)-3333333333);
    u = MsgPack::unpack(pack).toLongLong();
    QVERIFY(u == -3333333333);

    pack = MsgPack::pack((qint64)3333333333);
    u = MsgPack::unpack(pack).toLongLong();
    QVERIFY(u == 3333333333);
}

void UnpackTest::test_floats()
{
    QByteArray packed = QByteArray::fromBase64("k8sAAAAAAAAAAMtAHTMzMzMzM8tHM"
                                               "0JhcsdNgg==");
    QVariantList l = MsgPack::unpack(packed).toList();
    QVERIFY(l[0].toDouble() == 0.0);
    QVERIFY(l[1].toDouble() == 7.3);
    QVERIFY(l[2].toDouble() == pow(10, 35));
}

void UnpackTest::test_strings()
{
    QStringList strs;
    strs << "abc";
    QString s;
    for (int i = 0; i < 8; ++i)
        s += "xy";
    strs << s;
    s = QString();
    for (int i = 0; i < 64; ++i)
        s += "msgp";
    strs << s;
    s = QString();
    for (int i = 0; i < 4096; ++i)
        s += "messagepack test";
    strs << s;
    strs << "";

    QByteArray arr = MsgPack::pack(strs);

    QStringList l = MsgPack::unpack(arr).toStringList();
    QVERIFY(l[0] == strs[0]);
    QVERIFY(l[1] == strs[1]);
    QVERIFY(l[2] == strs[2]);
    QVERIFY(l[3] == strs[3]);
    QVERIFY(l[4].isEmpty());
}

void UnpackTest::test_binary()
{
    QVariantList l;
    l << QByteArray(16, 'm');
    l << QByteArray(256, 's');
    l << QByteArray(65536, 'g');
    l << QByteArray(0, 'p');

    QByteArray arr = MsgPack::pack(l);
    QVariantList u = MsgPack::unpack(arr).toList();

    QVERIFY(u[0].toByteArray() == l[0].toByteArray());
    QVERIFY(u[1].toByteArray() == l[1].toByteArray());
    QVERIFY(u[2].toByteArray() == l[2].toByteArray());
    QVERIFY(u[3].toByteArray().isEmpty());
}

void UnpackTest::test_array()
{
    QVariantList l;
    QVERIFY(MsgPack::unpack(MsgPack::pack(l)).toList().isEmpty());

    l << 0;
    QByteArray pack = MsgPack::pack(l);
    QVariantList lu = MsgPack::unpack(pack).toList();
    QVERIFY(lu.size() == 1);
    QVERIFY(lu[0] == 0);

    l = QVariantList();
    for (int i = 0; i < 256; ++i)
        l << i;
    pack = MsgPack::pack(l);
    lu = MsgPack::unpack(pack).toList();
    QVERIFY(lu.size() == 256);
    for (int i = 0; i < 256; ++i)
        QVERIFY(lu[i] == i);

    l = QVariantList();
    for(int i = 0; i < 65536; ++i)
        l << i;
    pack = MsgPack::pack(l);
    lu = MsgPack::unpack(pack).toList();
    QVERIFY(lu.size() == 65536);
    for (int i = 0; i < 65536; ++i)
        QVERIFY(lu[i] == i);
}

void UnpackTest::test_map()
{
    QVariantMap m;
    QVariantMap unpacked_m;

    qint64 ll = 339625541;
    m["ll"] = ll;

    unpacked_m = MsgPack::unpack(MsgPack::pack(m)).toMap();
    qDebug() << m << "<=>" << unpacked_m;
    QVERIFY(m == unpacked_m);

    m.clear();
    ll = 3333642741;
    m["ll"] = ll;

    QByteArray packArray = MsgPack::pack(m);
    unpacked_m = MsgPack::unpack(packArray).toMap();
    qDebug() << m << "<=>" << unpacked_m;
    QVERIFY(m == unpacked_m);

    m.clear();
    ll = 33336427413;
    m["ll"] = ll;

    unpacked_m = MsgPack::unpack(MsgPack::pack(m)).toMap();
    qDebug() << m << "<=>" << unpacked_m;
    QVERIFY(m == unpacked_m);

    m.clear();
    ll = 932838457459459;
    m["ll"] = ll;

    unpacked_m = MsgPack::unpack(MsgPack::pack(m)).toMap();
    qDebug() << m << "<=>" << unpacked_m;
    QVERIFY(m == unpacked_m);
}

QTEST_APPLESS_MAIN(UnpackTest)

#include "unpack_test.moc"
