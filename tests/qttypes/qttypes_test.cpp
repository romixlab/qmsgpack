#include <QString>
#include <QtTest>
#include <QDebug>
#include <msgpack.h>
#include <limits>

class QtTypesTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_qtime();
    void test_qdate();
    void test_qpoint();
    void test_qsize();
    void test_qrect();
};

void QtTypesTest::test_qtime()
{
    MsgPack::registerType(QMetaType::QTime, 0x77);
    QTime t(0, 0, 0, 0);
    QByteArray packed = MsgPack::pack(t);
    QTime t2 = MsgPack::unpack(packed).toTime();
    QVERIFY(t == t2);
    QVERIFY(packed.size() == 4);

    t = QTime(12, 01, 01, 0);
    packed = MsgPack::pack(t);
    t2 = MsgPack::unpack(packed).toTime();
    QVERIFY(t == t2);
    QVERIFY(packed.size() == 4);

    t = QTime(12, 59, 59, 0);
    packed = MsgPack::pack(t);
    t2 = MsgPack::unpack(packed).toTime();
    QVERIFY(t == t2);
    QVERIFY(packed.size() == 4);

    t = QTime(12, 34, 56, 999);
    packed = MsgPack::pack(t);
    t2 = MsgPack::unpack(packed).toTime();
    QVERIFY(t == t2);
    QVERIFY(packed.size() == 6);
}

void QtTypesTest::test_qdate()
{
    MsgPack::registerType(QMetaType::QDate, 0x78);
    QDate d;
    QByteArray packed = MsgPack::pack(d);
    QDate d2 = MsgPack::unpack(packed).toDate();
    QVERIFY(d == d2);
    QVERIFY(packed.size() == 6);

    d = QDate(1234, 12, 1);
    packed = MsgPack::pack(d);
    d2 = MsgPack::unpack(packed).toDate();
    QVERIFY(d == d2);

    d = QDate(9999, 1, 31);
    packed = MsgPack::pack(d);
    d2 = MsgPack::unpack(packed).toDate();
    QVERIFY(d == d2);
}

void QtTypesTest::test_qpoint()
{
    MsgPack::registerType(QMetaType::QPoint, 0x79);
	QPoint pt(1, 2);
	QByteArray packed = MsgPack::pack(pt);
	QVERIFY(packed.size() == 4);
	QPoint pt2 = MsgPack::unpack(packed).toPoint();
	QVERIFY(pt == pt2);

    pt = QPoint(1234, 5678);
    packed = MsgPack::pack(pt);
    QVERIFY(packed.size() == 9);
    pt2 = MsgPack::unpack(packed).toPoint();
    qDebug() << pt << pt2;
    qDebug() << packed.toHex() << packed.size();
    QVERIFY(pt == pt2);

    pt = QPoint(std::numeric_limits<qint32>::max(), std::numeric_limits<qint32>::max());
    packed = MsgPack::pack(pt);
    QVERIFY(packed.size() == 13);
    pt2 = MsgPack::unpack(packed).toPoint();
    QVERIFY(pt == pt2);
}

void QtTypesTest::test_qsize()
{
    MsgPack::registerType(QMetaType::QSize, 80);
    QSize sz(1, 2);
    QByteArray packed = MsgPack::pack(sz);
    QVERIFY(packed.size() == 4);
    QSize sz2 = MsgPack::unpack(packed).toSize();
    QVERIFY(sz == sz2);

    sz = QSize(1234, 5678);
    packed = MsgPack::pack(sz);
    QVERIFY(packed.size() == 9);
    sz2 = MsgPack::unpack(packed).toSize();
    QVERIFY(sz == sz2);
}

void QtTypesTest::test_qrect()
{
    MsgPack::registerType(QMetaType::QRect, 81);
    QRect r(1, 2, 3, 4);
    QByteArray packed = MsgPack::pack(r);
    QVERIFY(packed.size() == 6);
    QRect r2 = MsgPack::unpack(packed).toRect();
    QVERIFY(r == r2);

    qint32 max = std::numeric_limits<qint32>::max();
    r = QRect(0, 0, max, max);
    packed = MsgPack::pack(r);
    QVERIFY(packed.size() == 15);
    r2 = MsgPack::unpack(packed).toRect();
    QVERIFY(r == r2);
}

QTEST_APPLESS_MAIN(QtTypesTest)

#include "qttypes_test.moc"
