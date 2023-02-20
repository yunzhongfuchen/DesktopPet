#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../lib/mainPlayWidget.h"
#include "../lib/stackWidget/stack_foundMusic.h"
#include "../lib/main_listWidget.h"
#include "../lib/main_navigation.h"
#include <QMouseEvent>
#include <QList>
#include "../Database/dbManager.h"
#include <QGraphicsDropShadowEffect>

class mainPlay;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QVBoxLayout *mainVBoxLayout;
    QWidget *mainWidget;
    //导航栏
    main_navigation *main_naviWidget;
    //列表栏
    main_listwidget *main_liWidget;
    //歌曲播放栏
    mainPlay *playWidget;
protected:
    //鼠标按下
    void mousePressEvent(QMouseEvent* event);
    //鼠标移动
    void mouseMoveEvent(QMouseEvent* event);
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent* event);
    //大小变化
    void resizeEvent(QResizeEvent *event);
    //事件过滤
    //bool eventFilter(QObject *watched,QEvent *event);
private:
    void SlotConnectInit(void);
    //void showPaint(void);
private slots:

};

#endif // MAINWINDOW_H
