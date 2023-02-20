#include "../lib/main_navigation.h"


main_navigation::main_navigation(QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("navigationWidget");
    this->setMinimumSize(1020,70);
    this->setMaximumHeight(70);
    this->setSizePolicy(QSizePolicy::Expanding,
                                        QSizePolicy::Expanding);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_StyledBackground,true);

    LoadStyleSheet();

    Navigation_Layout();

    ConnectInit();
}

void main_navigation::LoadStyleSheet(void)
{
    QFile file(":/style.qss");

    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void main_navigation::Navigation_Layout(void)
{
    for(int i=0; i<4; i++)
    {
        navLabel[i] = new QLabel();
    }

    NetEasyLogo = new QPushButton();
    //logo��ǩ
    NetEasyLogo->setFixedSize(145,40);
    NetEasyLogo->setObjectName("logo_Button");
    //�л������ʽ
    NetEasyLogo->setCursor(Qt::PointingHandCursor);

    for(int i=0;i<14;i++)
    {
        navButton[i] = new QPushButton();
    }

    //������˰�ť
    navButton[0]->setFixedSize(24,24);
    navButton[0]->setObjectName("titlebackButton");
    //����ǰ����ť
    navButton[1]->setFixedSize(24,24);
    navButton[1]->setObjectName("titlefrontButton");
    //������
    searchLine = new QLineEdit();
    searchLine->setFixedSize(170,30);
    searchLine->setPlaceholderText(__QString("����"));
    searchLine->setObjectName("searchLine");
    searchLine->setMinimumHeight(30);
    //Ϊ���������һ��Сͼ��
    searchAction = new QAction(searchLine);
    searchAction->setIcon(QIcon(":/images/navigation/btn_search.png"));
    searchLine->addAction(searchAction,QLineEdit::LeadingPosition);
    //���ͼ�����¼�
    //connect(searchAction,SIGNAL(triggered(bool)),this,SLOT())
    //����ʶ��
    navButton[2]->setFixedSize(30,30);
    navButton[2]->setObjectName("listentomusicButton");

    //�û�ͷ��
    navButton[3]->setFixedSize(32,32);
    navButton[3]->setObjectName("UserAvatarButton");
    //�û���
    navButton[4]->setFixedSize(90,20);
    navButton[4]->setText(__QString("���з���"));
    navButton[4]->setObjectName("usernameButton");
    setWindowOpacity(1);
    //��ͨVIP
    /*navButton[12]->setFixedSize(50,20);
    navButton[12]->setText(__QString("��ͨVIP"));
    navButton[12]->setObjectName("vipButton");*/
    //����
    navButton[5]->setFixedSize(24,24);
    navButton[5]->setObjectName("setupButton");
    //Ƥ��
    navButton[6]->setFixedSize(26,21);
    navButton[6]->setObjectName("skinButton");
    //��Ϣ
    navButton[7]->setFixedSize(24,20);
    navButton[7]->setObjectName("messageButton");
    //��Ϣ����
    navLabel[1]->setFixedSize(20,20);
    navLabel[1]->setObjectName("messageNumLabel");
    //�����
    navLabel[2]->setFixedSize(20,20);
    navLabel[2]->setObjectName("SpacetBarLabel");

    for (int i=0;i<4;i++)
    {
        main_Button[i] = new QPushButton();
    }

    //miniģʽ
    main_Button[0]->setFixedSize(20,20);
    main_Button[0]->setObjectName("miniButton");
    //minisize��С��
    main_Button[1]->setFixedSize(20,20);
    main_Button[1]->setObjectName("minimumsizeButton");
    //maxsize���
    main_Button[2]->setFixedSize(20,20);
    main_Button[2]->setObjectName("maxsizeButton");
    //�ر�
    main_Button[3]->setFixedSize(20,20);
    main_Button[3]->setObjectName("closeButton");

    navigationLayout = new QHBoxLayout();

    //�հ�
    hSpacer = new QSpacerItem(70,10,
                              QSizePolicy::Expanding,
                              QSizePolicy::Minimum);

    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(NetEasyLogo);
    navigationLayout->addSpacing(70);
    navigationLayout->addWidget(navButton[0]);
    navigationLayout->addWidget(navButton[1]);
    navigationLayout->addWidget(searchLine);
    navigationLayout->addWidget(navButton[2]);
    navigationLayout->addSpacerItem(hSpacer);
    navigationLayout->addWidget(navButton[3]);
    navigationLayout->addWidget(navButton[4]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(navButton[12]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(navButton[5]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(navButton[6]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(navButton[7]);
    navigationLayout->addSpacing(0);
    //navigationLayout->addWidget(navLabel[1]);
    navigationLayout->addWidget(navLabel[2]);
    navigationLayout->addSpacing(0);
    navigationLayout->addWidget(main_Button[0]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(main_Button[1]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(main_Button[2]);
    navigationLayout->addSpacing(10);
    navigationLayout->addWidget(main_Button[3]);
    navigationLayout->addSpacing(20);
    navigationLayout->setAlignment(Qt::AlignHCenter);
    navigationLayout->setContentsMargins(0,0,0,0);

    this->setLayout(navigationLayout);
}

void main_navigation::ConnectInit(void)
{
    //�������س�����
    connect(searchLine,SIGNAL(editingFinished()),this,SLOT(searchclicked_slot()));
    //������icon����¼�
    connect(searchAction,SIGNAL(triggered()),this,SLOT(searchclicked_slot()));

    //��С����ť�ۺ���
    connect(this->main_Button[1],SIGNAL(clicked()),
            this,SLOT(btn_min_clicked()));
    //��󻯰�ť�ۺ���
    connect(this->main_Button[2],SIGNAL(clicked()),
            this,SLOT(btn_max_clicked()));

    //�رհ�ť�ۺ���
    connect(this->main_Button[3],SIGNAL(clicked()),
            this,SLOT(btn_close_clicked()));

    //logo����ۺ���
	connect(NetEasyLogo, SIGNAL(clicked()),
		this, SLOT(btn_logo_clicked()));
    connect(navButton[0],SIGNAL(clicked()),
            this,SLOT(btn_logo_clicked()));

}


void main_navigation::btn_logo_clicked(void)
{
    PSEventController::publish(EVENT_CHANGESTACKWIDGET,Q_ARG(int, 0));
}

void main_navigation::searchclicked_slot(void)
{
    if (searchLine->hasFocus())//QLineEdit�س��ᷢ�������źţ��²��ǻس�ʱ����һ�Σ��س���ʧȥ�����ٷ���һ���źţ����Խ��д�����ֹ��ѯ���Ρ�����
    {
        PSEventController::publish(EVENT_CHANGESTACKWIDGET,Q_ARG(int, 6));
        PSEventController::publish(EVNET_NAVIGATIONSEARCH,Q_ARG(QString,searchLine->text()));
        searchLine->clearFocus();
    }
	MusicSignals::Instance()->ChangePage(1);
}

void main_navigation::mousePressEvent(QMouseEvent *event)
{
    return QWidget::mousePressEvent(event);

}

void main_navigation::btn_close_clicked(void)
{
    this->parentWidget()->parentWidget()->close();
}

void main_navigation::btn_min_clicked(void)
{
    //��С��
    this->parentWidget()->parentWidget()->showMinimized();

}
void main_navigation::btn_max_clicked(void)
{
    //���
    if (this->parentWidget()->parentWidget()->isMaximized())
    {
        this->parentWidget()->parentWidget()->showNormal();
    }
    else
    {
        this->parentWidget()->parentWidget()->showMaximized();
    }
}


