#include "../../lib/stackWidget/stack_sheetMusic.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>


const QString ApiOfHotsonglist = "http://localhost:3000/playlist/detail?id=3778678";	//�ȸ��
const QString ApiOfEuropeAndAmericaHotsonglist = "http://localhost:3000/playlist/detail?id=2809513713";	//ŷ���ȸ��
const QString ApiOfClassicalsonglist = "http://localhost:3000/playlist/detail?id=71384707";	//�ŵ��
const QString ApiOfUKsonglist = "http://localhost:3000/playlist/detail?id=180106";	//UK���а�
const QString ApiOfElectroacousticsonglist = "http://localhost:3000/playlist/detail?id=1978921795";	//������
const QString ApiOfDailyRecommendedSongs = "http://localhost:3000/recommend/songs";
const QString ApiOfGetUrlById = "http://localhost:3000/song/url?id=%1&br=320000";
const QString ApiOfGetLyricById = "http://localhost:3000/lyric?id=%1";

sheetMusic::sheetMusic(QWidget *parent):
    QWidget(parent)
{
    this->setMinimumSize(800,530);
    this->setObjectName("localmusic");

    local_VBoxLayout = new QVBoxLayout();
    local_VBoxLayout->setContentsMargins(0,0,0,0);
    this->setLayout(local_VBoxLayout);
    this->setAttribute(Qt::WA_StyledBackground);

    localMusicLayout();

    LoadStyleSheet();
    //Ҫ�ȼ�������ʽ�ټ����ӿؼ�����Ϊ��������Widget�������͸������õ�ͬһ��QSS��ʽ��
    //�ӿؼ����Զ��̳и��ؼ�����ʽ�����ǵø��ؼ��ȼ�������ʽ���У�����
    TabWidgetLoad();

    ConnectInit();
    DataBaseInit();

	searchReply = nullptr;
	getSongLinkReply = nullptr;
	getSongLyricReply = nullptr;
	Manager = new QNetworkAccessManager(this);
    /*Dir_list.append("�ҵ�����");
    QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
    QDir dirbsolutePath(dir.absolutePath());
    Dir_list.append(dirbsolutePath.path());*/
}

