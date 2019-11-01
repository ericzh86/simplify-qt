#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QRadioButton>

#include "SimplifyQt.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected slots:
    void create2k5Points();
    void create10kPoints();
protected:
    void createPoints(int xc, int yc);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_FINAL;

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_FINAL;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_FINAL;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_FINAL;

private:
    bool pressed;
    QVector<QPointF> points;
    QVector<SimplifyQt::Segment> segments;

private:
    QRadioButton *buttonPoint;
    QRadioButton *buttonSegment;
    QRadioButton *buttonTotal;
};
#endif // MAINWINDOW_H
