#ifndef SIMPLIFYQT_H
#define SIMPLIFYQT_H

#include <QVector>
#include <QPointF>

namespace SimplifyQt {

class Segment
{
public:
    Q_DECL_CONSTEXPR inline Segment();
    Q_DECL_CONSTEXPR inline Segment(const Segment &other);
    Q_DECL_CONSTEXPR inline Segment(const QPointF &point);
    Q_DECL_CONSTEXPR inline Segment(const QPointF &point, const QPointF &cp1);

public:
    Q_DECL_RELAXED_CONSTEXPR inline void setControl1(const QPointF &pos);
    Q_DECL_RELAXED_CONSTEXPR inline void setControl2(const QPointF &pos);
    Q_DECL_RELAXED_CONSTEXPR inline void setEndPoint(const QPointF &pos);

    Q_DECL_CONSTEXPR inline const QPointF &control1() const;
    Q_DECL_CONSTEXPR inline const QPointF &control2() const;
    Q_DECL_CONSTEXPR inline const QPointF &endPoint() const;

    Q_DECL_RELAXED_CONSTEXPR inline void setControl1X(qreal xpos);
    Q_DECL_RELAXED_CONSTEXPR inline void setControl1Y(qreal ypos);
    Q_DECL_RELAXED_CONSTEXPR inline void setControl2X(qreal xpos);
    Q_DECL_RELAXED_CONSTEXPR inline void setControl2Y(qreal ypos);
    Q_DECL_RELAXED_CONSTEXPR inline void setEndPointX(qreal xpos);
    Q_DECL_RELAXED_CONSTEXPR inline void setEndPointY(qreal ypos);

    Q_DECL_CONSTEXPR inline qreal control1X() const;
    Q_DECL_CONSTEXPR inline qreal control1Y() const;
    Q_DECL_CONSTEXPR inline qreal control2X() const;
    Q_DECL_CONSTEXPR inline qreal control2Y() const;
    Q_DECL_CONSTEXPR inline qreal endPointX() const;
    Q_DECL_CONSTEXPR inline qreal endPointY() const;

public:
    Q_DECL_CONSTEXPR inline bool operator ==(const Segment & other);
    Q_DECL_CONSTEXPR inline bool operator !=(const Segment & other);

#ifndef QT_NO_DATASTREAM
    inline void save(QDataStream &ds) const;
    inline void load(QDataStream &ds);
#endif // QT_NO_DATASTREAM

private:
    QPointF _control1;
    QPointF _control2;
    QPointF _endPoint;
};

/*****************************************************************************
  Segment inline functions
 *****************************************************************************/

Q_DECL_CONSTEXPR inline Segment::Segment()
{
}

Q_DECL_CONSTEXPR inline Segment::Segment(const Segment &other)
    : _control1(other._control1)
    , _control2(other._control2)
    , _endPoint(other._endPoint)
{
}

Q_DECL_CONSTEXPR inline Segment::Segment(const QPointF &endPoint)
    : _endPoint(endPoint)
{
}

Q_DECL_CONSTEXPR inline Segment::Segment(const QPointF &endPoint, const QPointF &control1)
    : _endPoint(endPoint)
    , _control1(control1)
{
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl1(const QPointF &pos)
{
    _control1 = pos;
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl2(const QPointF &pos)
{
    _control2 = pos;
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setEndPoint(const QPointF &pos)
{
    _endPoint = pos;
}

Q_DECL_CONSTEXPR inline const QPointF &Segment::control1() const
{
    return _control1;
}

Q_DECL_CONSTEXPR inline const QPointF &Segment::control2() const
{
    return _control2;
}

Q_DECL_CONSTEXPR inline const QPointF &Segment::endPoint() const
{
    return _endPoint;
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl1X(qreal xpos)
{
    _control1.setX(xpos);
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl1Y(qreal ypos)
{
    _control1.setY(ypos);
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl2X(qreal xpos)
{
    _control2.setX(xpos);
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setControl2Y(qreal ypos)
{
    _control2.setY(ypos);
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setEndPointX(qreal xpos)
{
    _endPoint.setX(xpos);
}

Q_DECL_RELAXED_CONSTEXPR inline void Segment::setEndPointY(qreal ypos)
{
    _endPoint.setY(ypos);
}

Q_DECL_CONSTEXPR inline qreal Segment::control1X() const
{
    return _control1.x();
}

Q_DECL_CONSTEXPR inline qreal Segment::control1Y() const
{
    return _control1.y();
}

Q_DECL_CONSTEXPR inline qreal Segment::control2X() const
{
    return _control2.x();
}

Q_DECL_CONSTEXPR inline qreal Segment::control2Y() const
{
    return _control2.y();
}

Q_DECL_CONSTEXPR inline qreal Segment::endPointX() const
{
    return _endPoint.x();
}

Q_DECL_CONSTEXPR inline qreal Segment::endPointY() const
{
    return _endPoint.y();
}

Q_DECL_CONSTEXPR inline bool Segment::operator ==(const Segment & other)
{
    return (_control1 == other._control1) && (_control2 == other._control2) && (_endPoint == other._endPoint);
}

Q_DECL_CONSTEXPR inline bool Segment::operator !=(const Segment & other)
{
    return (_control1 != other._control1) || (_control2 != other._control2) || (_endPoint != other._endPoint);
}
#ifndef QT_NO_DATASTREAM

inline void Segment::save(QDataStream &ds) const
{
    ds << _control1 << _control2 << _endPoint;
}

inline void Segment::load(QDataStream &ds)
{
    ds >> _control1 >> _control2 >> _endPoint;
}

#endif // QT_NO_DATASTREAM

QVector<Segment> simplifyIs(const QVector<QPointF> &points, qreal tolerance = 2.5);
QVector<Segment> simplifySw(const QVector<QPointF> &points, qreal tolerance = 2.5);

} // namespace SimplifyQt

Q_DECLARE_TYPEINFO(SimplifyQt::Segment, Q_MOVABLE_TYPE);

/*****************************************************************************
  QPoint stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
inline QDataStream &operator<<(QDataStream &ds, const SimplifyQt::Segment &s) { s.save(ds); return ds; }
inline QDataStream &operator>>(QDataStream &ds, SimplifyQt::Segment &s) { s.load(ds); return ds; }
#endif // QT_NO_DATASTREAM

#endif // SIMPLIFYQT_H
