#include "../lib/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0,0,1020,675);
    //去除自带边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    //设置窗体透明
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    ////设置无边框
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ////实例阴影shadow
    //QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    ////设置阴影距离
    //shadow->setOffset(0, 0);
    ////设置阴影颜色
    //shadow->setColor(QColor("#444444"));
    ////设置阴影圆角
    //shadow->setBlurRadius(30);
    ////给嵌套QWidget设置阴影
    //this->setGraphicsEffect(shadow);
    ////给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    //this->setContentsMargins(24,24,24,24);


    mainWidget = new QWidget(this);
    mainWidget->setMinimumSize(1020,675);
    mainWidget->setObjectName("main_Widget");
    mainWidget->installEventFilter(this);
    //mainWidget->setAttribute(Qt::WA_StyledBackground);

    mainVBoxLayout = new QVBoxLayout();

    //主界面最上行导航栏
    main_naviWidget = new main_navigation();

    //主界面中间列表界面
    main_liWidget = new main_listwidget(mainWidget);

    //主界面最下行播放栏初始化
    playWidget = new mainPlay();

    mainVBoxLayout->addWidget(main_naviWidget);
    mainVBoxLayout->addWidget(main_liWidget);
    mainVBoxLayout->addWidget(playWidget);
    //去除layout的空隙
    mainVBoxLayout->setSpacing(0);
    mainVBoxLayout->setContentsMargins(0,0,0,0);

    mainWidget->setLayout(mainVBoxLayout);
    mainWidget->setAutoFillBackground(true);
    mainWidget->setContentsMargins(0,0,0,0);
    //qDebug() << "width" << mainWidget->width() << "height" << mainWidget->height();

    this->setCentralWidget(mainWidget);
    this->setObjectName("main");

    SlotConnectInit();

    this->playWidget->music_playWidget->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    //dbManager::destoryAllDBConn();
}

void MainWindow::SlotConnectInit(void)
{

}

static QPoint last(0,0);        //保存坐标

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //获取title widget 的高度，这里固定为60px
    int titleHeight = this->main_naviWidget->height();
    if(event->y() <titleHeight)
    {
        last = event->globalPos(); //获取到按压的位置
    }

    QWidget *widget = this->findChild<QWidget *>("pop_playlist");

    /*if (event->pos().rx() < (this->width() - widget->width()))
    {
        if (!widget->isHidden())
        {
            widget->hide();
        }
    }*/

    //return QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int titleHeight = this->main_naviWidget->height();
    if(event->y() <titleHeight)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        this->move(this->x()+dx,this->y()+dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int titleHeight = this->main_naviWidget->height();
    if(event->y() <titleHeight)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        this->move(this->x()+dx,this->y()+dy);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //获取音乐播放子控件
    QWidget *targetWidget = mainWidget->findChild<QWidget *>("music_playWidget");
    if (!targetWidget->isHidden())
    {
        //若音乐播放界面没有被隐藏，则跟随主界面一起缩放
        targetWidget->resize(main_liWidget->width(),
                            main_liWidget->height());
    }
}