void sheetMusic::LoadStyleSheet(void)
{
    QFile file(":/qss/stackqss/stack_localMusic.qss");
    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void sheetMusic::DataBaseInit(void)
{
    QString sql = "create table if not exists selectPath"
                  "(path STRING NOT NULL);";

    dbManager::excute(sql,"selectPath");

    QSqlQuery *query = nullptr;
    sql = QString("select * from selectPath;");
    query = dbManager::select(sql, "selectPath");

    if(query->first())
    {
        while (query->next())
        {
            //qDebug() << query->record().indexOf("path");
            QSqlRecord record = query->record();
            int FieldNo = record.indexOf("path");
            //qDebug() << record.value(FieldNo).toString();

            Dir_list.append(record.value(FieldNo).toString());
        }
    }
    else
    {
        Dir_list.append(__QString("�ҵ�����"));
        QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
        QDir dirbsolutePath(dir.absolutePath());
        Dir_list.append(dirbsolutePath.path());

        // ODBC����������벽�� 1Ԥ�������ռλ�� 2�ÿɱ��б��ռλ����ֵ 3��Ӱ�  4ִ��Ԥ����
        //Ԥ�������ռλ��
        sql = "insert into selectPath (path) values(?);";
        query = dbManager::select(sql, "selectPath");

        QStringList pathlist = this->Dir_list;

        query->addBindValue(pathlist);
        query->execBatch();

        //sql = "insert into selectPath (path) values(:path);";

        //query = dbManager::select(sql, "selectPath");

        //for (int i=0; i<Dir_list.count(); i++)
        //{
            //query->bindValue(":path",Dir_list.at(i));
            //query->bindValue(0,Dir_list.at(i));
            //query->exec();
        //}
    }

    delete query;
}

void sheetMusic::GetplayPath(void)
{
   Dir_list.clear();

    QString sql = "select * from selectPath;";

    QSqlQuery *query = nullptr;

    query = dbManager::select(sql, "selectPath");

    if (query->first())
    {
        do
        {
            Dir_list.append(query->value(0).toString());
        }
        while (query->next());
    }
    delete query;
}

void sheetMusic::SaveplayPath(void)
{
    QString sql;

    for (int i=0; i<Dir_list.count(); i++)
    {
        QSqlQuery *query = nullptr;
        sql = QString("select * from selectPath where path = '%1';").arg(Dir_list.at(i));
        query = dbManager::select(sql, "selectPath");
        if (!query->first())    //��ѯ���ݿ����Ƿ���ڸ�·�����������ڣ���������ݿ���
        {
            sql = QString("insert into selectPath (path) values('%1');").arg(Dir_list.at(i));
            dbManager::excute(sql, "selectPath");
        }
        delete query;
    }
}

void sheetMusic::localMusicLayout(void)
{
    display_HBoxLayout = new QHBoxLayout();

    displayLabel[0] = new QLabel();
    displayLabel[0]->setText(__QString("���а�����"));
    displayLabel[0]->setObjectName("localLabel");
    displayLabel[1] = new QLabel();
    displayLabel[1]->setText(__QString("��0��"));
    displayLabel[1]->setObjectName("numberlabel");

    /*selectDir = new QPushButton();
    selectDir->setText(__QString("ѡ��Ŀ¼"));
    selectDir->setObjectName("selectButton");
    selectDir->setCursor(Qt::PointingHandCursor);*/

    QSpacerItem *display_hSpacer = new QSpacerItem(200,10,
                                                   QSizePolicy::Expanding,
                                                   QSizePolicy::Expanding);

    display_HBoxLayout->addWidget(displayLabel[0]);
    display_HBoxLayout->addWidget(displayLabel[1]);
    display_HBoxLayout->addSpacerItem(display_hSpacer);
    //display_HBoxLayout->addWidget(selectDir);
    display_HBoxLayout->addSpacing(30);
}

void sheetMusic::TabWidgetLoad(void)
{
    /*QWidget *singer = new QWidget();
    QWidget *album = new QWidget();
    QWidget *folder = new QWidget();*/

    //����Tab�����ʼ��
    songInfoLayout();


    //������ϢTabҳ
    musicInfo_Widget = new QTabWidget();
    musicInfo_Widget->setObjectName("musicIndo_TabWidget");
    musicInfo_Widget->setMinimumSize(800,400);
    musicInfo_Widget->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Expanding);
    musicInfo_Widget->tabBar()->setObjectName("musicInfo_TabBar");
    musicInfo_Widget->addTab(song, __QString("����"));
    /*musicInfo_Widget->addTab(singer, __QString("����"));
    musicInfo_Widget->addTab(album, __QString("ר��"));
    musicInfo_Widget->addTab(folder, __QString("�ļ���"));*/

    local_VBoxLayout->addSpacing(20);
    local_VBoxLayout->addLayout(display_HBoxLayout);
    local_VBoxLayout->addWidget(musicInfo_Widget);
}

