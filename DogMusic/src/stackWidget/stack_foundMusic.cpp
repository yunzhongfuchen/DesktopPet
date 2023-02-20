#include "../../lib/stackWidget/stack_foundMusic.h"

foundmusic::foundmusic(QWidget *parent) :
    QWidget(parent)
{
    list_tabWidget = new QTabWidget();
    list_tabWidget->setMinimumSize(800,530);
    list_tabWidget->setSizePolicy(
                QSizePolicy::Expanding,
                QSizePolicy::Expanding);

    TabWidgetLayout();

    QFile file(":/qss/stackqss/stack_foundmusic.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        list_tabWidget->setStyleSheet(style);
        file.close();
    }
}

void foundmusic::TabWidgetLayout(void)
{
    personal = new QWidget;
    SongSheet = new QWidget;
    anchorStation = new QWidget;
    Ranklist = new QWidget;
    Singer = new QWidget;
    LatestMusic = new QWidget;

    QFont font;
    font = list_tabWidget->font();
    font.setPointSize(17);
    list_tabWidget->setFont(font);

    list_tabWidget->addTab(personal, __QString("狗性推荐"));
    list_tabWidget->addTab(SongSheet, __QString("歌单"));
    list_tabWidget->addTab(anchorStation, __QString("狗播电台"));
    list_tabWidget->addTab(Ranklist, __QString("排狗榜"));
    list_tabWidget->addTab(Singer, __QString("狗手"));
    list_tabWidget->addTab(LatestMusic, __QString("最狗音乐"));

    PersonalLayout();
}

void foundmusic::PersonalLayout()
{
    pictureWidget = new PictureWidget();
    pictureWidget->setMinimumSize(700,250);
    pictureWidget->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Preferred);

    RecommendWidget = new QWidget();

    Recommendation();

    personal_vLayout = new QVBoxLayout();
    personal_vLayout->setAlignment(Qt::AlignCenter);
    personal_vLayout->addWidget(pictureWidget);
    personal_vLayout->addWidget(RecommendWidget);
    personal_vLayout->setContentsMargins(0,0,0,0);

    personal_scrollWidget = new QScrollArea();
    personal_scrollWidget->setLayout(personal_vLayout);
    //去除scrollarea的边框
    personal_scrollWidget->setFrameStyle(QFrame::NoFrame);

    PerLayout = new QVBoxLayout();
    PerLayout->addWidget(personal_scrollWidget);
    personal->setLayout(PerLayout);



    /*
    pictureWidget = new PictureWidget();


    PerLayout = new QVBoxLayout();
    PerLayout->addWidget(pictureWidget);
    personal->setLayout(PerLayout);*/
}

//推荐歌单布局初始化
void foundmusic::Recommendation()
{
    QPushButton *RecommendButton = new QPushButton();
    RecommendButton->setMinimumSize(150,40);
    RecommendButton->setMaximumSize(150,40);
    RecommendButton->setText(__QString("推荐歌单>"));
    RecommendButton->setObjectName("nameButton");

    QSpacerItem *RecommendSpacer = new QSpacerItem(600,30,
                                                   QSizePolicy::Expanding,
                                                   QSizePolicy::Maximum);

    QHBoxLayout *RecommendLayout = new QHBoxLayout();
    //名字
    RecommendLayout->addWidget(RecommendButton);
    RecommendLayout->addSpacerItem(RecommendSpacer);

    QGridLayout *RecommendGLayout = new QGridLayout();


    QPushButton *recommButton[10];

    for(int i=0;i<10;i++)
    {
        recommButton[i] = new QPushButton();
        QString Objectname = tr("recommedButton%1").arg(i);
        recommButton[i]->setObjectName(Objectname);
        if(i<5)
        {
            recommButton[i]->setMinimumSize(140,140);
            recommButton[i]->setMaximumSize(250,250);
            recommButton[i]->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Expanding);
        }
        else
            recommButton[i]->setMinimumSize(140,20);
    }
    recommButton[0]->setObjectName("recommedButton0");
    //recommButton[0]->setStyleSheet("border-image: url(:/images/listWidget/widget1.png);");

    recommButton[5]->setText(__QString("热歌榜"));
    recommButton[6]->setText(__QString("欧美热歌榜"));
    recommButton[7]->setText(__QString("狗音古典榜"));
    recommButton[8]->setText(__QString("UK排行榜"));
    recommButton[9]->setText(__QString("狗音电音榜"));


    //设置网格布局比例
    RecommendGLayout->setRowStretch(0,5);
    RecommendGLayout->setRowStretch(1,1);
    RecommendGLayout->setSpacing(5);

    for (int i=0;i<5;i++)
    {
        RecommendGLayout->setColumnStretch(i,1);
    }

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<5; j++)
        {
           RecommendGLayout->addWidget(recommButton[i*5+j],i,j);
        }
    }

    QVBoxLayout *recommend_vBoxLayout = new QVBoxLayout();

    recommend_vBoxLayout->addLayout(RecommendLayout);
    recommend_vBoxLayout->addLayout(RecommendGLayout);
    recommend_vBoxLayout->setContentsMargins(0,0,0,0);
    recommend_vBoxLayout->setSpacing(0);
    RecommendWidget->setLayout(recommend_vBoxLayout);

	connect(recommButton[0], &QPushButton::clicked, recommButton[5], &QPushButton::click);
	connect(recommButton[5], &QPushButton::clicked, this, [] {
		MusicSignals::Instance()->ShowTopSheet("Hotsonglist");
	});

	connect(recommButton[1], &QPushButton::clicked, recommButton[6], &QPushButton::click);
	connect(recommButton[6], &QPushButton::clicked, this, [] {
		MusicSignals::Instance()->ShowTopSheet("EuropeAndAmericaHotsonglist");
	});

	connect(recommButton[2], &QPushButton::clicked, recommButton[7], &QPushButton::click);
	connect(recommButton[7], &QPushButton::clicked, this, [] {
		MusicSignals::Instance()->ShowTopSheet("Classicalsonglist");
	});

	connect(recommButton[3], &QPushButton::clicked, recommButton[8], &QPushButton::click);
	connect(recommButton[8], &QPushButton::clicked, this, [] {
		MusicSignals::Instance()->ShowTopSheet("UKsonglist");
	});

	connect(recommButton[4], &QPushButton::clicked, recommButton[9], &QPushButton::click);
	connect(recommButton[9], &QPushButton::clicked, this, [] {
		MusicSignals::Instance()->ShowTopSheet("Electroacousticsonglist");
	});
}
