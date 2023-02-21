#include "../../lib/stackWidget/stack_findMusic.h"


const QString ApiOfSerachByKeyword = "http://localhost:3000/cloudsearch?keywords=%1&offset=%2";
const QString ApiOfGetUrlById = "http://localhost:3000/song/url?id=%1&br=320000";
const QString ApiOfGetLyricById = "http://localhost:3000/lyric?id=%1";


findmusic::findmusic(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,530);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setObjectName("stack_findmusic");

    findmusic_vLayout = new QVBoxLayout();
    findmusic_vLayout->setContentsMargins(0,0,0,0);


    findmusic_LayoutInit();
    this->setLayout(findmusic_vLayout);
    ConnectInit();

    LoadStyleSheet();

    searchReply = nullptr;
    getSongLinkReply = nullptr;
    getSongLyricReply = nullptr;
    Manager = new QNetworkAccessManager(this);
}

void findmusic::LoadStyleSheet(void)
{
    QFile file(":/qss/stackqss/stack_findmusic.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString Style = this->styleSheet();
        Style += QLatin1String(file.readAll());
        this->setStyleSheet(Style);
        file.close();
    }
}

void findmusic::ConnectInit(void)
{
    //���ĵ��������������¼�
    PSEventController::subscribe(this,EVNET_NAVIGATIONSEARCH);
    //�Ҽ��˵���
    /*connect(singsong_table,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(table_customContextMenuRequest(QPoint)));*/

    //QAction ��ʼ���������ۺ���
    viewComments = new QAction("�鿴����");//�鿴����
    play = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"),"����");//����
    playnext = new QAction(QIcon(":/images/stackWidget/localMusic/btn_playnext.png"),"��һ�ײ���");//��һ�ײ���
    collecttolist = new QAction(QIcon(":/images/stackWidget/localMusic/btn_collectlist.png"),"�ղص��赥(Ctrl+S)  ");//�ղص��赥
    sharemusic = new QAction(QIcon(":/images/stackWidget/localMusic/btn_share.png"),"����");//����
    coppy = new QAction(QIcon(":/images/stackWidget/localMusic/btn_coppy.png"),"��������");//��������
    download = new QAction(QIcon(":/images/listWidget/btn_download.png"),"����");//����

	connect(singsong_table, &QTableWidget::doubleClicked, this, &findmusic::playActionclicked_slot);
    connect(play,SIGNAL(triggered()),this,SLOT(playActionclicked_slot()));
    connect(playnext,SIGNAL(triggered()),this,SLOT(playnextActionclicked_slot()));
    connect(singsong_page,SIGNAL(currentPageChanged(int)),this,SLOT(PageNavigatorClicked_slot(int)));
}

void findmusic::PageNavigatorClicked_slot(int page)
{
    qDebug() << "page:" << page;
    QString url = ApiOfSerachByKeyword.arg(keywords).arg(page*30-30);
    NetWorkManager::get(url,GetKeywordsSearch_Reply,this);
}

void findmusic::playActionclicked_slot(void)
{
    if (singsong_table->currentIndex().isValid())   //��ȡ��ǰtableWidget�������
    {
        int index = singsong_table->currentIndex().row();
        qDebug() << "current_index_row" << index;
        QString id = songInfo.at(index).songId;
        qDebug() << "id:" << id;
        GetSongLinkBySongId(songInfo.at(index),id, QString::number(index,10) + "+play");
        //PSEventController::publish(EVENT_FINDMUSICPLAY,Q_ARG(MediaObjectInfo,songInfo.at(index)),Q_ARG(QString,songUrl));
    }
}