void sheetMusic::songInfoLayout(void)
{
    //-----------------��ť��ʼ��---------------------//
    song = new QWidget();
    QVBoxLayout *song_VLayout = new QVBoxLayout();

    playAll = new QPushButton();
    playAll->setText(__QString("����ȫ��"));
    playAll->setMaximumSize(120,35);
    playAll->setMinimumSize(120,35);
    playAll->setObjectName("playallBtn");
    playAll->setIcon(QIcon(":/images/stackWidget/localMusic/btn_playall.png"));

    /*QAction *addAllAction = new QAction(playAll);
    addAllAction->setIcon(QIcon(":/images/stackWidget/sheetMusic/btn_playalladd.png")); 
    playAll->addAction(addAllAction);*/

    /*QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText(__QString("������������"));
    searchEdit->setObjectName("localSearch");
    searchEdit->setMaximumSize(200,25);*/

    //QAction *searchAction = new QAction(searchEdit);
    //searchAction->setIcon(QIcon(":/images/stackWidget/sheetMusic/icon_search.png"));
    //����ICON���������ұ�
    //searchEdit->addAction(searchAction,QLineEdit::TrailingPosition);

    QSpacerItem *Button_HSpacer = new QSpacerItem(200,20,
                                                  QSizePolicy::Expanding,
                                                  QSizePolicy::Expanding);


    QHBoxLayout *button_HBoxLayout = new QHBoxLayout();
    button_HBoxLayout->addWidget(playAll);
    button_HBoxLayout->addSpacerItem(Button_HSpacer);
    //button_HBoxLayout->addWidget(searchEdit);

    music_Table = new QTableWidget();
    music_Table->setMinimumSize(750,350);
    /*music_Table->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);*/
    music_Table->setObjectName("table_music");
    QList <QString> tableList;
    tableList = __QString(",���ֱ���,����,ר��,ʱ��,�ȶ�").split(",");
    music_Table->setColumnCount(6);
    music_Table->setHorizontalHeaderLabels(tableList);
    music_Table->setColumnWidth(0,50);
    music_Table->setColumnWidth(1,200);
    music_Table->setColumnWidth(2,100);
    music_Table->setColumnWidth(3,250);
    music_Table->setColumnWidth(4,70);
    music_Table->setColumnWidth(5,70);
    //���Ҽ��˵�����
    music_Table->setContextMenuPolicy(Qt::CustomContextMenu);
    //���������б���ɫ
    music_Table->setAlternatingRowColors(true);
    //���ñ�����ݲ��ɱ༭
    music_Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //����Ϊ��ѡ��
    music_Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //�������һ������Ӧ����
    music_Table->horizontalHeader()->setStretchLastSection(true);
    //ɾ�������
    music_Table->setShowGrid(false);
    //ȥ���߿���
    music_Table->setFrameShape(QFrame::NoFrame);
    //ȥ��ѡ�����߿�
    music_Table->setFocusPolicy(Qt::NoFocus);
    //���õ����ͷ��������
    music_Table->setSortingEnabled(false);
    //���ñ�ͷ����������
    music_Table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //���õ����Ԫ��ʱ����ͷ�����
    music_Table->horizontalHeader()->setHighlightSections(false);
    music_Table->verticalHeader()->setHighlightSections(false);
    //����ֻ��ѡ��һ��Ŀ��
    music_Table->setSelectionMode(QAbstractItemView::SingleSelection);
    //���ô�ֱ��������С���
    music_Table->verticalScrollBar()->setMaximumWidth(7);

    music_Table->verticalHeader()->setAlternatingRowColors(true);
    music_Table->verticalHeader()->setObjectName("music_verticalHeader");
    music_Table->verticalHeader()->setDefaultSectionSize(30);
    music_Table->verticalHeader()->setVisible(false);

    song_VLayout->addLayout(button_HBoxLayout);
    song_VLayout->addWidget(music_Table);

    song->setLayout(song_VLayout);
}

void sheetMusic::table_customContextMenuRequest(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
	menu->setMinimumWidth(150);
    //QAction *ViewComments = menu->addAction(__QString("�鿴����"));
	QAction *play = new QAction(QIcon(":/images/stackWidget/localMusic/btn_play.png"), __QString("����"));
	menu->addAction(play);
	menu->addSeparator(); 
	QAction *playAll = new QAction(QIcon(":/images/stackWidget/localMusic/btn_playnext.png"), __QString("����ȫ��"));
	menu->addAction(playAll);
    /*QAction *nextPlay = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_playnext.png"), __QString("��һ�ײ���"));
    menu->addSeparator();
    QAction *Collecttolist = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_collectlist.png"), __QString("�ղص��赥"));
    QAction *sharemusic = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_share.png"), __QString("����"));
    QAction *coppy = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_coppy.png"), __QString("��������"));
    QAction *uploadcloud = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_cloudmusic.png"), __QString("�ϴ����ҵ���������"));
    QAction *opendir = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_folder.png"), __QString("���ļ�����Ŀ¼"));
    menu->addSeparator(); 
    QAction *deletefromlist = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_delete.png"), __QString("���б���ɾ��(Delete)"));
    QAction *deletefromlocal = menu->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_delete.png"), __QString("�ӱ��ش���ɾ��"));

    QMenu *collectChild = new QMenu(this);
    Collecttolist->setMenu(collectChild);

    QAction *creatnewSheet = collectChild->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_add.png"), __QString("�����¸赥"));
    QAction *myfavor = collectChild->addAction(QIcon(":/images/stackWidget/sheetMusic/btn_add.png"), __QString("��ϲ��������"));*/


    //menu.addAction("",this,SLOT());
	/*connect(nextPlay, &QAction::triggered, this, [=]() {
		if (music_Table->currentIndex().isValid())
		{
			int index = music_Table->currentIndex().row();
			qDebug() << "current_index_row" << index;
			QString id = mediaObjectInfo.at(index).songId;
			qDebug() << "id:" << id;
			QString url = ApiOfGetUrlById.arg(id);
			NetWorkManager::get(url, GetAllSongId_Reply, this, QString::number(-1 - index));
		}
	});*/
	connect(play, &QAction::triggered, this, &sheetMusic::playOneSong);
	connect(playAll, &QAction::triggered, this, &sheetMusic::PlayAll);
    menu->exec(QCursor::pos());

}

