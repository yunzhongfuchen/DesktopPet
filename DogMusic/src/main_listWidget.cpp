#include "../lib/main_listWidget.h"


main_listwidget::main_listwidget(QWidget *parent) :
    QWidget(parent)
{
    main_listWidget = new QWidget();

    ReadListWidgetStyle();

    listWidgeLayout();
    ConnectInit();

    this->setLayout(list_hLayout);
    this->setMinimumSize(1020,525);
    this->setObjectName("list_mainWidget");
    this->setAttribute(Qt::WA_StyledBackground);
}

void main_listwidget::listWidgeLayout()
{
    for (int i=0; i<2; i++)
        list_ScrollLabel[i] = new QLabel();

    for (int i=0; i<3; i++)
        list_ScrollWidget[i] = new QListWidget();
    setList1();
    setList2();
    setList3();
    list_ScrollLabel[0]->setText(__QString("我的音乐"));
    list_ScrollLabel[0]->setObjectName("favor_Label");
    list_ScrollLabel[0]->setFrameStyle(QFrame::NoFrame);
    list_ScrollLabel[0]->setContentsMargins(0,0,0,0);
    list_ScrollLabel[1]->setText(__QString("创建的歌单"));
    list_ScrollLabel[1]->setObjectName("CreatePlaylist_Label");
    list_ScrollLabel[1]->setFrameStyle(QFrame::NoFrame);

    Scroll_vLayout = new QVBoxLayout();
    Scroll_vLayout->addWidget(list_ScrollWidget[0]);
    Scroll_vLayout->addWidget(list_ScrollLabel[0]);
    Scroll_vLayout->addWidget(list_ScrollWidget[1]);
    Scroll_vLayout->addWidget(list_ScrollLabel[1]);
    Scroll_vLayout->addWidget(list_ScrollWidget[2]);
    Scroll_vLayout->addStretch();
    Scroll_vLayout->setContentsMargins(0,0,0,0);

    mainScrollArea = new QScrollArea();
    mainScrollArea->setMinimumWidth(200);
    mainScrollArea->setMaximumWidth(200);
    mainScrollArea->setLayout(Scroll_vLayout);
    mainScrollArea->setContentsMargins(0,0,0,0);
    //mainScrollArea->setFrameStyle(QFrame::NoFrame);
    mainScrollArea->setObjectName("list_ScrollWidget");


    list_hSpacer = new QSpacerItem(20,540,
                                   QSizePolicy::Maximum,
                                   QSizePolicy::Expanding);
    setStackWidget();

    list_hLayout = new QHBoxLayout();
    list_hLayout->addWidget(mainScrollArea);
    list_hLayout->addSpacerItem(list_hSpacer);
    list_hLayout->addWidget(main_stackWidget);
    list_hLayout->setContentsMargins(0,0,0,0);
}