void findmusic::playnextActionclicked_slot(void)
{
    if (singsong_table->currentIndex().isValid())   //��ȡ��ǰtableWidget�������
    {
        int index = singsong_table->currentIndex().row();
        qDebug() << "current_index_row" << index;
        QString id = songInfo.at(index).songId;
        qDebug() << "id:" << id;
        GetSongLinkBySongId(songInfo.at(index),id, QString::number(index,10) + "+playnext");
        //PSEventController::publish(EVENT_FINDMUSICNEXTPLAY,Q_ARG(MediaObjectInfo,songInfo.at(index)),Q_ARG(QString,songUrl));
    }
}

void findmusic::table_customContextMenuRequest(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    //������Ӱ�߿�
    /*QGraphicsDropShadowEffect *shadow =  new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(10);
    menu->setAttribute(Qt::WA_TranslucentBackground, true);
    menu->setGraphicsEffect(shadow);*/

    /*menu->setWindowFlags(menu->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground,true);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(10);
    menu->setGraphicsEffect(shadow);
    //����ֱ���������ñ߾�(�˲�����Ҫ, ���ÿ��Ϊ��Ӱ�Ŀ��)
    menu->setContentsMargins(10,10,10,10);*/


    menu->addAction(viewComments);
    menu->addAction(play);
    menu->addAction(playnext);
    menu->addSeparator();
    menu->addAction(collecttolist);
    menu->addAction(sharemusic);
    menu->addAction(coppy);
    menu->addAction(download);

    QMenu *collectChild = new QMenu(this);
    collecttolist->setMenu(collectChild);

    QTableWidgetItem *item = singsong_table->itemAt(pos);
    if (item != NULL)
    {
        //qDebug() << "row:" << item->row();//��ǰ�к�
        menu->exec(QCursor::pos());
    }
}

void findmusic::on_psEvent_navigationSearch(QString msg)
{
    keywords = msg;
    GetSearchByKeywords(msg,0);
}

void findmusic::GetSearchByKeywords(QString keyword,int page)
{
    QString url = ApiOfSerachByKeyword.arg(keyword).arg(page);
    NetWorkManager::get(url,GetKeywordsSearch_Reply,this, QString::number(page,10));
}

void findmusic::GetKeywordsSearch_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
    findmusic *p_findmusic = reinterpret_cast<findmusic *>(pCusData);

    if (errCode == E_NetTimeOut)
    {
        qDebug() << "Reply_timeout";
    }
    else if (errCode == E_NetReplyError)
    {
        qDebug() << "Reply_error";
    }
    else if (errCode == E_NetOK)
    {
        qDebug() << "key" << msg;
        QByteArray array = bytes;
        QJsonParseError jsonError;
        QJsonDocument json = QJsonDocument::fromJson (array, &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {qDebug() << "ERROR" << jsonError.errorString ();}

        if (json.isObject ()) {
            QJsonObject obj = json.object ();

            if (obj.contains ("result")) {
                p_findmusic->songInfo.clear();
                QJsonValue result = obj.value ("result");
                QJsonObject obj_songs = result.toObject ();

                auto songcount = obj_songs["songCount"].toInt();

                QString text = __QString("�ҵ���") + QString::number(songcount,10) + __QString("�׵���");
                p_findmusic->num_label->setText(text);

                //if (p_findmusic->singsong_page->getCurrentPageCount() != songcount/30+1)

                if (msg == "0")
                {
                    p_findmusic->singsong_page->setPageCount(songcount/30+1);
                }

                result = obj_songs.value ("songs");
                QJsonArray songs_array = obj_songs["songs"].toArray ();

                for (int row = 0; row < songs_array.size (); row++)
                {
                    MediaObjectInfo info;
                    QJsonObject jobj = songs_array[row].toObject();
                    info.fileName = jobj["name"].toString ();
                    info.songId = QString::number (jobj["id"].toInt ());
                    auto time = jobj["dt"].toInt();
                    auto second = time/1000;
                    auto minute = second/60;
                    second = second%60;

                    if (minute > 9)
                        info.playtime += QString::number(minute,10);
                    else
                        info.playtime += "0" + QString::number(minute,10);
                    info.playtime += ":";
                    if (second > 9)
                        info.playtime += QString::number(second,10);
                    else
                        info.playtime += "0" + QString::number(second,10);

                    QJsonArray Artists = jobj["ar"].toArray ();
                    auto Result = p_findmusic->GetSingerInfo (Artists);
                    info.singer = Result.first;
                    //info.SingerImage = Result.second;

                    QJsonObject album = jobj["al"].toObject ();
                    info.AlbumName = album["name"].toString ();
                    info.SingerImage = album["picUrl"].toString();
                    info.songSource = NETWORKMUSIC;

                    p_findmusic->songInfo.append(info);
                }
            }
        }
        p_findmusic->AddSingInfo(&p_findmusic->songInfo,0);
    }

    p_findmusic = nullptr;
}