void sheetMusic::ConnectInit(void)
{
    //tablelist�����Ҽ������˵���
    connect(music_Table,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(table_customContextMenuRequest(QPoint)));
    //////ѡ��Ŀ¼
    //connect(selectDir,SIGNAL(clicked()),
    //        this,SLOT(btn_selectDirSlot()));

    //����ȫ����ť
    /*connect(playAll,SIGNAL(clicked()),
            this,SLOT(btn_addlistclicked_slot()));*/

    connect(playAll, &QPushButton::clicked,
            this,&sheetMusic::PlayAll);
	connect(music_Table, &QTableWidget::doubleClicked, this, &sheetMusic::playOneSong);

	connect(MusicSignals::Instance(), &MusicSignals::signal_ShowTopSheet, this, &sheetMusic::GetSheet);
}

void sheetMusic::GetSheet(QString sheetName)
{
	music_Table->clearContents();
	QString url;
	if ("Hotsonglist" == sheetName)
	{
		displayLabel[0]->setText(__QString("�ȸ��"));
		url = ApiOfHotsonglist;
	}
	else if ("EuropeAndAmericaHotsonglist" == sheetName)
	{
		displayLabel[0]->setText(__QString("ŷ���ȸ��"));
		url = ApiOfEuropeAndAmericaHotsonglist;
	}
	else if ("Classicalsonglist" == sheetName)
	{
		displayLabel[0]->setText(__QString("�����ŵ��"));
		url = ApiOfClassicalsonglist;
	}
	else if ("UKsonglist" == sheetName)
	{
		displayLabel[0]->setText(__QString("UK���а�"));
		url = ApiOfUKsonglist;
	}
	else if ("Electroacousticsonglist" == sheetName)
	{
		displayLabel[0]->setText(__QString("����������"));
		url = ApiOfElectroacousticsonglist;
	}


	NetWorkManager::get(url, GetSheet_Reply, this, sheetName);
}

QPair<QString, QString> GetSingerInfo(QJsonArray array) {
	QString SingerName = "";
	QString SingerImage = "";

	for (int row = 0; row < array.size(); ++row) {
		QJsonObject jobj = array[row].toObject();
		SingerName = jobj["name"].toString();
		SingerImage = jobj["picUrl"].toString();
	}

	if (SingerImage == "") SingerImage = "https://p2.music.126.net/6y-UleORITEDbvrOLV0Q8A==/5639395138885805.jpg";

	auto result = qMakePair(SingerName, SingerImage);
	return result;
}

void sheetMusic::GetSheet_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
	sheetMusic* p_sheetMusic = reinterpret_cast<sheetMusic *>(pCusData);	//Ϊ�˺�����÷Ǿ�̬����
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
		QStringList theKey;
		/*if (msg == "*�ȸ��*" || msg == "*ŷ���ȸ��*")
		{*/
			theKey << "playlist" << "tracks";
		/*}*/
		//else
		//{
		//	theKey << "result" << "songs";
		//}
		qDebug() << "key" << msg;
		QByteArray array = bytes;
		QJsonParseError jsonError;
		QJsonDocument json = QJsonDocument::fromJson(array, &jsonError);

		if (jsonError.error != QJsonParseError::NoError) { qDebug() << "ERROR" << jsonError.errorString(); }

		if (json.isObject()) 
{
			QJsonObject obj = json.object();

			if (obj.contains(theKey[0])) {
				p_sheetMusic->mediaObjectInfo.clear();
				QJsonValue result = obj.value(theKey[0]);
				QJsonObject obj_songs = result.toObject();

				auto songcount = obj_songs["songCount"].toInt();

				result = obj_songs.value(theKey[1]);
				QJsonArray songs_array = obj_songs[theKey[1]].toArray();
				int songname = 0;
				if (theKey[0] == "result")  songname = obj_songs["songCount"].toInt();
				else songname = songs_array.size();

				QString text = "�ҵ���" + QString::number(songname, 10) + "�׵���";
				qDebug() << text;
				for (int row = 0; row < songs_array.size(); row++)
				{
					MediaObjectInfo info;
					QJsonObject jobj = songs_array[row].toObject();
					info.fileName = jobj["name"].toString();
					info.songId = QString::number(jobj["id"].toInt());
					auto time = jobj["dt"].toInt();
					auto second = time / 1000;
					auto minute = second / 60;
					second = second % 60;

					if (minute > 9)
						info.playtime += QString::number(minute, 10);
					else
						info.playtime += "0" + QString::number(minute, 10);
					info.playtime += ":";
					if (second > 9)
						info.playtime += QString::number(second, 10);
					else
						info.playtime += "0" + QString::number(second, 10);

					QJsonArray Artists = jobj["ar"].toArray();
					auto Result = GetSingerInfo(Artists);
					info.singer = Result.first;
					//info.SingerImage = Result.second;

					QJsonObject album = jobj["al"].toObject();
					info.AlbumName = album["name"].toString();
					info.SingerImage = album["picUrl"].toString();
					info.songSource = NETWORKMUSIC;

					p_sheetMusic->mediaObjectInfo.append(info);
				}
			}
		}
		p_sheetMusic->AddSingInfo();
	}
	
}

