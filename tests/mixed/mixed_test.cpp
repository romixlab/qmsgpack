#include <QString>
#include <QtTest>
#include <msgpack.h>
#include <QDebug>

class MixedTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_float();
    void test_double();
    void test_map();
    void test_ext();
    void test_mixed();
};

void MixedTest::test_float()
{
    float f;
    QByteArray packed;

    packed = MsgPack::pack(0.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == 0.0f);
    QVERIFY(packed.size() == 5);

    packed = MsgPack::pack(-0.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == -0.0f);
    QVERIFY(packed.size() == 5);

    packed = MsgPack::pack(1.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == 1.0f);
    QVERIFY(packed.size() == 5);

    packed = MsgPack::pack(-1.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == -1.0f);
    QVERIFY(packed.size() == 5);

    packed = MsgPack::pack(32767.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == 32767.0f);
    QVERIFY(packed.size() == 5);

    packed = MsgPack::pack(-32767.0f);
    f = MsgPack::unpack(packed).toFloat();
    QVERIFY(f == -32767.0f);
    QVERIFY(packed.size() == 5);
}

void MixedTest::test_double()
{
    double d;
    QByteArray packed;

    packed = MsgPack::pack(0.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == 0.0);
    QVERIFY(packed.size() == 9);

    packed = MsgPack::pack(-0.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == -0.0);
    QVERIFY(packed.size() == 9);

    packed = MsgPack::pack(1.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == 1.0);
    QVERIFY(packed.size() == 9);

    packed = MsgPack::pack(-1.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == -1.0);
    QVERIFY(packed.size() == 9);

    packed = MsgPack::pack(32767.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == 32767.0);
    QVERIFY(packed.size() == 9);

    packed = MsgPack::pack(-32767.0);
    d = MsgPack::unpack(packed).toDouble();
    QVERIFY(d == -32767.0);
    QVERIFY(packed.size() == 9);
}

void MixedTest::test_map()
{
    QVariantMap map;
    map.insert(QString("%1").QString::arg(0, 5), "v");
    QByteArray arr = MsgPack::pack(map);
    QVERIFY(arr.size() == map.size() * 8 + 1);
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == (0x80 | map.size()));
    QVariantMap m2 = MsgPack::unpack(arr).toMap();
    QVERIFY(m2 == map);

    for (int i = 1; i < 16; ++i)
        map.insert(QString("%1").QString::arg(i, 5), "v");
    arr = MsgPack::pack(map);
    QVERIFY(arr.size() == map.size() * 8 + 3);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xde);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x10);
    m2 = MsgPack::unpack(arr).toMap();
    QVERIFY(m2 == map);

    for (int i = 16; i < 65536; ++i)
        map.insert(QString("%1").QString::arg(i, 5), "v");
    arr = MsgPack::pack(map);
    QVERIFY(arr.size() == map.size() * 8 + 5);
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xdf);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
    m2 = MsgPack::unpack(arr).toMap();
    QVERIFY(m2 == map);
}

class CustomType
{
public:
    CustomType() {}
    CustomType(const CustomType &other) { m_size = other.m_size; }
    ~CustomType() {}

    CustomType(int size) : m_size(size) {}

    int size() { return m_size; }
    void setSize(int size) { m_size = size; }
private:
    int m_size;
};

Q_DECLARE_METATYPE(CustomType)

QByteArray pack_custom_type(const QVariant &variant)
{
    CustomType ct = variant.value<CustomType>();
    QByteArray data;
    data.resize(ct.size());
    quint8 *p = (quint8 *)data.data();
    for (int i = 0; i < ct.size(); ++i)
        p[i] = 7;
    return data;
}

QVariant unpack_custom_type(const QByteArray &data)
{
    return data;
}

void MixedTest::test_ext()
{
    CustomType ct(1);
    QVariant custom;
    custom.setValue(ct);

    bool packer_registered = MsgPack::registerPacker((QMetaType::Type)qMetaTypeId<CustomType>(),
                            3, pack_custom_type);
    QVERIFY(packer_registered);
    bool unpacker_registered = MsgPack::registerUnpacker(3, unpack_custom_type);
    QVERIFY(unpacker_registered);

    QByteArray arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 2 + ct.size());
    quint8 *p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd4);
    QVERIFY(p[1] == 0x03);
    QVERIFY(p[2] == 0x07);
    QByteArray ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(2);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 2 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd5);
    QVERIFY(p[1] == 0x03);
    QVERIFY(p[2] == 0x07);
    QVERIFY(p[3] == 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(4);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 2 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd6);
    QVERIFY(p[1] == 0x03);
    QVERIFY(p[2] == 0x07);
    QVERIFY(p[3] == 0x07);
    QVERIFY(p[4] == 0x07);
    QVERIFY(p[5] == 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(8);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 2 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd7);
    QVERIFY(p[1] == 0x03);
    for (int i = 0; i < ct.size(); ++i)
        QVERIFY(p[2 + i] = 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(16);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 2 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xd8);
    QVERIFY(p[1] == 0x03);
    for (int i = 0; i < ct.size(); ++i)
        QVERIFY(p[2 + i] = 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(3);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 3 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc7);
    QVERIFY(p[1] == 0x03);
    QVERIFY(p[2] == 0x03);
    for (int i = 0; i < ct.size(); ++i)
        QVERIFY(p[3 + i] = 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(256);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 4 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc8);
    QVERIFY(p[1] == 0x01);
    QVERIFY(p[2] == 0x00);
    QVERIFY(p[3] == 0x03);
    for (int i = 0; i < ct.size(); ++i)
        QVERIFY(p[4 + i] = 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);

    ct.setSize(65536);
    custom.setValue(ct);
    arr = MsgPack::pack(custom);
    QVERIFY(arr.size() == 6 + ct.size());
    p = (quint8 *)arr.data();
    QVERIFY(p[0] == 0xc9);
    QVERIFY(p[1] == 0x00);
    QVERIFY(p[2] == 0x01);
    QVERIFY(p[3] == 0x00);
    QVERIFY(p[4] == 0x00);
    QVERIFY(p[5] == 0x03);
    for (int i = 0; i < ct.size(); ++i)
        QVERIFY(p[6 + i] = 0x07);
    ub = MsgPack::unpack(arr).toByteArray();
    QVERIFY(ub.size() == ct.size());
    for (int i = 0; i < ub.size(); ++i)
        QVERIFY(ub.data()[i] == 7);
}

void MixedTest::test_mixed()
{
    QVariantMap map;
    map["booleans"] = QVariantList() << false << true;
    map["list2"] = QVariantList() << (QVariantList() << "abc" << "def")
                                  << (QVariantList() << "qwe" << "rty");
    map["integers"] = QVariantList() << 0 << 127 << -31 << 128 << 777;
    map["bytearray"] = QByteArray("bytes");

    QVariantMap map2;
    map2["one"] = 1;
    map2["two"] = QVariantMap();
    map["map2"] = map2;

    QByteArray arr = MsgPack::pack(map);

    QVariant unpacked = MsgPack::unpack(arr);

    QVERIFY(unpacked.toMap() == map);
}

QTEST_APPLESS_MAIN(MixedTest)

#include "mixed_test.moc"
