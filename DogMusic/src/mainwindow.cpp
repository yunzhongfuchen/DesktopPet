#include "../lib/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0,0,1020,675);
    //ȥ���Դ��߿�
    this->setWindowFlags(Qt::FramelessWindowHint);

    //���ô���͸��
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    ////�����ޱ߿�
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ////ʵ����Ӱshadow
    //QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    ////������Ӱ����
    //shadow->setOffset(0, 0);
    ////������Ӱ��ɫ
    //shadow->setColor(QColor("#444444"));
    ////������ӰԲ��
    //shadow->setBlurRadius(30);
    ////��Ƕ��QWidget������Ӱ
    //this->setGraphicsEffect(shadow);
    ////����ֱ���������ñ߾�(�˲�����Ҫ, ���ÿ��Ϊ��Ӱ�Ŀ��)
    //this->setContentsMargins(24,24,24,24);


    mainWidget = new QWidget(this);
    mainWidget->setMinimumSize(1020,675);
    mainWidget->setObjectName("main_Widget");
    mainWidget->installEventFilter(this);
    //mainWidget->setAttribute(Qt::WA_StyledBackground);

    mainVBoxLayout = new QVBoxLayout();

    //�����������е�����
    main_naviWidget = new main_navigation();

    //�������м��б����
    main_liWidget = new main_listwidget(mainWidget);

    //�����������в�������ʼ��
    playWidget = new mainPlay();

    mainVBoxLayout->addWidget(main_naviWidget);
    mainVBoxLayout->addWidget(main_liWidget);
    mainVBoxLayout->addWidget(playWidget);
    //ȥ��layout�Ŀ�϶
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

static QPoint last(0,0);        //��������

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //��ȡtitle widget �ĸ߶ȣ�����̶�Ϊ60px
    int titleHeight = this->main_naviWidget->height();
    if(event->y() <titleHeight)
    {
        last = event->globalPos(); //��ȡ����ѹ��λ��
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
    //��ȡ���ֲ����ӿؼ�
    QWidget *targetWidget = mainWidget->findChild<QWidget *>("music_playWidget");
    if (!targetWidget->isHidden())
    {
        //�����ֲ��Ž���û�б����أ������������һ������
        targetWidget->resize(main_liWidget->width(),
                            main_liWidget->height());
    }
}