void sheetMusic::AddSingInfo()
{
	music_Table->clearContents();

	//int count = (mediaObjectInfo.count()) > 200 ? 200 : mediaObjectInfo.count();
	int count = mediaObjectInfo.count();
	displayLabel[1]->setText(__QString("��") + QString::number(count) + __QString("��"));
	music_Table->setRowCount(count);
	for (int i = 0; i < count; i++)
	{
		QString num = "";
		if (i < 9)
			num += "0" + QString::number(i + 1, 10);
		else
			num += QString::number(i + 1, 10);

		QTableWidgetItem *item0 = new QTableWidgetItem(num);
		music_Table->setItem(i, 0, item0);
		QTableWidgetItem *item1 = new QTableWidgetItem(mediaObjectInfo.at(i).fileName);
		music_Table->setItem(i, 1, item1);
		QTableWidgetItem *item2 = new QTableWidgetItem(mediaObjectInfo.at(i).singer);
		music_Table->setItem(i, 2, item2);
		QTableWidgetItem *item3 = new QTableWidgetItem(mediaObjectInfo.at(i).AlbumName);
		music_Table->setItem(i, 3, item3);
		QTableWidgetItem *item4 = new QTableWidgetItem(mediaObjectInfo.at(i).playtime);
		music_Table->setItem(i, 4, item4);
		QTableWidgetItem *item5 = new QTableWidgetItem(mediaObjectInfo.at(i).hotnum);
		music_Table->setItem(i, 5, item5);
	}
}

void sheetMusic::playOneSong()
{
	if (music_Table->currentIndex().isValid())   //��ȡ��ǰtableWidget�������
	{
		int index = music_Table->currentIndex().row();
		qDebug() << "current_index_row" << index;
		QString id = mediaObjectInfo.at(index).songId;
		qDebug() << "id:" << id;
		QString url = ApiOfGetUrlById.arg(id);
		NetWorkManager::get(url, GetAllSongId_Reply, this, QString::number(index));
	}
}
void sheetMusic::PlayAll()
{
	MusicSignals::Instance()->cleanPlaySheet();
	QString songIds = QString();
	int num = 0;
	for (int i = 0; i < mediaObjectInfo.size(); ++i)
	{
		songIds = songIds + "," + mediaObjectInfo.at(i).songId;
		++num;
	}
	songIds = songIds.right(songIds.size() - 1);    //ȥ������ߵ�,
	qDebug() << songIds;
	QString url = ApiOfGetUrlById.arg(songIds);
	NetWorkManager::get(url, GetAllSongId_Reply, this, "all");
}

