#ifndef PATHFITTERIS_H
#define PATHFITTERIS_H

#include "../SimplifyQt.h"

#include <xmmintrin.h>
#include <emmintrin.h>

namespace SimplifyQt {

class PathFitterIs
{
public:
    explicit PathFitterIs(const QVector<QPointF> &points)
        : points(points) {
        if (!points.isEmpty()) {
            QPointF last = points.first();
            for (const QPointF &point : points) {
                distances.append(getDistance(last, point));
                last = point;
            }
        }
    }

public:
    QVector<Segment> fit(qreal error)
    {
        /* JavaScript
        if (length > 0) {
            segments = [new Segment(points[0])];
            if (length > 1) {
                this.fitCubic(segments, error, 0, length - 1,
                        points[1].subtract(points[0]),
                        points[length - 2].subtract(points[length - 1]));

                if (this.closed) {
                    segments.shift();
                    segments.pop();
                }
            }
        }
        return segments;
         */

        QVector<Segment> segments;

        int c = points.count();
        if (c > 0) {
            segments.append(Segment(points.first()));
            if (c > 1) {
                fitCubic(segments, error, 0, c - 1,
                         points[1] - points[0], points[c - 2] - points[c - 1]);
            }
        }

        return segments;
    }

public:
    void fitCubic(QVector<Segment> &segments, qreal error, int first, int last, const QPointF &tan1, const QPointF &tan2) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var points = this.points;
        if (last - first === 1) {
            var pt1 = points[first],
                pt2 = points[last],
                dist = pt1.getDistance(pt2) / 3;
            this.addCurve(segments, [pt1, pt1.add(tan1.normalize(dist)),
                    pt2.add(tan2.normalize(dist)), pt2]);
            return;
        }
        */

        if ((last - first) == 1) {
            const QPointF &pt1 = points[first];
            const QPointF &pt2 = points[last];
            qreal dist = getDistance(pt1, pt2) / 3;
            addCurve(segments,
                     pt1,
                     pt1 + normalize(tan1, dist),
                     pt2 + normalize(tan2, dist),
                     pt2);
            return;
        }

        /* JavaScript
        var uPrime = this.chordLengthParameterize(first, last),
            maxError = Math.max(error, error * error),
            split,
            parametersInOrder = true;
        for (var i = 0; i <= 4; i++) {
            var curve = this.generateBezier(first, last, uPrime, tan1, tan2);
            var max = this.findMaxError(first, last, curve, uPrime);
            if (max.error < error && parametersInOrder) {
                this.addCurve(segments, curve);
                return;
            }
            split = max.index;
            if (max.error >= maxError)
                break;
            parametersInOrder = this.reparameterize(first, last, uPrime, curve);
            maxError = max.error;
        }
        var tanCenter = points[split - 1].subtract(points[split + 1]);
        this.fitCubic(segments, error, first, split, tan1, tanCenter);
        this.fitCubic(segments, error, split, last, tanCenter.negate(), tan2);
        */

        QVector<qreal> uPrime = chordLengthParameterize(first, last);
        qreal maxError = qMax(error, error * error);
        int split = 0;
        bool parametersInOrder = true;
        for (int i = 0; i <= 4; ++i) {
            QPointF curve[4];
            generateBezier(first, last, uPrime, tan1, tan2, curve);
            QPair<qreal, int> max = findMaxError(first, last, curve, uPrime);
            if ((max.first < error) && parametersInOrder) {
                addCurve(segments, curve[0], curve[1], curve[2], curve[3]);
                return;
            }
            split = max.second;
            if (max.first >= maxError)
                break;
            parametersInOrder = reparameterize(first, last, uPrime, curve);
            maxError = max.first;
        }
        QPointF tanCenter = points[split - 1] - points[split + 1];

        fitCubic(segments, error, first, split, tan1, tanCenter);
        fitCubic(segments, error, split, last, tanCenter * -1, tan2);
    }

    void generateBezier(int first, int last, const QVector<qreal> &uPrime, const QPointF &tan1, const QPointF &tan2, QPointF *curves) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var epsilon = Numerical.EPSILON,
            abs = Math.abs,
            points = this.points,
            pt1 = points[first],
            pt2 = points[last],
            C = [[0, 0], [0, 0]],
            X = [0, 0];

        for (var i = 0, l = last - first + 1; i < l; i++) {
            var u = uPrime[i],
                t = 1 - u,
                b = 3 * u * t,
                b0 = t * t * t,
                b1 = b * t,
                b2 = b * u,
                b3 = u * u * u,
                a1 = tan1.normalize(b1),
                a2 = tan2.normalize(b2),
                tmp = points[first + i]
                    .subtract(pt1.multiply(b0 + b1))
                    .subtract(pt2.multiply(b2 + b3));
            C[0][0] += a1.dot(a1);
            C[0][1] += a1.dot(a2);
            C[1][0] = C[0][1];
            C[1][1] += a2.dot(a2);
            X[0] += a1.dot(tmp);
            X[1] += a2.dot(tmp);
        }
        */

