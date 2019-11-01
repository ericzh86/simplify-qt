#include "SimplifyQt.h"

#include "private/PathFitterIs.h"
#include "private/PathFitterSw.h"

namespace SimplifyQt {

QVector<Segment> simplifyIs(const QVector<QPointF> &points, qreal tolerance)
{
    return SimplifyQt::PathFitterIs(points).fit(tolerance);
}

QVector<Segment> simplifySw(const QVector<QPointF> &points, qreal tolerance)
{
    return SimplifyQt::PathFitterSw(points).fit(tolerance);
}

} // namespace SimplifyQt
