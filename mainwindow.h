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
    void Clear(void);

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int LastWidth;
    int LastHeight;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int TransformToAbsoluteX(int x);
    int TransformToAbsoluteY(int y);

    //qreal TransformToRelativeX(int x);
    //qreal TransformToRelativeY(int y);

    int GetCurrWidth(void);
    int GetCurrHeight(void);
    void DrawBasis(void);
    void DrawGraph(void);
    void DrawPoint(QPoint pnt);
    void DrawPointsFromData(void);
    void DrawBesier(void);
    qreal FuncX(qreal t);
    qreal FuncY(qreal t);

    void Configure(void);

    //void mousePressEvent(QGraphicsSceneMouseEvent *event);


public slots:
    void ResizeCheck(void);

/*protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);*/

private slots:
    void on_Apply_clicked();

private:
    Ui::MainWindow *ui;
    QTimer ResizeTimer;

    GraphicsParams GraphParams;
    GraphicsData GraphData;
};

#endif // MAINWINDOW_H
