#include "simplifytest.h"

#include <QtTest>

#include "private/PathFitterIs.h"
#include "private/PathFitterSw.h"

// class SimplifyTest

void SimplifyTest::initTestCase()
{
    int yoffset = 0;
    for (int i = 0; i < 10000; ++i) {
        int r = qrand() % 50 - 25;
        int y = r + 100 + yoffset;
        yoffset += r;
        points.append(QPointF(i * 3, y));
    }
}

void SimplifyTest::cleanupTestCase()
{
}

void SimplifyTest::simplifySw()
{
    QBENCHMARK {
        segmentsSw = SimplifyQt::simplifySw(points);
    }
}

void SimplifyTest::simplifyIs()
{
    QBENCHMARK {
        segmentsIs = SimplifyQt::simplifyIs(points);
    }
}

void SimplifyTest::compareSegments()
{
    QVERIFY(segmentsSw.count() == segmentsIs.count());

    int c = segmentsSw.count();
    for (int i = 0; i < c; ++i) {
        QVERIFY(segmentsSw[i] == segmentsIs[i]);
    }
}

void SimplifyTest::evaluate1Sw()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterSw::evaluate1(curves.constData(), 0.5);
    }
}

void SimplifyTest::evaluate1Is()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterIs::evaluate1(curves.constData(), 0.5);
    }
}

void SimplifyTest::evaluate2Sw()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterSw::evaluate2(curves.constData(), 0.5);
    }
}

void SimplifyTest::evaluate2Is()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterIs::evaluate2(curves.constData(), 0.5);
    }
}

void SimplifyTest::evaluate3Sw()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterSw::evaluate3(curves.constData(), 0.5);
    }
}

void SimplifyTest::evaluate3Is()
{
    QVector<QPointF> curves;
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));
    curves.append(QPointF(1.3, 2.6));

    QBENCHMARK {
        SimplifyQt::PathFitterIs::evaluate3(curves.constData(), 0.5);
    }
}

void SimplifyTest::dotSw()
{
    QPointF o(1.3, 2.6);
    QPointF p(2.3, 3.6);

    QBENCHMARK {
        SimplifyQt::PathFitterSw::dot(o, p);
    }
}

void SimplifyTest::dotIs()
{
    QPointF o(1.3, 2.6);
    QPointF p(2.3, 3.6);

    QBENCHMARK {
        SimplifyQt::PathFitterIs::dot(o, p);
    }
}

void SimplifyTest::getDistanceSw()
{
    QPointF o(1.3, 2.6);
    QPointF p(2.3, 3.6);

    QBENCHMARK {
        SimplifyQt::PathFitterSw::getDistance(o, p);
    }
}

void SimplifyTest::getDistanceIs()
{
    QPointF o(1.3, 2.6);
    QPointF p(2.3, 3.6);

    QBENCHMARK {
        SimplifyQt::PathFitterIs::getDistance(o, p);
    }
}

void SimplifyTest::normalizeSw()
{
    QPointF o(1.3, 2.6);

    QBENCHMARK {
        SimplifyQt::PathFitterSw::normalize(o);
    }
}

void SimplifyTest::normalizeIs()
{
    QPointF o(1.3, 2.6);

    QBENCHMARK {
        SimplifyQt::PathFitterIs::normalize(o);
    }
}

void SimplifyTest::getLengthSw()
{
    QPointF o(1.3, 2.6);

    QBENCHMARK {
        SimplifyQt::PathFitterSw::getLength(o);
    }
}

void SimplifyTest::getLengthIs()
{
    QPointF o(1.3, 2.6);

    QBENCHMARK {
        SimplifyQt::PathFitterIs::getLength(o);
    }
}

QTEST_APPLESS_MAIN(SimplifyTest)
