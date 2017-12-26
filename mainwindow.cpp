#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <cmath>
#include <qmath.h>
#include <QDebug>
#include <QRect>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

const qreal PARAM = 2.;
const qreal CONST_A = -.5;
const qreal CONST_B = 30.;
const qreal STRETCH = .004;
const qreal STEP = .05;

const qreal RELATIVE_WIDTH = 200.;
const qreal RELATIVE_HEIGHT = 100.;

const int POINT_RADIUS = 3;

const int WINDOW_SPACE = 10;
const int TOP_SPACE = 10;

bool IsPointsNear(QPoint p1, QPoint p2) {
    int x_n = p1.x() - p2.x();
    int y_n = p1.y() - p2.y();
    return (qSqrt(x_n * x_n + y_n * y_n) < (qreal) POINT_RADIUS);
}

bool IsPointsNearDouble(QPoint p1, QPoint p2) {
    int x_n = p1.x() - p2.x();
    int y_n = p1.y() - p2.y();
    return (qSqrt(x_n * x_n + y_n * y_n) < (qreal) POINT_RADIUS * 2.);
}

GraphicsData::GraphicsData() {
    this->Points.resize(0);
}

void GraphicsData::Add(QPoint point) {
    bool log = true;
    for (int i = 0; i < this->Points.size(); i++) {
        if (IsPointsNearDouble(this->Points[i], point)) {
            log = false;
            break;
        }
    }
    if (log) {
        this->Points.push_back(point);
    }
}

void GraphicsData::Delete(QPoint point) {
    //int pos = -1;
    for (int i = 0; i < this->Points.size(); i++) {
        if (IsPointsNear(this->Points[i], point)) {
            this->Points.erase(this->Points.begin() + i);
            break;
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->GraphParams.SetParams = false;
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    this->Configure();
    connect(&(this->ResizeTimer), SIGNAL(timeout()), this, SLOT(ResizeCheck()));
    this->ResizeTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawBasis(void) {
    QGraphicsScene *scene = ui->graphicsView->scene();
    QPen pen(Qt::gray);
    qreal basis[2] = {this->GraphParams.basis_x, this->GraphParams.basis_y};
    QLineF line_x(0, basis[1], scene->width() - 1, basis[1]);
    QLineF line_y(basis[0], 0, basis[0], scene->height() - 1);

    scene->addLine(line_x, pen);
    scene->addLine(line_y, pen);
}

int MainWindow::GetCurrWidth(void) {
    //qDebug() << "CurrWidth = " << this->GraphParams.basis_x * 2;
    //return this->GraphParams.basis_x * 2;
    return ui->graphicsView->width();
}

int MainWindow::GetCurrHeight(void) {
    //qDebug() << "CurrHeight = " << this->GraphParams.basis_y * 2;
    //return this->GraphParams.basis_y * 2;
    return ui->graphicsView->height();
}

int MainWindow::TransformToAbsoluteX(qreal x) {
    //qDebug() << "CurrX = " << x;
    /*x += this->GraphParams.basis_x;
    qDebug() << "CurrX = " << x;
    return (int) (x * GetCurrWidth() / RELATIVE_WIDTH);*/
    x *= STRETCH * GetCurrWidth();
    return x + this->GraphParams.basis_x;
}

int MainWindow::TransformToAbsoluteY(qreal y) {
    /*y += this->GraphParams.basis_y;
    return (int) (y * GetCurrHeight() / RELATIVE_HEIGHT);*/
    //y += this->GraphParams.basis_y;
    y *= -STRETCH * GetCurrHeight();
    return y + this->GraphParams.basis_y;
}

qreal MainWindow::TransformToRelativeX(int x) {
    x -= this->GraphParams.basis_x;
    return x / STRETCH / GetCurrWidth();
}

qreal MainWindow::TransformToRelativeY(int y) {
    y -= this->GraphParams.basis_y;
    return y / -STRETCH / GetCurrHeight();
}

//For Wolfram Alpha:
//x = (3 * a * t) / (1 + t^3); y = (3 * a * t^2) / (1 + t^3)

void MainWindow::DrawGraph(void) {
    QGraphicsScene *scene = ui->graphicsView->scene();
    qreal x[2];
    qreal y[2];
    /*x[0] = 0; y[0] = 0;
    x[1] = 100; y[1] = 100;
    QLineF line(TransformToAbsoluteX(x[0]), TransformToAbsoluteY(y[0]), TransformToAbsoluteX(x[1]), TransformToAbsoluteY(y[1]));
    //QLineF line(x[0], y[0], x[1], y[1]);
    scene->addLine(line, this->GraphParams.Pen);*/
    //scene->addEllipse(TransformToAbsoluteX(0), TransformToAbsoluteY(0), 16, 16, this->GraphParams.Pen, this->GraphParams.Brush);
    //DrawPoint(QPoint(10, 10));
    GraphData.Add(QPoint(0, 0));
    GraphData.Add(QPoint(4, 4));
    GraphData.Add(QPoint(-10, 10));
    GraphData.Add(QPoint(35, 20));
    GraphData.Delete(QPoint(33, 14));
    DrawPointsFromData();
    //scene->add
}

void MainWindow::DrawPoint(QPoint pnt) {
    int x = pnt.x();
    int y = pnt.y();
    y = -y;
    QGraphicsScene *scene = ui->graphicsView->scene();
    int rect_x = x + this->GraphParams.basis_x - POINT_RADIUS;
    int rect_y = y + this->GraphParams.basis_y - POINT_RADIUS;
    int side = POINT_RADIUS * 2;

    scene->addEllipse(rect_x, rect_y, side, side, this->GraphParams.Pen, this->GraphParams.Brush);
}

void MainWindow::DrawPointsFromData(void) {
    for (int i = 0; i < this->GraphData.Points.size(); i++) {
        QPoint curr = this->GraphData.Points[i];
        DrawPoint(curr);
        //DrawPoint(QPoint(TransformToAbsoluteX(curr.x()), TransformToAbsoluteY(curr.y())));
    }
}

void MainWindow::Configure(void) {
    this->LastWidth = this->width();
    this->LastHeight = this->height();

    ui->graphicsView->setGeometry(WINDOW_SPACE, WINDOW_SPACE + TOP_SPACE, this->width() - 2 * WINDOW_SPACE, this->height() - 2 * WINDOW_SPACE - TOP_SPACE);
    /*QRect geom_tmp = (ui->Label1->geometry());
    geom_tmp.setY(this->height() - 30);
    ui->Label1->setGeometry(geom_tmp);*/
    //ui->Label1->setGeometry(ui->Label1->geo);
    QGraphicsScene *scene = ui->graphicsView->scene();
    scene->clear();
    scene->setSceneRect(0, 0, ui->graphicsView->width() - 2, ui->graphicsView->height() - 2);
    this->GraphParams.Pen.setColor(Qt::black);
    this->GraphParams.Brush = QBrush(Qt::black);
    this->GraphParams.basis_x = scene->width() / 2;
    this->GraphParams.basis_y = scene->height() / 2;
    this->DrawBasis();
    /*if (this->GraphParams.SetParams) {
        this->DrawGraph();
    }*/
    this->DrawGraph();
}
void MainWindow::ResizeCheck(void) {
    if (this->width() != this->LastWidth || this->height() != this->LastHeight) {
        this->Configure();
    }
}

//void MainWindow::mousePressEvent(QGraphicsSceneMouseEvent *event) {
void MainWindow::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Press";
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    qDebug() << "Move";
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Release";
}