void sheetMusic::GetAllSongId_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg)
{
	sheetMusic *p_sheetMusic = reinterpret_cast<sheetMusic *>(pCusData);

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
			//int index = -1;
			QMap<int, QString> songUrlMp = QMap<int, QString>();
			for (int i = 0; i < data_Array.size(); i++)
			{
				QJsonObject jobj = data_Array[i].toObject();
				//sheetMusic->songUrl = jobj["url"].toString();
				//qDebug() << "songurl1:" << p_sheetMusic->songUrl;
				int songid = jobj["id"].toInt();
				QString songUrl = jobj["url"].toString();
				qDebug()<<"id: "<< songid << "songurl:" << songUrl;
				QStringList list = msg.split("+");
				//index++;
				songUrlMp.insert(songid, songUrl);
				//p_sheetMusic->mediaObjectInfo[index].filePath = songUrl;
			}
			if (msg == "all")
			{
				for (int i = 0; i < songUrlMp.size(); i++)
				{
					p_sheetMusic->mediaObjectInfo[i].filePath = songUrlMp[p_sheetMusic->mediaObjectInfo[i].songId.toInt()];
				}
				MusicSignals::Instance()->PlayAllSheetSong(&(p_sheetMusic->mediaObjectInfo));
			}
			else if(msg.toInt() >= 0)
			{
				p_sheetMusic->mediaObjectInfo[msg.toInt()].filePath = songUrlMp[p_sheetMusic->mediaObjectInfo[msg.toInt()].songId.toInt()];
				MusicSignals::Instance()->PlayOneSheetSong(p_sheetMusic->mediaObjectInfo[msg.toInt()]);
			}
			else
			{
				int index = 0 - msg.toInt() - 1;
				p_sheetMusic->mediaObjectInfo[index].filePath = songUrlMp[p_sheetMusic->mediaObjectInfo[index].songId.toInt()];
				MusicSignals::Instance()->SetNextSheetSong(p_sheetMusic->mediaObjectInfo[index]);
			}
		}
		else
		{
			qDebug() << "Link JSONERROE:" << JsonError.errorString();
		}
	}

	p_sheetMusic = nullptr;
}
//
//void sheetMusic::btn_deleteclicked_slot(void)
//{
//    mediaObjectInfo.clear();
//    music_Table->clearContents();
//    music_Table->setRowCount(0);
//
//	displayLabel[1]->setText(__QString("��") + QString::number(mediaObjectInfo.count(), 10) + __QString("��"));
//}

//void sheetMusic::btn_addlistclicked_slot(void)
//{
//    emit addall_signal(&this->mediaObjectInfo);
//}

//void sheetMusic::btn_selectDirSlot(void)
//{
//    //��ȡ����λ��x,y
//    int x = this->parentWidget()->parentWidget()->parentWidget()->mapToGlobal(QPoint(0,0)).rx() +
//            parentWidget()->parentWidget()->parentWidget()->width()/2 -
//            450/2;
//    int y = this->parentWidget()->parentWidget()->parentWidget()->mapToGlobal(QPoint(0,0)).ry() +
//            parentWidget()->parentWidget()->parentWidget()->height()/2 -
//            300/2;
//    GetplayPath();
//
//    //selectDia *selectDia = new class selectDia(this);
//    selectDia *selectDia = new class selectDia(this,Dir_list);
//    selectDia->move(x,y);
//    //���ùرմ���ʱ�Զ�����
//    //selectDia->setAttribute(Qt::WA_DeleteOnClose);
//
//    //��ģ̬��ʽ��ʾ�Ի���
//    int ret = selectDia->exec();
//
//    int count = 0;
//
//    if(ret == QDialog::Accepted)
//    {
//        Dir_list = selectDia->Dir_List;
//
//        SaveplayPath();
//        foreach (QString str, selectDia->Dir_List)
//        {
//            //�����Ŀ¼��ѡ��
//            if(selectDia->listPath->item(count)->checkState() == Qt::Checked)
//            {
//                QDir dir(str);
//                if (dir.exists())
//                {
//                    //���������
//                    QStringList filter;
//                    //��������.mp3��׺���ļ�
//                    filter << "*.mp3";
//                    //��ȡ��Ŀ¼�µ������ļ�
//                    QFileInfoList files = dir.entryInfoList(filter,QDir::Files);
//
//                    for(int i=0; i< files.count(); i++)
//                    {
//                        QString songname = QString::fromUtf8(files.at(i).fileName().replace(".mp3","").toUtf8().data());
//
//                        bool result = false;
//                        foreach(MediaObjectInfo search,mediaObjectInfo)
//                        {
//                            if(search.filePath == files.at(i).filePath())
//                            {
//                                result = true;
//                            }
//                        }
//
//                        if (result == false)
//                        {
//                            MediaObjectInfo info;
//                            //qDebug() << files.at(i).fileName();
//
//                            music_Table->insertRow(music_Table->rowCount());
//                            int index = music_Table->rowCount()-1;
//                            if(index < 0)
//                                index =0;
//
//                            QString number;
//                            if (index+1<10)
//                                number = "0" + QString::number(index+1,10);
//                            else
//                                number = QString::number(index+1,10);
//
//                            QTableWidgetItem *item0 = new QTableWidgetItem(number);
//                            music_Table->setItem(index,0,item0);
//
//                            QStringList songinfo = songname.split("-");
//
//                            info.fileName = songinfo.at(0);
//
//                            if (songinfo.count()>1)
//                                info.singer = songinfo.at(1);
//                            else
//                                info.singer = "δ֪����";
//
//                            info.playtime = "00:00";
//
//                            info.filePath = files.at(i).filePath();
//                            //qDebug() << "path:" << files.at(i).filePath();
//
//                            QTableWidgetItem *item1 = new QTableWidgetItem(info.fileName);
//                            music_Table->setItem(index,1,item1);
//                            QTableWidgetItem *item2 = new QTableWidgetItem(info.singer);
//                            music_Table->setItem(index,2,item2);
//                            QTableWidgetItem* item3 = new QTableWidgetItem("δ֪ר��");
//                            music_Table->setItem(index,3,item3);
//                            QTableWidgetItem* item4 = new QTableWidgetItem(info.playtime);
//                            music_Table->setItem(index,4,item4);
//
//                            float size = (float)files.at(i).size()/1000.0f/1000.0f;
//                            QString string_size = QString::number(size,'f',1) + "M";
//
//                            QTableWidgetItem* item5 = new QTableWidgetItem(string_size);
//                            music_Table->setItem(index,5,item5);
//
//                            info.filesize = string_size;
//                            mediaObjectInfo.append(info);
//                        }
//                    }
//                }
//            }
//            count++;
//        }
//        music_Table->setRowCount(mediaObjectInfo.count());
//        displayLabel[1]->setText(__QString("��")+QString::number(mediaObjectInfo.count(),10)+ __QString("��"));
//    }
//
//    delete selectDia;
//}






