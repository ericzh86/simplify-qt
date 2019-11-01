#ifndef SIMPLIFYTEST_H
#define SIMPLIFYTEST_H

#include <QObject>

#include "SimplifyQt.h"

class SimplifyTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void simplifySw();
    void simplifyIs();
private:
    QVector<SimplifyQt::Segment> segmentsSw;
    QVector<SimplifyQt::Segment> segmentsIs;

private slots:
    void compareSegments();

public slots:
    void evaluate1Sw();
    void evaluate1Is();
public slots:
    void evaluate2Sw();
    void evaluate2Is();
public slots:
    void evaluate3Sw();
    void evaluate3Is();

public slots:
    void dotSw();
    void dotIs();
public slots:
    void getDistanceSw();
    void getDistanceIs();
public slots:
    void normalizeSw();
    void normalizeIs();
public slots:
    void getLengthSw();
    void getLengthIs();

private:
    QVector<QPointF> points;
};

#endif // SIMPLIFYTEST_H
