#include "../../lib/PopupWidget/Popup_playlist.h"
#include <QFile>



Pop_playlist::Pop_playlist(QWidget *parent) :
    QWidget(parent)
{
    this->hide();

    this->setGeometry(0,0,450,500);
    this->setObjectName("pop_playlist");
    //设置样式
    //this->setAttribute(Qt::WA_StyledBackground);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0,0,0,0);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup|Qt::NoDropShadowWindowHint);//去除popup自带的阴影边框
    this->setFocusPolicy(Qt::NoFocus);
    //this->installEventFilter(this);

    //设置阴影边框
    /*QGraphicsDropShadowEffect *shadow =  new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(10);
    this->setGraphicsEffect(shadow);*/
    TabWidget_Init();

    ReadListFromDB();//从数据库读取播放列表

    ConnectInit();

    QTimer::singleShot(1000,this,SLOT(addtolist_slot()));//注册时间比发布注册慢，所以延迟一会儿发布。。。。

    StyleSheetInit();
}

void Pop_playlist::TabWidget_Init(void)
{
    playTabWidget = new QTabWidget();

    main_vLayout = new QVBoxLayout();

    //播放列表
    playWidget = new QWidget();
    playlist_vLayout = new QVBoxLayout();
    playlist_vLayout->setContentsMargins(4,0,0,0);
    playlist_Init();
    playWidget->setLayout(playlist_vLayout);
    //历史记录
    /*historyWidget = new QWidget;*/

    this->playTabWidget->addTab(playWidget, __QString("播放列表"));
    /*this->playTabWidget->addTab(historyWidget, __QString("历史记录"));*/

    this->playTabWidget->setTabShape(QTabWidget::Rounded);
    //this->setTabShape(TabShape::Triangular);

    main_vLayout->addWidget(playTabWidget);
    main_vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(main_vLayout);

}

void Pop_playlist::addtolist_slot()
{
    //发布更新列表事件
    PSEventController::publish(EVENT_UPDATELIST,Q_ARG(QList <MediaObjectInfo>,mediaObjectInfo));
}