        // https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Number/EPSILON
        qreal epsilon = std::pow(2, -52);
        const QPointF &pt1 = points[first];
        const QPointF &pt2 = points[last];
        qreal C[2][2] = {{0, 0}, {0, 0}};
        qreal X[2] = {0, 0};

        for (int i = 0, l = last - first + 1; i < l; ++i) {
            qreal u = uPrime[i];
            qreal t = 1 - u;
            qreal b = 3 * u * t;
            qreal b0 = t * t * t;
            qreal b1 = b * t;
            qreal b2 = b * u;
            qreal b3 = u * u * u;
            QPointF a1 = normalize(tan1, b1);
            QPointF a2 = normalize(tan2, b2);
            QPointF tmp = points[first + i]
                    - pt1 * (b0 + b1)
                    - pt2 * (b2 + b3);
            C[0][0] += dot(a1, a1);
            C[0][1] += dot(a1, a2);
            C[1][0] = C[0][1];
            C[1][1] += dot(a2, a2);
            X[0] += dot(a1, tmp);
            X[1] += dot(a2, tmp);
        }

        /* JavaScript
        var detC0C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1],
            alpha1,
            alpha2;
        if (abs(detC0C1) > epsilon) {
            var detC0X = C[0][0] * X[1]    - C[1][0] * X[0],
                detXC1 = X[0]    * C[1][1] - X[1]    * C[0][1];
            alpha1 = detXC1 / detC0C1;
            alpha2 = detC0X / detC0C1;
        } else {
            var c0 = C[0][0] + C[0][1],
                c1 = C[1][0] + C[1][1];
            alpha1 = alpha2 = abs(c0) > epsilon ? X[0] / c0
                            : abs(c1) > epsilon ? X[1] / c1
                            : 0;
        }
        */

        qreal detC0C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
        qreal alpha1 = 0.0;
        qreal alpha2 = 0.0;

        if (std::abs(detC0C1) > epsilon) {
            qreal detC0X = C[0][0] * X[1] - C[1][0] * X[0];
            qreal detXC1 = X[0] * C[1][1] - X[1] * C[0][1];
            alpha1 = detXC1 / detC0C1;
            alpha2 = detC0X / detC0C1;
        } else {
            qreal c0 = C[0][0] + C[0][1];
            qreal c1 = C[1][0] + C[1][1];
            /* JavaScript
            alpha1 = alpha2 = abs(c0) > epsilon ? X[0] / c0
                            : abs(c1) > epsilon ? X[1] / c1
                            : 0;
            */
            if (std::abs(c0) > epsilon) {
                alpha1 = alpha2 = X[0] / c0;
            } else if (std::abs(c1) > epsilon) {
                alpha1 = alpha2 = X[1] / c1;
            } else {
                alpha1 = alpha2 = 0.0;
            }
        }

        /* JavaScript
        var segLength = pt2.getDistance(pt1),
            eps = epsilon * segLength,
            handle1,
            handle2;
        if (alpha1 < eps || alpha2 < eps) {
            alpha1 = alpha2 = segLength / 3;
        } else {
            var line = pt2.subtract(pt1);
            handle1 = tan1.normalize(alpha1);
            handle2 = tan2.normalize(alpha2);
            if (handle1.dot(line) - handle2.dot(line) > segLength * segLength) {
                alpha1 = alpha2 = segLength / 3;
                handle1 = handle2 = null;
            }
        }

        return [pt1,
                pt1.add(handle1 || tan1.normalize(alpha1)),
                pt2.add(handle2 || tan2.normalize(alpha2)),
                pt2];
        */

        qreal segLength = getDistance(pt2, pt1);
        qreal eps = epsilon * segLength;
        QPointF handle1;
        QPointF handle2;
        if (alpha1 < eps || alpha2 < eps) {
            alpha1 = alpha2 = segLength / 3;
        } else {
            QPointF line = pt2 - pt1;
            handle1 = normalize(tan1, alpha1);
            handle2 = normalize(tan2, alpha2);
            if ((dot(handle1, line) - dot(handle2, line)) > (segLength * segLength)) {
                alpha1 = alpha2 = segLength / 3;
                handle1 = handle2 = QPointF();
            }
        }