void findmusic::GetSongLinkBySongId(MediaObjectInfo info,QString Id, QString LinkState)
{
    QString url = ApiOfGetUrlById.arg(Id);
    NetWorkManager::get(url,GetSongId_Reply,this,LinkState);
}

void findmusic::GetSongId_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
    findmusic *p_findmusic = reinterpret_cast<findmusic *>(pCusData);

    if (errCode == E_NetTimeOut)
    {
        qDebug() << "Reply_timeout";
    }
    else if (errCode == E_NetReplyError)
    {
        qDebug() << "Reply_error";
    }
    else if (errCode == E_NetOK)
    {
        QByteArray Array = bytes;
        QJsonParseError JsonError;
        QJsonDocument json = QJsonDocument::fromJson(Array, &JsonError);

        if (JsonError.error == QJsonParseError::NoError)
        {
            QJsonObject obj = json.object();
            QJsonArray data_Array = obj["data"].toArray();

            for (int i=0; i< data_Array.size(); i++)
            {
                QJsonObject jobj = data_Array[i].toObject();
                p_findmusic->songUrl = jobj["url"].toString();
                qDebug() << "songurl:" << p_findmusic->songUrl;

                QJsonObject obj = json.object();
                QJsonArray data_Array_1 = obj["data"].toArray();

                QString songUrl;
                for (int i=0; i< data_Array_1.size(); i++)
                {
                    QJsonObject jobj = data_Array_1[i].toObject();
                    songUrl = jobj["url"].toString();
                    qDebug() << "songurl:" << songUrl;
                }

                QStringList list = msg.split("+");
                int index = list.at(0).toInt();

                MediaObjectInfo info = p_findmusic->songInfo.at(index);

                if (list.at(1) == "play")
                {
                    PSEventController::publish(EVENT_FINDMUSICPLAY,Q_ARG(MediaObjectInfo,info),Q_ARG(QString,songUrl));
                }
                else
                {
                    PSEventController::publish(EVENT_FINDMUSICNEXTPLAY,Q_ARG(MediaObjectInfo,info),Q_ARG(QString,songUrl));
                }
            }
        }
        else
        {
            qDebug() << "Link JSONERROE:" << JsonError.errorString();
        }
    }

    p_findmusic = nullptr;
}

QPair<QString, QString> findmusic::GetSingerInfo(QJsonArray array) {
    QString SingerName = "";
    QString SingerImage = "";

    for (int row = 0; row < array.size (); ++row) {
        QJsonObject jobj = array[row].toObject();
        SingerName = jobj["name"].toString ();
        SingerImage = jobj["picUrl"].toString ();
    }

    if (SingerImage == "") SingerImage = "https://p2.music.126.net/6y-UleORITEDbvrOLV0Q8A==/5639395138885805.jpg";

    auto result = qMakePair(SingerName, SingerImage);
    return result;
}