void main_listwidget::setList1(void)
{
	list_ScrollWidget[0] = new QListWidget();//导航列表
	list_ScrollWidget[0]->setMinimumSize(200, 200);
	list_ScrollWidget[0]->setContentsMargins(0, 0, 0, 0);
	list_ScrollWidget[0]->setSizePolicy(QSizePolicy::Expanding,
		QSizePolicy::Expanding);
	list_ScrollWidget[0]->setObjectName("list1Widget");
	//去除边框线
	list_ScrollWidget[0]->setFrameStyle(QFrame::NoFrame);
	//去除选中虚线框
	list_ScrollWidget[0]->setFocusPolicy(Qt::NoFocus);
	list_ScrollWidget[0]->setVerticalScrollBarPolicy(
		Qt::ScrollBarAlwaysOff);
	list_ScrollWidget[0]->setHorizontalScrollBarPolicy(
		Qt::ScrollBarAlwaysOff);
	connect(list_ScrollWidget[0], SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(on_list1Widget_itemClicked(QListWidgetItem *)));

	QListWidgetItem *foundMusic = new QListWidgetItem();
	foundMusic->setText(__QString("发现音乐"));
	foundMusic->setIcon(QIcon(":/images/listWidget/btn_mystation.png"));
	QListWidgetItem *findmusic = new QListWidgetItem();
	findmusic->setIcon(QIcon(QPixmap(":/images/listWidget/btn_cloudmusic.png").scaled(18, 18)));
	findmusic->setText(__QString("搜索音乐"));
	QListWidgetItem *video = new QListWidgetItem();
	video->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18, 17)));
	video->setText(__QString("汪"));
	QListWidgetItem *friends = new QListWidgetItem();
	friends->setText(__QString("汪"));
	friends->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18, 17)));
	QListWidgetItem *live = new QListWidgetItem();
	live->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18, 17)));
	live->setText(__QString("汪"));
	QListWidgetItem *privateFM = new QListWidgetItem();
	privateFM->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18, 17)));
	privateFM->setText(__QString("汪"));
	list_ScrollWidget[0]->addItem(foundMusic);
	list_ScrollWidget[0]->addItem(findmusic);
	list_ScrollWidget[0]->addItem(video);
	list_ScrollWidget[0]->addItem(friends);
	list_ScrollWidget[0]->addItem(live);
	list_ScrollWidget[0]->addItem(privateFM);
	//设置默认选中第一行
	list_ScrollWidget[0]->setCurrentRow(0);
}
void main_listwidget::setList2(void)
{
    list_ScrollWidget[1] = new QListWidget();
    list_ScrollWidget[1]->setMinimumSize(200,200);
    list_ScrollWidget[1]->setContentsMargins(0,0,0,0);
    list_ScrollWidget[1]->setSizePolicy(QSizePolicy::Expanding,
                                        QSizePolicy::Expanding);
    list_ScrollWidget[1]->setObjectName("list2Widget");
    list_ScrollWidget[1]->setFrameStyle(QFrame::NoFrame);
    list_ScrollWidget[1]->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    //去除选中虚线框
    list_ScrollWidget[1]->setFocusPolicy(Qt::NoFocus);
    list_ScrollWidget[1]->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    connect(list_ScrollWidget[1],SIGNAL(itemClicked(QListWidgetItem *)),
            this,SLOT(on_list2Widget_itemClicked(QListWidgetItem *)));

    QListWidgetItem *localmusic = new QListWidgetItem();
    localmusic->setIcon(QIcon(QPixmap(":/images/listWidget/btn_localmusic.png").scaled(18,17)));
    localmusic->setText(__QString("本地音乐"));
    QListWidgetItem *downloadManage = new QListWidgetItem();
    downloadManage->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18,17)));
    downloadManage->setText(__QString("汪"));
    QListWidgetItem *musicCloudDisk = new QListWidgetItem();
    musicCloudDisk->setIcon(QIcon(QPixmap(":/images/listWidget/btn_mycollect.png").scaled(18,18)));
    musicCloudDisk->setText(__QString("汪"));
    QListWidgetItem *radioStation = new QListWidgetItem();
    radioStation->setIcon(QIcon(":/images/listWidget/btn_mycollect.png"));
    radioStation->setText(__QString("汪"));
    QListWidgetItem *collection = new QListWidgetItem();
    collection->setIcon(QIcon(":/images/listWidget/btn_mycollect.png"));
    collection->setText(__QString("汪"));
    list_ScrollWidget[1]->addItem(localmusic);
    list_ScrollWidget[1]->addItem(downloadManage);
    list_ScrollWidget[1]->addItem(musicCloudDisk);
    list_ScrollWidget[1]->addItem(radioStation);
    list_ScrollWidget[1]->addItem(collection);
}
void main_listwidget::setList3(void)
{
    list_ScrollWidget[2] = new QListWidget();
    list_ScrollWidget[2]->setMinimumSize(200,40);
    list_ScrollWidget[2]->setContentsMargins(0,0,0,0);
    list_ScrollWidget[2]->setSizePolicy(QSizePolicy::Expanding,
                                        QSizePolicy::Expanding);
    list_ScrollWidget[2]->setObjectName("list3Widget");
    list_ScrollWidget[2]->setFrameStyle(QFrame::NoFrame);
    list_ScrollWidget[2]->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    list_ScrollWidget[2]->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
	connect(list_ScrollWidget[2], SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(on_list3Widget_itemClicked(QListWidgetItem *)));
    QListWidgetItem *favorMusic = new QListWidgetItem();
    favorMusic->setText(__QString("汪"));
	favorMusic->setIcon(QIcon(":/images/listWidget/btn_mycollect.png"));
    list_ScrollWidget[2]->addItem(favorMusic);

}

