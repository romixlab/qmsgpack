#include <QString>
#include <QtTest>

class d : public QObject
{
    Q_OBJECT

public:
    d();

private Q_SLOTS:
    void testCase1();
};

d::d()
{
}

void d::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(d)

#include "tst_d.moc"