void findmusic::AddSingInfo(const QList <MediaObjectInfo> *info,int page)
{
    singsong_table->clearContents();

    int count = (info->count()) > 99 ? 100 : info->count();

    singsong_table->setRowCount(count);
    for (int i=0; i< count;i++)
    {
        QString num = "";
        if (i<9)
            num += "0" + QString::number(i+1,10);
        else
            num += QString::number(i+1,10);

        QTableWidgetItem *item0 = new QTableWidgetItem(num);
        singsong_table->setItem(i,0,item0);
        QTableWidgetItem *item1 = new QTableWidgetItem(info->at(i).fileName);
        singsong_table->setItem(i,1,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(info->at(i).singer);
        singsong_table->setItem(i,2,item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(info->at(i).AlbumName);
        singsong_table->setItem(i,3,item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(info->at(i).playtime);
        singsong_table->setItem(i,4,item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(info->at(i).hotnum);
        singsong_table->setItem(i,5,item5);
    }
}

void findmusic::findmusic_LayoutInit(void)
{
    num_label = new QLabel();
    num_label->setObjectName("numlabel");
    num_label->setText(__QString("�����Ϸ�����������ؼ���"));

    TabWidget_Init();

    findmusic_vLayout->addWidget(num_label);
    findmusic_vLayout->addWidget(findmusic_tab);
}

void findmusic::TabWidget_Init(void)
{
    findmusic_tab = new QTabWidget();
    findmusic_tab->setObjectName("SingleSongTabwidget");
    findmusic_tab->tabBar()->setObjectName("SingleSongTabBar");

    QFont font = findmusic_tab->font();
    font.setPointSize(15);
    findmusic_tab->setFont(font);

    singSong = new QWidget();
    //singer = new QWidget();
    findmusic_tab->addTab(singSong, __QString("����"));
    //findmusic_tab->addTab(singer,__QString("����"));

    SingSong_LayoutInit();
}

void findmusic::SingSong_LayoutInit(void)
{
    songsong_vLayout = new QVBoxLayout();

    singsong_table = new QTableWidget();
    singsong_table->setMinimumSize(750,300);
    singsong_table->setObjectName("singsong_tableWidget");
    singsong_table->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);

	QStringList list = __QString(",���ֱ���,����,ר��,ʱ��,�ȶ�").split(",");
    //list << "" << "���ֱ���" << "����" << "ר��" << "ʱ��" << "�ȶ�";
    singsong_table->setColumnCount(6);
    singsong_table->setHorizontalHeaderLabels(list);
    singsong_table->verticalHeader()->setVisible(false);
    singsong_table->setColumnWidth(0,50);
    singsong_table->setColumnWidth(1,200);
    singsong_table->setColumnWidth(2,100);
    singsong_table->setColumnWidth(3,200);
    singsong_table->setColumnWidth(4,70);
    singsong_table->setColumnWidth(5,100);
    //�����Ҽ��˵���
    singsong_table->setContextMenuPolicy(Qt::CustomContextMenu);
    //�������һ������Ӧ
    singsong_table->horizontalHeader()->setStretchLastSection(true);
    //��������ɫ����
    singsong_table->setAlternatingRowColors(true);
    //����ѡ��Ϊ��
    singsong_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ɾ��������
    singsong_table->setShowGrid(false);
    //����ֻ��ѡ��һ��
    singsong_table->setSelectionMode(QAbstractItemView::SingleSelection);
    //ȥ��ѡ�����߿�
    singsong_table->setFocusPolicy(Qt::NoFocus);
    //�������ݲ����޸�
    singsong_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���ñ�ͷ����������
    singsong_table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //���õ����Ԫ��ʱ����ͷ������
    singsong_table->horizontalHeader()->setHighlightSections(false);
    //���ù����������
    singsong_table->verticalScrollBar()->setMaximumWidth(7);
    //ȥ���߿�
    singsong_table->setFrameShape(QFrame::NoFrame);

    singsong_page = new PageNavigator(singSong);

    singsong_page->initialize();
    //singsong_page->setPageCount(5);

    songsong_vLayout->addWidget(singsong_table);
    songsong_vLayout->addWidget(singsong_page);

    singSong->setLayout(songsong_vLayout);
}
