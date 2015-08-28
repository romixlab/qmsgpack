#include <QString>
#include <QtTest>
#include <QDebug>
#include <msgpack.h>
#include <limits>
#include <msgpack_ext.h>

class Big : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_big_list();
};

void Big::test_big_list()
{
    QVariantList list;
    for (int i = 0; i < 10000; ++i)
        list << i;
    QByteArray packed;
    QVariantList unpacked;
    QBENCHMARK {
        packed = MsgPack::pack(list);
        unpacked = MsgPack::unpack(packed).toList();
    }
    bool listOk = true;
    for (int i = 0; i < unpacked.size(); ++i) {
        if (unpacked[i].toInt() != i) {
            listOk = false;
        }
    }
    QVERIFY(listOk);
}


QTEST_APPLESS_MAIN(Big)

#include "big.moc"