void Pop_playlist::StyleSheetInit(void)
{
    QFile file(":/qss/popqss/pop_playlist.qss");
    file.open(QIODevice::ReadOnly);

    if(file.isOpen())
    {
        QString style;
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void Pop_playlist::playlist_Init(void)
{
    for(int i=0;i<2;i++)
    {
        Playlist_Btn[i] = new QPushButton();
    }

    QHBoxLayout *list_hLayout = new QHBoxLayout();

    songnum_label = new QLabel();
    songnum_label->setText(__QString("共0首"));
    songnum_label->setObjectName("songnum_label");

    /*Playlist_Btn[0]->setText(__QString("收藏全部"));
    Playlist_Btn[0]->setObjectName("playlist_btn0");
    Playlist_Btn[0]->setIconSize(QSize(20,20));
    Playlist_Btn[0]->setIcon(QIcon(QPixmap(":/images/PopWidget/playlist/btn_collectlist.png").scaled(20,20)));
    Playlist_Btn[0]->setFixedSize(100,30);*/

    Playlist_Btn[1]->setText(__QString("清空"));
    Playlist_Btn[1]->setObjectName("playlist_btn1");
    Playlist_Btn[1]->setIconSize(QSize(18,18));
    Playlist_Btn[1]->setIcon(QIcon(QPixmap(":/images/PopWidget/playlist/btn_delete.png").scaled(18,18)));
    Playlist_Btn[1]->setFixedSize(70,30);

    //画一条竖线
    QFrame *line = new QFrame();
    line->setMaximumHeight(15);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    QPalette palette = line->palette();
    palette.setColor(QPalette::Dark,QColor(220,220,220));
    line->setPalette(palette);

    list_hLayout->addWidget(songnum_label);
    list_hLayout->addSpacing(100);
    //list_hLayout->addWidget(Playlist_Btn[0]);
    list_hLayout->addWidget(line);   
    list_hLayout->addWidget(Playlist_Btn[1]);
    list_hLayout->addSpacing(20);

    playlist_table = new QTableWidget();
    playlist_table->setObjectName("playlist_table");
    playlist_table->setMinimumSize(450,300);
    playlist_table->setMaximumWidth(450);
    playlist_table->setColumnCount(4);


    //不显示表头
    playlist_table->horizontalHeader()->setVisible(false);
    playlist_table->verticalHeader()->setVisible(false);
    //设置行颜色间替
    playlist_table->setAlternatingRowColors(true);
    //去除网格线
    playlist_table->setShowGrid(false);
    //去除边框线
    //playlist_table->setFrameShape(QFrame::NoFrame);
    //去除选中虚线框
    playlist_table->setFocusPolicy(Qt::NoFocus);
    //设置列宽
    playlist_table->horizontalHeader()->setMinimumSectionSize(10);
    playlist_table->setColumnWidth(0,20);
    playlist_table->setColumnWidth(1,230);
    playlist_table->setColumnWidth(2,100);
    playlist_table->setColumnWidth(3,100);

    //设置选择行为
    playlist_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置内容不可编辑
    playlist_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置最后一列自适应
    playlist_table->horizontalHeader()->setStretchLastSection(true);
    //设置一次只能选择一行
    playlist_table->setSelectionMode(QAbstractItemView::SingleSelection);
    //设置右键按钮
    playlist_table->setContextMenuPolicy(Qt::CustomContextMenu);

    playlist_vLayout->addSpacing(7);
    playlist_vLayout->addLayout(list_hLayout);
    playlist_vLayout->addSpacing(5);
    playlist_vLayout->addWidget(playlist_table);
}

void Pop_playlist::ConnectInit()
{
    QWidget *findWnd = nullptr;
    QWidgetList allWidgetlist = QApplication::allWidgets();
    for(QWidget *pwnd : allWidgetlist)
    {
        if(pwnd->objectName() == "localmusic")
        {
            findWnd = pwnd;
            break;
        }
    }

    //播放全部按钮
    connect(findWnd,SIGNAL(addall_signal( QList <MediaObjectInfo> *)),
            this,SLOT(Rev_ListMsg_slot( QList <MediaObjectInfo> *)));
	connect(MusicSignals::Instance(), &MusicSignals::signal_PlayAllSheetSong,
		this, &Pop_playlist::Rev_ListMsg_slot);
    //清空按钮
    connect(Playlist_Btn[1],SIGNAL(clicked()),
            this,SLOT(btn_deleteclick_slot()));
	connect(MusicSignals::Instance(), &MusicSignals::cleanPlaySheet,
		this, &Pop_playlist::btn_deleteclick_slot);
	//	更新列表
	connect(MusicSignals::Instance(), &MusicSignals::signal_UpdatePlayList,
		this, [=](QList <MediaObjectInfo> list) {
		Rev_ListMsg_slot(&list);
	});

    //ViewComments = new QAction("查看评论");      //查看评论
    Play = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"),__QString("播放"));              //播放
    //Album = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"),"专辑");             //专辑
    //Singer = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"),"歌手");            //歌手
    //Comefrom = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"),"来自");          //自来
    //CollectToList = new QAction(QIcon(":/images/stackWidget/localMusic/btn_collectlist.png"),"收藏到歌单(Ctrl+S)  ");     //收藏到歌单
    //Share = new QAction(QIcon(":/images/stackWidget/localMusic/btn_share.png"),"分享...");             //分享
    //CoppLink = new QAction(QIcon(":/images/stackWidget/localMusic/btn_coppy.png"),"复制链接");          //复制链接
    //OpenFolder = new QAction(QIcon(":/images/stackWidget/localMusic/btn_folder.png"),"下载");                      //打开文件所在目录
    DeleteFromList = new QAction(QIcon(":/images/stackWidget/localMusic/btn_delete.png"), __QString("从列表删除"));    //从目录中删除

    //tableWidget右键菜单栏
    connect(playlist_table,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(tableClickedMenu_slot(QPoint)));
    connect(Play,SIGNAL(triggered()),this,SLOT(PlayClicked_slot()));
    connect(DeleteFromList,SIGNAL(triggered()),this,SLOT(ListDeleteClicked_slot()));
}

void Pop_playlist::ListDeleteClicked_slot(void)
{
    if (playlist_table->currentIndex().isValid())
    {
        int index = playlist_table->currentIndex().row();

        PSEventController::publish(EVENT_DELETEAPPOINT_SONG,Q_ARG(int, index));

        playlist_table->removeRow(index);
		mediaObjectInfo.removeAt(index);
    }
}

void Pop_playlist::PlayClicked_slot(void)
{
    if (playlist_table->currentIndex().isValid())
    {
        int index = playlist_table->currentIndex().row();

        PSEventController::publish(EVENT_PLAYAPPOINT_SONG,Q_ARG(int,index));
    }
}

void Pop_playlist::tableClickedMenu_slot(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
	menu->setMinimumWidth(150);
    //menu->addAction(ViewComments);
    menu->addAction(Play);
    /*menu->addSeparator();
    menu->addAction(Album);
    menu->addAction(Singer);
    menu->addAction(Comefrom);
    menu->addSeparator();
    menu->addAction(CollectToList);
    menu->addAction(Share);
    menu->addAction(CoppLink);
    menu->addAction(OpenFolder);*/
    menu->addSeparator();
    menu->addAction(DeleteFromList);

    //QMenu *childMenu = new QMenu(this);
    //CollectToList->setMenu(childMenu);

    menu->exec(QCursor::pos());

}

void Pop_playlist::btn_deleteclick_slot(void)
{
    mediaObjectInfo.clear();
    playlist_table->clear();
    playlist_table->setRowCount(0);
    songnum_label->setText(__QString("共")+QString::number(mediaObjectInfo.count())+ __QString("首"));

    PSEventController::publish(EVENT_CLEARLIST);
}

void Pop_playlist::AddList(QList <MediaObjectInfo> *info)
{
    mediaObjectInfo.clear();
    mediaObjectInfo = *info;
    playlist_table->setRowCount(info->count());

    for (int i=0; i<mediaObjectInfo.count(); i++)
    {
        QTableWidgetItem *Item0 = new QTableWidgetItem("");
        playlist_table->setItem(i,0,Item0);

        QTableWidgetItem *Item1 = new QTableWidgetItem(mediaObjectInfo.at(i).fileName);
        playlist_table->setItem(i,1,Item1);

        QTableWidgetItem *Item2 = new QTableWidgetItem(mediaObjectInfo.at(i).singer);
        Item2->setTextColor(QColor(100,100,100));
        playlist_table->setItem(i,2,Item2);

        QTableWidgetItem *Item3 = new QTableWidgetItem(mediaObjectInfo.at(i).playtime);
        Item3->setTextColor(QColor(150,150,150));
        playlist_table->setItem(i,3,Item3);
    }
    songnum_label->setText(__QString("共")+QString::number(mediaObjectInfo.count())+ __QString("首"));
}

void Pop_playlist::UpdateList(void)
{
    playlist_table->clearContents();
    playlist_table->setRowCount(mediaObjectInfo.count());

    for (int i=0; i<mediaObjectInfo.count(); i++)
    {
        QTableWidgetItem *Item0 = new QTableWidgetItem("");
        playlist_table->setItem(i,0,Item0);

        QTableWidgetItem *Item1 = new QTableWidgetItem(mediaObjectInfo.at(i).fileName);
        playlist_table->setItem(i,1,Item1);

        QTableWidgetItem *Item2 = new QTableWidgetItem(mediaObjectInfo.at(i).singer);
        Item2->setTextColor(QColor(100,100,100));
        playlist_table->setItem(i,2,Item2);

        QTableWidgetItem *Item3 = new QTableWidgetItem(mediaObjectInfo.at(i).playtime);
        Item3->setTextColor(QColor(150,150,150));
        playlist_table->setItem(i,3,Item3);
    }
    songnum_label->setText(__QString("共")+QString::number(mediaObjectInfo.count())+ __QString("首"));
}

void Pop_playlist::AddSong(MediaObjectInfo info)
{

}

void Pop_playlist::addNextSong(MediaObjectInfo info,int position)
{
    mediaObjectInfo.insert(position,info);
    UpdateList();
}

void Pop_playlist::Rev_ListMsg_slot( QList <MediaObjectInfo> *info)
{
    AddList(info);
    AddAllListtoDB();
    //发布更新列表事件
    PSEventController::publish(EVENT_UPDATELIST,Q_ARG(QList <MediaObjectInfo>,mediaObjectInfo));
}

int SHADOW_WIDTH = 4;

void Pop_playlist::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(QRect(SHADOW_WIDTH,0,this->width(),this->height()),QBrush(Qt::white));
    QColor color(230,230,230,20);
    for (int i=0; i<SHADOW_WIDTH;i++)
    {
        color.setAlpha(120- qSqrt(i)*40);
        painter.setPen(color);

        //painter.drawRoundedRect(SHADOW_WIDTH+1-i,0,(i),this->height(),4,4);
        painter.drawLine(SHADOW_WIDTH-i,0,1,this->height());
    }

    painter.fillRect(QRect(SHADOW_WIDTH,0,this->width(),this->height()),QBrush(Qt::white));

    painter.setPen(QPen(QColor(180,180,180),1,Qt::SolidLine));

    float x = this->playTabWidget->tabBar()->geometry().x()-0.5;
    float y = this->playTabWidget->tabBar()->geometry().y()+15-0.5;    //padding-top:15
    float width = this->playTabWidget->tabBar()->width()+1;
    float height = this->playTabWidget->tabBar()->height()-25+1;     //padding-top:15 padding-bottom:10

    const qreal radius = 16;
    QRectF rect = QRectF(x,y,width,height);

    QPainterPath path;
    path.moveTo(rect.bottomRight() - QPointF(0, radius));
    path.lineTo(rect.topRight() + QPointF(0, radius));
    path.arcTo(QRectF(QPointF(rect.topRight() - QPointF(radius * 2, 0)), QSize(radius * 2, radius *2)), 0, 90);
    path.lineTo(rect.topLeft() + QPointF(radius, 0));
    path.arcTo(QRectF(QPointF(rect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
    path.lineTo(rect.bottomLeft() - QPointF(0, radius));
    path.arcTo(QRectF(QPointF(rect.bottomLeft() - QPointF(0, radius * 2)), QSize(radius * 2, radius * 2)), 180, 90);
    path.lineTo(rect.bottomLeft() + QPointF(radius, 0));
    path.arcTo(QRectF(QPointF(rect.bottomRight() - QPointF(radius * 2, radius * 2)), QSize(radius * 2, radius * 2)), 270, 90);

    //painter.fillPath(path, QColor(Qt::gray));
    painter.drawPath(path);
    //QWidget::paintEvent(event);
}

void Pop_playlist::mousePressEvent(QMouseEvent *event)
{
    if(event->pos().rx() < 0)
    {
        this->hide();
    }
    else
    {
        if (event->pos().ry() < 0)
        {
            this->hide();
        }
        if (event->pos().ry() > 520)
        {
            this->hide();
        }
    }
}

bool Pop_playlist::eventFilter(QObject*watcher,QEvent *event)
{
    if (QEvent::WindowDeactivate == event->type())
    {
        //hide();
    }
    else
    {
        return QWidget::eventFilter(watcher,event);
    }
}

void Pop_playlist::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    //this->hide();
}

void Pop_playlist::ReadListFromDB(void)
{
    QString sql = "create table if not exists playlist"
                  "(filename string not null,filepath string not null,singer string,"
                  "filesize string not null,playtime string);";
    dbManager::excute(sql, "playlist");

    sql = "select * from playlist;";
    QSqlQuery *query = dbManager::select(sql, "playlist");
    QList <MediaObjectInfo> list;

    if (query->first())
    {
        do
        {
            MediaObjectInfo info;
            QSqlRecord res = query->record();

            int num = res.indexOf("filename");
            info.fileName = res.value(num).toString();
            num = res.indexOf("filepath");
            info.filePath = res.value(num).toString();
            num = res.indexOf("singer");
            info.singer = res.value(num).toString();
            num = res.indexOf("filesize");
            info.filesize = res.value(num).toString();
            num = res.indexOf("playtime");
            info.playtime = res.value(num).toString();

            info.songSource = LOCALMUSIC;

            list.append(info);
        }while(query->next());
    }
    AddList(&list);

    delete query;
}

void Pop_playlist::AddAllListtoDB(void)
{
    QString sql = "delete from playlist;";
    dbManager::excute(sql, "playlist");

    QStringList filenamelist;
    QStringList filePathlist;
    QStringList singerlist;
    QStringList filesizelist;
    QStringList playtimelist;

    for (int i=0; i<mediaObjectInfo.count(); i++)
    {
        filenamelist << mediaObjectInfo.at(i).fileName;
        filePathlist << mediaObjectInfo.at(i).filePath;
        singerlist << mediaObjectInfo.at(i).singer;
        filesizelist << mediaObjectInfo.at(i).filesize;
        playtimelist << mediaObjectInfo.at(i).playtime;
    }

    sql = "insert into playlist (filename,filepath,singer,filesize,playtime) values(?,?,?,?,?);";
    QSqlQuery *query = dbManager::select(sql, "playlist");
    query->addBindValue(filenamelist);
    query->addBindValue(filePathlist);
    query->addBindValue(singerlist);
    query->addBindValue(filesizelist);
    query->addBindValue(playtimelist);

    query->execBatch();
    delete query;
}