void main_listwidget::setStackWidget(void)
{
    main_stackWidget =new QStackedWidget(this);
    main_stackWidget->setObjectName("main_stackWidget");

    //发现音乐界面初始化
    main_stackfoud = new foundmusic();
    ////视频界面
    //main_stackvideo = new stack_video();
    ////朋友界面
    //stack_video *main_friend = new stack_video();
    ////直播界面
    //stack_video *main_live = new stack_video();
    ////私人FM
    //stack_video *main_FM = new stack_video();
	//音乐搜索界面
	main_findmusic = new findmusic();
    //本地音乐界面
    main_stacklocal = new localMusic(main_stackWidget);
	//排行榜音乐界面
	main_stacksheet = new sheetMusic(main_stackWidget);
    
	
    main_stackWidget->addWidget(main_stackfoud->list_tabWidget);
    //main_stackWidget->addWidget(main_stackvideo);
    //main_stackWidget->addWidget(main_friend);
    //main_stackWidget->addWidget(main_live);
    //main_stackWidget->addWidget(main_FM);
	main_stackWidget->addWidget(main_findmusic);
    main_stackWidget->addWidget(main_stacklocal);
	main_stackWidget->addWidget(main_stacksheet);
    


    main_stackWidget->setMinimumSize(800,525);
    main_stackWidget->setCurrentIndex(0);
}

void main_listwidget::ReadListWidgetStyle(void)
{
    QFile file(":/main_listWidget.qss");

    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void main_listwidget::on_list1Widget_itemClicked(QListWidgetItem *item)
{
    //去除另外一个关联list的选中
    list_ScrollWidget[1]->clearSelection();
	list_ScrollWidget[2]->clearSelection();
//    item->font().setBold(true);
//    QFont font;
//    font.setFamily("微软雅黑");
//    font.setPointSize(11);

//    for(int i=0;i<5;i++)
//    {
//        list_ScrollWidget[0]->item(i)->setFont(font);
//    }
//    list_ScrollWidget[0]->setFont(font);
//    //list_ScrollWidget[0]->row(item);
//    //qDebug() << "currentRow" << list_ScrollWidget[0]->currentRow();

//    font = item->font();
//    font.setBold(true);
//    font.setPointSize(font.pointSize()+3);
//    //list_ScrollWidget[0]->item(list_ScrollWidget[0]->row(item))->setFont(font);
	switch (list_ScrollWidget[0]->currentRow())
	{
	case 0:MusicSignals::Instance()->ChangePage(0);
		break;
	case 1:MusicSignals::Instance()->ChangePage(1);
		break;
	default:
		break;
	}
    //main_stackWidget->setCurrentIndex(list_ScrollWidget[0]->currentRow());
}
void main_listwidget::on_list2Widget_itemClicked(QListWidgetItem *item)
{
    //去除另外一个关联list的选中
    list_ScrollWidget[0]->clearSelection();
	list_ScrollWidget[2]->clearSelection();
	switch (list_ScrollWidget[1]->currentRow())
	{
	case 0:MusicSignals::Instance()->ChangePage(2);
		break;
	default:
		break;
	}
    //main_stackWidget->setCurrentIndex(list_ScrollWidget[1]->currentRow()+5);
}

void main_listwidget::on_list3Widget_itemClicked(QListWidgetItem *item)
{
	list_ScrollWidget[0]->clearSelection();
	list_ScrollWidget[1]->clearSelection();
}

void main_listwidget::ConnectInit(void)
{
	connect(MusicSignals::Instance(), &MusicSignals::signal_ChangePage, this, [=](int i) {
		main_stackWidget->setCurrentIndex(i);
	});
	connect(MusicSignals::Instance(), &MusicSignals::signal_ShowTopSheet, this, [=]() {
		main_stackWidget->setCurrentIndex(3);
	});
    PSEventController::subscribe(this,EVENT_CHANGESTACKWIDGET);
}

void main_listwidget::on_psEvent_changeStackWidget(int index)
{
    main_stackWidget->setCurrentIndex(index); 
    list_ScrollWidget[0]->clearSelection();
    list_ScrollWidget[1]->clearSelection();

    QWidget *findwidget = nullptr;
    QWidgetList allWidgetlist = QApplication::allWidgets();

    for(QWidget *pwd : allWidgetlist)
    {
        if (pwd->objectName() == "music_playWidget")
        {
            findwidget = pwd;
        }
    }
    if (findwidget!=nullptr)
    {
        if(!findwidget->isHidden())
            findwidget->hide();
    }
}


//大小变化
void main_listwidget::resizeEvent(QResizeEvent *event)
{


}