//---------------------------Dialog--------------------------------------//
selectDia::selectDia(QWidget *parent) :
    QDialog(parent)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();

    int x = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->width()/2-225+desktopWidget->availableGeometry().x();
    int y = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->height()/2-150+desktopWidget->availableGeometry().y();

    this->setGeometry(x,y,450,300);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    dialog_vLayout = new QVBoxLayout();

    select_LayoutInit();

    LoadStyleSheet();

    ConnectInit();
}

selectDia::selectDia(QWidget *parent ,QList <QString> list):
    QDialog(parent)
{
    Dir_List = list;

    QDesktopWidget *desktopWidget = QApplication::desktop();

    int x = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->width()/2-225+desktopWidget->availableGeometry().x();
    int y = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->height()/2-150+desktopWidget->availableGeometry().y();

    this->setGeometry(x,y,450,300);
    //this->setWindowFlag(Qt::FramelessWindowHint);

    dialog_vLayout = new QVBoxLayout();

    select_LayoutInit();

    LoadStyleSheet();

    ConnectInit();
}

void selectDia::LoadStyleSheet(void)
{
    QFile file(":/qss/stackqss/stack_localMusicDialog.qss");

    file.open(QIODevice::ReadOnly);

    if (file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void selectDia::select_LayoutInit(void)
{
    closeButton = new QPushButton();
    closeButton->setObjectName("selectDialog_closeButton");
    closeButton->setMaximumSize(20,20);
    closeButton->setCursor(Qt::PointingHandCursor);
    QHBoxLayout *Head_hLayout = new QHBoxLayout();
    Head_hLayout->addStretch();
    Head_hLayout->addWidget(closeButton);
    Head_hLayout->addSpacing(5);


    headLabel = new QLabel();
    headLabel->setMinimumSize(450,30);
    headLabel->setText(__QString("ѡ�񱾵������ļ���"));
    headLabel->setAlignment(Qt::AlignCenter);

    QFont font = headLabel->font();
    font.setBold(true);
    font.setPointSize(13);
    headLabel->setFont(font);

    describLabel = new QLabel();
    describLabel->setText(__QString("���Զ�ɨ���㹴ѡ��Ŀ¼���ļ�����ɾʵʱͬ����"));

    QPalette pe;
    pe.setColor(QPalette::WindowText,QColor(115,115,115));
    describLabel->setPalette(pe);

    /*Dir_List.append("�ҵ�����");
    QDir dir(QCoreApplication::applicationDirPath() + "/mp3");
    QDir dirbsolutePath(dir.absolutePath());
    Dir_List.append(dirbsolutePath.path());*/

    listPath = new QListWidget();
    listPath->setViewMode(QListView::ListMode);
    listPath->setObjectName("selectDialog_listPath");
    listPath->setMinimumSize(350,100);
    listPath->setFocusPolicy(Qt::NoFocus);
    listPath->verticalScrollBar()->setFixedWidth(7);

    foreach(QString path, Dir_List)
    {
        QListWidgetItem *pathItem = new QListWidgetItem();
        //����Item�߶�
        pathItem->setSizeHint(QSize(0,30));
        pathItem->setText(path);
        pathItem->setCheckState(Qt::Checked);
        listPath->addItem(pathItem);    //����һ����Ŀ
    }

    confirm_Button = new QPushButton();
    confirm_Button->setText(__QString("ȷ��"));
    confirm_Button->setMaximumSize(90,36);
    confirm_Button->setMinimumSize(90,36);
    confirm_Button->setObjectName("selectDialog_confirbtn");
    addfolder_Button = new QPushButton();
    addfolder_Button->setText(__QString("����ļ���"));
    addfolder_Button->setMaximumSize(115,36);
    addfolder_Button->setMinimumSize(115,36);
    addfolder_Button->setObjectName("selectDialog_addbtn");

    QHBoxLayout *button_hLayout = new QHBoxLayout();

    button_hLayout->addStretch();
    button_hLayout->addWidget(confirm_Button);
    button_hLayout->addSpacing(5);
    button_hLayout->addWidget(addfolder_Button);
    button_hLayout->addStretch();

    dialog_vLayout->addLayout(Head_hLayout);
    dialog_vLayout->addWidget(headLabel);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addWidget(describLabel);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addWidget(listPath);
    dialog_vLayout->addSpacing(5);
    dialog_vLayout->addLayout(button_hLayout);
    dialog_vLayout->addSpacing(20);

    this->setLayout(dialog_vLayout);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_StyledBackground);
    //�����ޱ߿�dialog����ק
    this->setSizeGripEnabled(true);
    //���ô��ڲ��ܸı��С
    this->setFixedSize(this->geometry().size());


    //��Dialog����Բ�Ǳ߿�
    QBitmap bmp(QSize(this->size()));
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),5,5);
    setMask(bmp);
}

