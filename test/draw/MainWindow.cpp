#include "MainWindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(800, 600);
    setWindowTitle("simplify-qt");
    QGridLayout *mainLayout = new QGridLayout();

    QPushButton *button2k5 = new QPushButton(tr("2k5 Points"), this);
    mainLayout->addWidget(button2k5, 0, 0);
    connect(button2k5, SIGNAL(clicked(bool)),
            this, SLOT(create2k5Points()));

    QPushButton *button10k = new QPushButton(tr("10k Points"), this);
    mainLayout->addWidget(button10k, 0, 1);
    connect(button10k, SIGNAL(clicked(bool)),
            this, SLOT(create10kPoints()));

    buttonPoint = new QRadioButton(tr("Point"), this);
    mainLayout->addWidget(buttonPoint, 0, 2);
    connect(buttonPoint, SIGNAL(released()),
            this, SLOT(update()));

    buttonSegment = new QRadioButton(tr("Segment"), this);
    mainLayout->addWidget(buttonSegment, 0, 3);
    connect(buttonSegment, SIGNAL(released()),
            this, SLOT(update()));

    buttonTotal = new QRadioButton(tr("Total"), this);
    mainLayout->addWidget(buttonTotal, 0, 4);
    connect(buttonTotal, SIGNAL(released()),
            this, SLOT(update()));

    buttonTotal->setChecked(true);

    mainLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred), 0, 5);
    mainLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding), 1, 0);

    setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
}

void MainWindow::create2k5Points()
{
    createPoints(50, 50);
}

void MainWindow::create10kPoints()
{
    createPoints(100, 100);
}

void MainWindow::createPoints(int xc, int yc)
{
    points.clear();
    segments.clear();

    int w = width() - 20;
    int h = height() - 20;

    qreal xscale = w * 1.0 / xc;
    qreal yscale = h * 1.0 / yc;

    for (int i = 0; i < xc; ++i) {
        for (int j = 0; j < yc; ++j) {
            int x = 10 + i * xscale + qrand() % 10;
            int y = 10 + j * yscale;

            points.append(QPointF(x, y));
        }
    }

    QElapsedTimer timer;
    timer.start();

    segments = SimplifyQt::simplifyIs(points);
    qint64 nsecs = timer.nsecsElapsed();
    int c = points.count();

    update();

    // update title

    qint64 ms = nsecs / 1000000;
    qint64 us = nsecs % 1000000 / 1000;

    QString text = QString::fromLatin1("%1 points, %2 segments, %3.%4 ms")
            .arg(c).arg(segments.count()).arg(ms).arg(us, 3, 10, QLatin1Char('0'));

    setWindowTitle(text);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    if (buttonTotal->isChecked() || buttonSegment->isChecked()) {
        painter.setPen(QPen(QColor(qRgb(0x00, 0x00, 0x00)), 7));
        // draw segments -->
        if (!segments.isEmpty()) {
            QPainterPath path;
            SimplifyQt::Segment l = segments.first();
            path.moveTo(l.endPoint());
            for (int i = 1; i < segments.count(); ++i) {
                SimplifyQt::Segment s = segments.at(i);
                path.cubicTo(l.endPoint() + l.control2(),
                             s.endPoint() + s.control1(),
                             s.endPoint());

                l = s;
            }
            painter.drawPath(path);
        }
        // <-- draw segments
    }

    if (buttonTotal->isChecked() || buttonPoint->isChecked() || pressed) {
        painter.setPen(QPen(QColor(qRgb(0xff, 0x00, 0x00)), 2));
        // draw points -->
        if (!points.isEmpty()) {
            QPointF l = points.first();
            for (int i = 1; i < points.count(); ++i) {
                QPointF p = points.at(i);
                painter.drawLine(l, p);

                l = p;
            }
        }
        // <-- draw points
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = true;

        points.clear();
        segments.clear();

        points.append(event->pos());

        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        points.append(event->pos());

        update();

        // update title

        setWindowTitle(QString::fromLatin1("simplify-qt: %1 points").arg(points.count()));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;

        points.append(event->pos());

        QElapsedTimer timer;
        timer.start();

        segments = SimplifyQt::simplifyIs(points);
        qint64 nsecs = timer.nsecsElapsed();
        int c = points.count();

        update();

        // update title

        qint64 ms = nsecs / 1000000;
        qint64 us = nsecs % 1000000 / 1000;

        QString text = QString::fromLatin1("simplify-qt: %1 points, %2 segments, %3.%4 ms")
                .arg(c).arg(segments.count()).arg(ms).arg(us, 3, 10, QLatin1Char('0'));

        setWindowTitle(text);
    }
}
