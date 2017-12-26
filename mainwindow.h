#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QPoint>
#include <QVector>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

namespace Ui {
class MainWindow;
}

class GraphicsParams {
public:
    QPen Pen;
    QBrush Brush;
    int basis_x;
    int basis_y;
};

class GraphicsData {
public:
    QVector <QPoint> Points;

    GraphicsData();
    void Add(QPoint point);
    void Delete(QPoint point);

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int LastWidth;
    int LastHeight;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int TransformToAbsoluteX(qreal x);
    int TransformToAbsoluteY(qreal y);

    qreal TransformToRelativeX(int x);
    qreal TransformToRelativeY(int y);

    int GetCurrWidth(void);
    int GetCurrHeight(void);
    void DrawBasis(void);
    void DrawGraph(void);
    void DrawPoint(QPoint pnt);
    void DrawPointsFromData(void);

    void Configure(void);

    //void mousePressEvent(QGraphicsSceneMouseEvent *event);


public slots:
    void ResizeCheck(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QTimer ResizeTimer;

    GraphicsParams GraphParams;
    GraphicsData GraphData;
};

#endif // MAINWINDOW_H