void selectDia::ConnectInit(void)
{
    //�رհ�ť��
    connect(closeButton,SIGNAL(clicked()),this,SLOT(btn_close_slot()));

    //ȷ����ť��
    connect(confirm_Button,SIGNAL(clicked()),this,SLOT(confirmclicked_slot()));
    //����ļ��а�ť��
    connect(addfolder_Button,SIGNAL(clicked()),this,SLOT(addfolder_slot()));
    //·��List�����
    connect(listPath,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(PathlistClicked_slot(QListWidgetItem *)));

}

/*void selectDia::PathlistClicked_slot(QListWidgetItem *item)
{
    if(item->checkState()==Qt::Checked)
    {
        item->setCheckState(Qt::Unchecked);
    }
    else
    {
        item->setCheckState(Qt::Checked);
    }

}

void selectDia::btn_close_slot(void)
{
    reject();
}

void selectDia::confirmclicked_slot(void)
{
    accept();
}
void selectDia::addfolder_slot(void)
{
    QString DirPath = QFileDialog::getExistingDirectory(this);
    Dir_List.append(DirPath);

    QListWidgetItem *addItem = new QListWidgetItem();
    addItem->setText(DirPath);
    addItem->setCheckState(Qt::Checked);
    listPath->addItem(addItem);
    //qDebug() << "path:" << DirPath;
}*/

static QPoint dialog_last(0,0);        //��������

void selectDia::mousePressEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        dialog_last = event->globalPos(); //��ȡ����ѹ��λ��
    }
}
void selectDia::mouseMoveEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        int dx = event->globalX() - dialog_last.x();
        int dy = event->globalY() - dialog_last.y();
        dialog_last = event->globalPos();
        this->move(this->x()+dx,this->y()+dy);
    }
}
void selectDia::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->y() <100)
    {
        int dx = event->globalX() - dialog_last.x();
        int dy = event->globalY() - dialog_last.y();
        this->move(this->x()+dx,this->y()+dy);
    }
}