        curves[0] = pt1;
        curves[1] = pt1 + (handle1.isNull() ? normalize(tan1, alpha1) : handle1);
        curves[2] = pt2 + (handle2.isNull() ? normalize(tan2, alpha2) : handle2);
        curves[3] = pt2;
    }

    void addCurve(QVector<Segment> &segments, const QPointF &curve0, const QPointF &curve1, const QPointF &curve2, const QPointF &curve3) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var prev = segments[segments.length - 1];
        prev.setHandleOut(curve[1].subtract(curve[0]));
        segments.push(new Segment(curve[3], curve[2].subtract(curve[3])));
        */

        Segment &segment = segments.last();
        segment.setControl2(curve1 - curve0);
        segments << Segment(curve3, curve2 - curve3);
    }

    bool reparameterize(int first, int last, const QVector<qreal> &u, const QPointF *curves) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        for (var i = first; i <= last; i++) {
            u[i - first] = this.findRoot(curve, this.points[i], u[i - first]);
        }
        for (var i = 1, l = u.length; i < l; i++) {
            if (u[i] <= u[i - 1])
                return false;
        }
        return true;
        */

        QVector<qreal> u2 = u;
        for (int i = first; i <= last; ++i) {
            u2[i - first] = findRoot(curves, points[i], u2[i - first]);
        }
        for (int i = 1, l = u2.count(); i < l; ++i) {
            if (u2[i] <= u2[i - 1]) {
                return false;
            }
        }

        return true;
    }

    qreal findRoot(const QPointF *curves, const QPointF &point, qreal u) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var curve1 = [],
            curve2 = [];
        for (var i = 0; i <= 2; i++) {
            curve1[i] = curve[i + 1].subtract(curve[i]).multiply(3);
        }
        for (var i = 0; i <= 1; i++) {
            curve2[i] = curve1[i + 1].subtract(curve1[i]).multiply(2);
        }
        var pt = this.evaluate(3, curve, u),
            pt1 = this.evaluate(2, curve1, u),
            pt2 = this.evaluate(1, curve2, u),
            diff = pt.subtract(point),
            df = pt1.dot(pt1) + diff.dot(pt2);
        return Numerical.isMachineZero(df) ? u : u - diff.dot(pt1) / df;
        */

        QPointF curve1[3];
        curve1[0] = curve1[1] - curve1[0] * 3;
        curve1[1] = curve1[2] - curve1[1] * 3;
        curve1[2] = curve1[3] - curve1[2] * 3;

        QPointF curve2[2];
        curve2[0] = curve1[1] - curve1[0] * 2;
        curve2[1] = curve1[2] - curve1[1] * 2;

        QPointF pt0 = evaluate3(curves, u);
        QPointF pt1 = evaluate2(curve1, u);
        QPointF pt2 = evaluate1(curve2, u);

        QPointF diff = pt0 - point;
        qreal df = dot(pt1, pt1) + dot(diff, pt2);
        return qFuzzyIsNull(df) ? u : (u - dot(diff, pt1) / df);
    }

    QPair<qreal, int> findMaxError(int first, int last, const QPointF *curves, const QVector<qreal> &u) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var index = Math.floor((last - first + 1) / 2),
            maxDist = 0;
        for (var i = first + 1; i < last; i++) {
            var P = this.evaluate(3, curve, u[i - first]);
            var v = P.subtract(this.points[i]);
            var dist = v.x * v.x + v.y * v.y;
            if (dist >= maxDist) {
                maxDist = dist;
                index = i;
            }
        }
        return {
            error: maxDist,
            index: index
        };
        */

        int index = std::floor((last - first + 1) / 2.0);
        qreal maxDist = 0.0;
        for (int i = first + 1; i < last; ++i) {
            QPointF P = evaluate3(curves, u[i - first]);
            QPointF v = P - points[i];
            qreal dist = v.x() * v.x() + v.y() * v.y();
            if (dist >= maxDist) {
                maxDist = dist;
                index = i;
            }
        }

        return QPair<qreal, int>(maxDist, index);
    }

    QVector<qreal> chordLengthParameterize(int first, int last) const
    {
        // src/path/PathFitter.js

        /* JavaScript
        var u = [0];
        for (var i = first + 1; i <= last; i++) {
            u[i - first] = u[i - first - 1]
                    + this.points[i].getDistance(this.points[i - 1]);
        }
        for (var i = 1, m = last - first; i <= m; i++) {
            u[i] /= u[m];
        }
        return u;
        */

        QVector<qreal> u{0};
        for (int i = first + 1; i <= last; ++i) {
            u << u[i - first - 1] + distances[i];
        }
        for (int i = 1, m = last - first; i <= m; ++i) {
            u[i] = u[i] / u[m];
        }

        return u;
    }

    static inline QPointF evaluate1(const QPointF *curves, qreal t)
    {
        // src/path/PathFitter.js

        /* JavaScript
        var tmp = curve.slice();
        for (var i = 1; i <= degree; i++) {
            for (var j = 0; j <= degree - i; j++) {
                tmp[j] = tmp[j].multiply(1 - t).add(tmp[j + 1].multiply(t));
            }
        }
        return tmp[0];
        */

        return curves[0] * (1 - t) + curves[1] * t;
    }

    static inline QPointF evaluate2(const QPointF *curves, qreal t)
    {
        // src/path/PathFitter.js

        /* JavaScript
        var tmp = curve.slice();
        for (var i = 1; i <= degree; i++) {
            for (var j = 0; j <= degree - i; j++) {
                tmp[j] = tmp[j].multiply(1 - t).add(tmp[j + 1].multiply(t));
            }
        }
        return tmp[0];
        */

        __m128d temp[3] {
            { curves[0].x(), curves[0].y() },
            { curves[1].x(), curves[1].y() },
            { curves[2].x(), curves[2].y() }
        };

        __m128d q1 { 1 - t, 1 - t };
        __m128d q2 { t    , t     };

        temp[0] = _mm_add_pd(_mm_mul_pd(temp[0], q1), _mm_mul_pd(temp[1], q2));
        temp[1] = _mm_add_pd(_mm_mul_pd(temp[1], q1), _mm_mul_pd(temp[2], q2));
        temp[0] = _mm_add_pd(_mm_mul_pd(temp[0], q1), _mm_mul_pd(temp[1], q2));

        return QPointF(temp[0].m128d_f64[0], temp[0].m128d_f64[1]);
    }

    static inline QPointF evaluate3(const QPointF *curves, qreal t)
    {
        // src/path/PathFitter.js

        /* JavaScript
        var tmp = curve.slice();
        for (var i = 1; i <= degree; i++) {
            for (var j = 0; j <= degree - i; j++) {
                tmp[j] = tmp[j].multiply(1 - t).add(tmp[j + 1].multiply(t));
            }
        }
        return tmp[0];
        */

        __m128d temp[4] {
            { curves[0].x(), curves[0].y() },
            { curves[1].x(), curves[1].y() },
            { curves[2].x(), curves[2].y() },
            { curves[3].x(), curves[3].y() }
        };

        __m128d q1 { 1 - t, 1 - t };
        __m128d q2 { t    , t     };

        temp[0] = _mm_add_pd(_mm_mul_pd(temp[0], q1), _mm_mul_pd(temp[1], q2));
        temp[1] = _mm_add_pd(_mm_mul_pd(temp[1], q1), _mm_mul_pd(temp[2], q2));
        temp[2] = _mm_add_pd(_mm_mul_pd(temp[2], q1), _mm_mul_pd(temp[3], q2));
        temp[0] = _mm_add_pd(_mm_mul_pd(temp[0], q1), _mm_mul_pd(temp[1], q2));
        temp[1] = _mm_add_pd(_mm_mul_pd(temp[1], q1), _mm_mul_pd(temp[2], q2));
        temp[0] = _mm_add_pd(_mm_mul_pd(temp[0], q1), _mm_mul_pd(temp[1], q2));

        return QPointF(temp[0].m128d_f64[0], temp[0].m128d_f64[1]);
    }

    static inline qreal dot(const QPointF &o, const QPointF &point)
    {
        // src/basic/Point.js

        /* JavaScript
        var point = Point.read(arguments);
        return this.x * point.x + this.y * point.y;
         */

        return QPointF::dotProduct(o, point);
    }

    static inline qreal getDistance(const QPointF &o, const QPointF &point)
    {
        // src/basic/Point.js

        /* JavaScript
        var point = Point.read(arguments),
            x = point.x - this.x,
            y = point.y - this.y,
            d = x * x + y * y,
            squared = Base.read(arguments);
        return squared ? d : Math.sqrt(d);
        */

        qreal x = point.x() - o.x();
        qreal y = point.y() - o.y();
        return std::sqrt(x * x + y * y);
    }

    static inline QPointF normalize(const QPointF &o, qreal length = 1.0)
    {
        // src/basic/Point.js

        /* JavaScript
        if (length === undefined)
            length = 1;
        var current = this.getLength(),
            scale = current !== 0 ? length / current : 0,
            point = new Point(this.x * scale, this.y * scale);
        if (scale >= 0)
            point._angle = this._angle;
        return point;
         */

        qreal current = getLength(o);
        qreal scale = qFuzzyIsNull(current) ? 0 : (length / current);
        return QPointF(o.x() * scale, o.y() * scale);
    }

    static inline qreal getLength(const QPointF &o)
    {
        // src/basic/Point.js

        /* JavaScript
        return Math.sqrt(this.x * this.x + this.y * this.y);
         */

        return std::sqrt(o.x() * o.x() + o.y() * o.y());
    }

private:
    QVector<QPointF> points;
    QVector<qreal>   distances;
}; // class PathFitterIs

} // namespace SimplifyQt

#endif // PATHFITTERIS_H
